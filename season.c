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
static void SwapDriver(Driver* arr, int a, int b);
static void SwapTeam(Team* arr, int a, int b);
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

    Driver * sorted_drivers = SeasonGetDriversStandings(season);
    if(sorted_drivers == NULL){
        free(sorted_drivers);
        return NULL;
    }
    Driver driver = sorted_drivers[position-1];
    if(driver == NULL) {
        free(sorted_drivers);
        return NULL;
    }
    free(sorted_drivers);
    SetStatus(status, SEASON_OK);
    return driver;
}
Driver* SeasonGetDriversStandings(Season season) {
    if(season == NULL || season->drivers == NULL) return NULL;
    Driver* sorted_drivers = malloc(sizeof(Driver)*season->numOfDrivers);
    for (int i = 0; i < season->numOfDrivers; i++) {
        sorted_drivers[i] = season->drivers[i];
    }
    maxSortDrivers(sorted_drivers, season->numOfDrivers, season->last_game);
    return sorted_drivers;
}
Team SeasonGetTeamByPosition(Season season, int position, SeasonStatus* status) {
    if (season == NULL) {
        SeasonSetStatus(status, SEASON_NULL_PTR);
        return NULL;
    }
    if (position > season->numOfTeams || position <= 0) {
        SeasonSetStatus(status, BAD_SEASON_INFO);
        return NULL;
    }

    SeasonSetStatus(status, SEASON_OK);
    Team * sorted_teams = SeasonGetTeamsStandings(season);
    if(sorted_teams == NULL){
        free(sorted_teams);
        return NULL;
    }
    Team team = sorted_teams[position-1];
    if(team == NULL) {
        free(sorted_teams);
        return NULL;
    }
    free(sorted_teams);
    return team;
}
Team* SeasonGetTeamsStandings(Season season) {
    if(season == NULL || season->teams == NULL) return NULL;
    Team* sorted_teams = malloc(sizeof(Team)*season->numOfTeams);
    for (int i=0; i < season->numOfTeams; i++) {
        sorted_teams[i] = season->teams[i];
    }
    maxSortTeams(sorted_teams, season->numOfTeams, season->last_game);
    return sorted_teams;
}
int SeasonGetNumberOfDrivers(Season season) {
    if (season == NULL) {
        return 0;
    }
    return season->numOfDrivers;
}
int SeasonGetNumberOfTeams(Season season) {
    if (season == NULL) {
        return 0;
    }
    return season->numOfTeams;
}
SeasonStatus SeasonAddRaceResult(Season season, int* results) {
    if(season == NULL || results == NULL || season->drivers == NULL)
        return SEASON_NULL_PTR;

    //update last_game
    for (int i = 0; i < season->numOfDrivers; i++) {
        season->last_game[i] = results[i];
    }
    for(int i = 0; i < season->numOfDrivers; i++)
    {
        int position = 0, Id = DriverGetId(season->drivers[i]);
        while(Id != results[position]){
            position++;
        }
        DriverAddRaceResult(season->drivers[i], position+1);
        //season->last_game[i] = position;
    }
    return SEASON_OK;
}
static void SeasonSetStatus(SeasonStatus *status, SeasonStatus wanted_status) {
    if (status != NULL) *status = wanted_status;
}
static void SetDriver(Driver * drivers, Team * teams, char* word,
                      Season season) {
    int drivers_num = SeasonGetNumberOfDrivers(season),
            teams_num = SeasonGetNumberOfTeams(season) - 1;
    DriverStatus driver_status;
    drivers[drivers_num] = DriverCreate(&driver_status, word,
                                           drivers_num + 1);
    DriverSetTeam(drivers[drivers_num], teams[teams_num]);
    TeamAddDriver(teams[teams_num], drivers[drivers_num]);
    DriverSetSeason(drivers[drivers_num], season);
}
static void swap_driver(int a, int b, Driver* arr){
    Driver temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}
static void swap_teams(int a, int b, Team* arr){
    Team temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}
static void maxSortDrivers(Driver* drivers, int len, const int* last_game) {
    int max = 0;
    int max_pos = 0;
    int points = 0;
    int id1 = 0, id2 = 0;
    int pos1=0,pos2=0;
    DriverStatus status;
    for (int i = 0; i < len; i++) {
        max = DriverGetPoints(drivers[i], &status);
        max_pos = i;
        for (int j = i+1; j < len; j++) {
            points = DriverGetPoints(drivers[j], &status);
            if (points > max) {
                max = points;
                max_pos = j;
            }
            //comparing with results in case of tie
            if (points == max) {
                for (int k = 0; k < len; k++) {
                    id1 = DriverGetId(drivers[j]);
                    id2 = DriverGetId(drivers[max_pos]);
                    if (last_game[k] == id1) {
                        pos1 = k;
                    }
                    else if (last_game[k] == id2) {
                        pos2 = k;
                    }
                }
                if(pos1<pos2){
                    max=points;
                    max_pos = j;
                }
            }
        }
        swap_driver(i, max_pos, drivers);
    }
}
static void maxSortTeams(Team* teams, int len, const int* last_game) {
    int max = 0;
    int max_pos = 0;
    int points = 0;
    int team1id1 = 0, team1id2 = 0, team2id1 = 0, team2id2 = 0;
    TeamStatus status;
    for (int i = 0; i < len; i++) {
        max = TeamGetPoints(teams[i], &status);
        max_pos = i;
        for (int j = i+1; j < len; j++) {
            points = TeamGetPoints(teams[j], &status);
            if (points > max) {
                max = points;
                max_pos = j;
            }
            //comparing with results in case of tie
            if (points == max) {
                team1id1 = DriverGetId(TeamGetDriver(teams[j], FIRST_DRIVER));
                team1id2 = DriverGetId(TeamGetDriver(teams[j], SECOND_DRIVER));
                team2id1 = DriverGetId(TeamGetDriver(teams[max_pos], FIRST_DRIVER));
                team2id2 = DriverGetId(TeamGetDriver(teams[max_pos], SECOND_DRIVER));
                for (int k = 0; k < len; k++) {
                    if (last_game[k] == team1id1 || last_game[k] == team1id2) {
                        max_pos = j;
                        break; //k = len;
                    }
                    if (last_game[k] == team2id1 || last_game[k] == team2id2) {
                        break; //k = len;
                    }
                }
            }
        }
        swap_teams(i, max_pos, teams);
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