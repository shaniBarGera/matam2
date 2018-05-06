#include "driver.h"
#include "team.h"
#include "season.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct season {
    int year;
    int drivers_num;
    Driver* drivers;
    int teams_num;
    Team* teams;
    int* last_game;
};

static void SetStatus(SeasonStatus *status, enum seasonStatus wanted_status);
static void SetDriver(Driver* drivers, Team* teams, char* word, Season season);
static void MaxSortDrivers(Driver* drivers, int len, const int* last_game);
static void MaxSortTeams(Team* teams, int len, const int* last_game);
static void SwapDrivers(Driver* arr, int a, int b);
static void SwapTeams(Team* arr, int a, int b);
static bool CheckString(const char* string, SeasonStatus* status,
                       Season season, enum seasonStatus wanted_status);
static bool CheckSeason(Season season, SeasonStatus* status,
                        enum seasonStatus wanted_status);
static bool CheckDrivers(Driver* drivers, SeasonStatus* status, Season season,
                         enum seasonStatus wanted_status);
static bool CheckTeams(Team* teams, SeasonStatus* status, Season season,
                       enum seasonStatus wanted_status);
static bool CheckResults(int* results, SeasonStatus* status, Season season,
                       enum seasonStatus wanted_status);

Season SeasonCreate(SeasonStatus* status, const char* season_info) {
    // Check inputs
    if(CheckString(season_info, status, NULL, SEASON_NULL_PTR)) return NULL;

    // Allocate memory to season and it's arrays and initialize them
    Season season = malloc(sizeof(*season));
    if(CheckSeason(season, status, SEASON_MEMORY_ERROR)) return NULL;
    season->drivers = malloc(sizeof(Driver));;
    season->teams = malloc(sizeof(Team));
    if(CheckDrivers(season->drivers, status, season, SEASON_MEMORY_ERROR)||
       CheckTeams(season->teams, status, season, SEASON_MEMORY_ERROR))
        return NULL;
    season->drivers[0] = NULL;
    season->teams[0] = NULL;

    // Split info to words
    char* info_aux = malloc(sizeof(char) * (strlen(season_info) + 1));
    if(CheckString(info_aux, status, season, SEASON_MEMORY_ERROR)) return NULL;
    strcpy(info_aux, season_info);
    char* word = strtok(info_aux, "\n");

    // Initialize variables
    int counter = 0;
    season->drivers_num = 0;
    season->teams_num = 0;
    season->year = atoi(word);
    word = strtok(NULL, "\n");

    // Update info
    while (word) {
        if (counter % 3 == 0) {     // Make team
            season->teams = (Team*)realloc(season->teams,
                              sizeof(Team) * (season->teams_num + 1));
            season->teams[season->teams_num] = TeamCreate(NULL, word);
            season->teams_num++;
        } else if (strcmp(word, "None")) {    // Make driver
            season->drivers = (Driver*)realloc(season->drivers,
                               sizeof(Driver) * (season->drivers_num + 1));
            SetDriver(season->drivers, season->teams, word, season);
            season->drivers_num++;
        }

        counter++;
        word = strtok(NULL, "\n");
    }

    // Allocate and initialize last game
    season->last_game = malloc(sizeof(int) * season->drivers_num);
    if(CheckResults(season->last_game, status, season, SEASON_MEMORY_ERROR))
        return NULL;
    for(int i = 0; i < season->drivers_num; i++){
        season->last_game[i] = 0;
    }

    free(info_aux);
    free(word);
    SetStatus(status, SEASON_OK);
    return season;
}
void SeasonDestroy(Season season) {
    if(!season) return;
    if(season->drivers) {
        for (int i = 0; i < season->drivers_num; i++){
            DriverDestroy(season->drivers[i]);
        }
    }
    if(season->teams) {
        for (int i = 0; i < season->teams_num; i++) {
            TeamDestroy(season->teams[i]);
        }
    }
    free(season->last_game);
    free(season->teams);
    free(season->drivers);
    free(season);
}
Driver SeasonGetDriverByPosition(Season season, int position,
                                 SeasonStatus* status) {
    // Check input
    if(CheckSeason(season, status, SEASON_NULL_PTR)) return NULL;
    if (position > season->drivers_num || position <= 0) {
        SetStatus(status, BAD_SEASON_INFO);
        return NULL;
    }

    // Get driver's position
    Driver * sorted_drivers = SeasonGetDriversStandings(season);
    if(!sorted_drivers){
        SetStatus(status, SEASON_NULL_PTR);
        free(sorted_drivers);
        return NULL;
    }
    Driver driver = sorted_drivers[position-1];
    if(!driver) {
        SetStatus(status, SEASON_NULL_PTR);
        free(sorted_drivers);
        return NULL;
    }

    free(sorted_drivers);
    SetStatus(status, SEASON_OK);
    return driver;
}
Driver* SeasonGetDriversStandings(Season season) {
    // Check input
    if(!season || !season->drivers) return NULL;

    // Sort drivers
    Driver* sorted_drivers = malloc(sizeof(Driver) * season->drivers_num);
    for (int i = 0; i < season->drivers_num; i++) {
        sorted_drivers[i] = season->drivers[i];
    }
    MaxSortDrivers(sorted_drivers, season->drivers_num, season->last_game);
    return sorted_drivers;
}
Team SeasonGetTeamByPosition(Season season, int position, SeasonStatus* status) {
    // Check input
    if (!season) {
        SetStatus(status, SEASON_NULL_PTR);
        return NULL;
    }
    if (position > season->teams_num || position <= 0) {
        SetStatus(status, BAD_SEASON_INFO);
        return NULL;
    }

    // Get team position
    Team * sorted_teams = SeasonGetTeamsStandings(season);
    if(!sorted_teams){
        SetStatus(status, SEASON_MEMORY_ERROR);
        free(sorted_teams);
        return NULL;
    }
    Team team = sorted_teams[position-1];
    if(!team) {
        SetStatus(status, SEASON_MEMORY_ERROR);
        free(sorted_teams);
        return NULL;
    }

    free(sorted_teams);
    SetStatus(status, SEASON_OK);
    return team;
}
Team* SeasonGetTeamsStandings(Season season) {
    // Check input
    if(!season || !season->teams) return NULL;

    // Sort teams
    Team* sorted_teams = malloc(sizeof(Team) * season->teams_num);
    for (int i = 0; i < season->teams_num; i++) {
        sorted_teams[i] = season->teams[i];
    }
    MaxSortTeams(sorted_teams, season->teams_num, season->last_game);
    return sorted_teams;
}
int SeasonGetNumberOfDrivers(Season season) {
    if (!season) return 0;
    return season->drivers_num;
}
int SeasonGetNumberOfTeams(Season season) {
    if (!season) return 0;
    return season->teams_num;
}
SeasonStatus SeasonAddRaceResult(Season season, int* results) {
    // Check input
    if(!season || !results || !season->drivers) return SEASON_NULL_PTR;

    //update last_game
    for (int i = 0; i < season->drivers_num; i++) {
        season->last_game[i] = results[i];
    }
    for(int i = 0; i < season->drivers_num; i++)
    {
        int position = 0;
        while(DriverGetId(season->drivers[i]) != results[position]){
            position++;
        }
        DriverAddRaceResult(season->drivers[i], position+1);
    }

    return SEASON_OK;
}

static void SetStatus(SeasonStatus *status, SeasonStatus wanted_status) {
    if (status != NULL) *status = wanted_status;
}
static void SetDriver(Driver * drivers, Team * teams, char* word,
                      Season season) {
    // Define variables
    int drivers_num = SeasonGetNumberOfDrivers(season),
            teams_num = SeasonGetNumberOfTeams(season) - 1;

    // Set driver
    drivers[drivers_num] = DriverCreate(NULL, word, drivers_num + 1);
    DriverSetTeam(drivers[drivers_num], teams[teams_num]);
    TeamAddDriver(teams[teams_num], drivers[drivers_num]);
    DriverSetSeason(drivers[drivers_num], season);
}
static void SwapDrivers(Driver* arr, int a, int b) {
    Driver temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}
static void SwapTeams( Team* arr, int a, int b){
    Team temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}
static void MaxSortDrivers(Driver* drivers, int len, const int* last_game) {
    // Define variables
    int max = 0, max_pos = 0,
            points = 0,
            id1 = 0, id2 = 0;

    for (int i = 0; i < len; i++) {
        max = DriverGetPoints(drivers[i], NULL);
        max_pos = i;
        for (int j = i + 1; j < len; j++) {
            points = DriverGetPoints(drivers[j], NULL);
            if (points > max) {
                max = points;
                max_pos = j;
            }

            // Comparing with results in case of tie
            if (points == max) {
                for (int k = 0; k < len; k++) {
                    id1 = DriverGetId(drivers[j]);
                    id2 = DriverGetId(drivers[max_pos]);
                    if (last_game[k] == id1) {
                        max_pos = j;
                        break;
                    } else if (last_game[k] == id2) {
                        break;
                    }
                }
            }
        }
        SwapDrivers(drivers, i, max_pos);
    }
}
static void MaxSortTeams(Team* teams, int len, const int* last_game) {
    // Define variables
    int max = 0, max_pos = 0,
            points = 0,
            team1id1 = 0, team1id2 = 0,
            team2id1 = 0, team2id2 = 0;

    for (int i = 0; i < len; i++) {
        max = TeamGetPoints(teams[i], NULL);
        max_pos = i;
        for (int j = i + 1; j < len; j++) {
            points = TeamGetPoints(teams[j], NULL);
            if (points > max) {
                max = points;
                max_pos = j;
            }

            // Comparing with results in case of tie
            if (points == max) {
                team1id1 = DriverGetId(TeamGetDriver(teams[j], FIRST_DRIVER));
                team1id2 = DriverGetId(TeamGetDriver(teams[j], SECOND_DRIVER));
                team2id1 = DriverGetId(TeamGetDriver(teams[max_pos],
                                                     FIRST_DRIVER));
                team2id2 = DriverGetId(TeamGetDriver(teams[max_pos],
                                                     SECOND_DRIVER));
                for (int k = 0; k < len; k++) {
                    if (last_game[k] == team1id1 || last_game[k] == team1id2) {
                        max_pos = j;
                        break;
                    } else if (last_game[k] == team2id1 ||
                            last_game[k] == team2id2) {
                        break;
                    }
                }
            }
        }
        SwapTeams(teams, i, max_pos);
    }
}
static bool CheckString(const char* string, SeasonStatus* status,
                         Season season, enum seasonStatus wanted_status){
    if(!string) {
        SetStatus(status, wanted_status);
        SeasonDestroy(season);
        return true;
    }
    return false;
}
static bool CheckSeason(Season season, SeasonStatus* status,
                        enum seasonStatus wanted_status){
    if(!season) {
        SetStatus(status, wanted_status);
        return true;
    }
    return false;
}
static bool CheckDrivers(Driver* drivers, SeasonStatus* status, Season season,
                         enum seasonStatus wanted_status){
    if(!drivers) {
        SetStatus(status, wanted_status);
        SeasonDestroy(season);
        return true;
    }
    return false;
}
static bool CheckTeams(Team* teams, SeasonStatus* status, Season season,
                       enum seasonStatus wanted_status){
    if(!teams) {
        SetStatus(status, wanted_status);
        SeasonDestroy(season);
        return true;
    }
    return false;
}
static bool CheckResults(int* results, SeasonStatus* status, Season season,
                         enum seasonStatus wanted_status){
    if(!results) {
        SetStatus(status, wanted_status);
        SeasonDestroy(season);
        return true;
    }
    return false;
}