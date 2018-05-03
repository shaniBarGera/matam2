#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "season.h"
#include "team.h"
#include "driver.h"

struct season {
    int year;
    int numOfTeams;
    Team* teams;
    int numOfDrivers;
    Driver* drivers;
};

int GetYear(Season season)
{
    return season->year;
}

static int CountRows(char* info_aux) {
    int numOfRows = 1;
    char i = 0;
    while (info_aux[i]) {
        if (info_aux[i] == '\n') {
            numOfRows++;
        }
        i++;
    }
    return numOfRows;
}

static char* CopyString(const char* season_info){
    int len = strlen(season_info);
    char* info_aux = malloc(len);
    strcpy(info_aux, season_info);
    return info_aux;
}



//Should be "Season" not "struct season"!
Season SeasonCreate(SeasonStatus* status, const char* season_info) {
    Season season = malloc(sizeof(*season));

    //Check inputs
    if (!season || !season_info) {
        *status = SEASON_NULL_PTR;
        return NULL;
    }

    char* info_aux = CopyString(season_info);
    int rows_num = CountRows(info_aux);

    //allocate memory to arrays
    season->teams = malloc(sizeof(Team) *((rows_num-1)/3));
    season->drivers = malloc(sizeof(Driver)*((rows_num-1)/3)*2);
    if(!season->teams || !season->drivers){
        *status = SEASON_MEMORY_ERROR;
        return NULL;
    }

    //Split info aux
    char * word = strtok(info_aux, "\n");

    //Enter year
    season->year = atoi(word);
    word = strtok(NULL, "\n");

    //Initialize variables
    season->numOfTeams = 0;
    season->numOfDrivers = 0;
    int counter = 0;

    //Go through info
    while (word) {

        //make team
        if (counter % 3 == 0) {
            TeamStatus team_status;
            season->teams[season->numOfTeams] = TeamCreate(&team_status, word);
            season->numOfTeams++;

        }

        //make driver
        else if (strcmp(word, "None")) {
            DriverStatus driver_status;
            season->drivers[season->numOfDrivers] =
                    DriverCreate(&driver_status, word, season->numOfDrivers);
            TeamAddDriver(season->teams[season->numOfTeams-1],
                          season->drivers[season->numOfDrivers]);
            DriverSetSeason(season->drivers[season->numOfDrivers], season);
            season->numOfDrivers++;

        }

        counter++;
        word = strtok(NULL, "\n");
    }

    return season;
}

void SeasonDestroy(Season season) {
    for(int i=0; i<season->numOfDrivers; i++)
        DriverDestroy(season->drivers[i]);
    free(season->drivers);
    for(int i=0; i<season->numOfTeams; i++)
        TeamDestroy(season->teams[i]);
    free(season->teams);
    free(season);
}
Driver SeasonGetDriverByPosition(Season season, int position, SeasonStatus* status) {
    if (season == NULL || season->drivers == NULL || season->numOfDrivers <=0)
        return NULL;
    if (position > season->numOfDrivers || position <= 0) {
        *status = SEASON_INVALID_POSITION;
        return NULL;
    }

    *status = SEASON_OK;
    return season->drivers[position-1];
}

Driver* SeasonGetDriversStandings(Season season) {
    if(season == NULL) return NULL;
    return season->drivers;
}

Team SeasonGetTeamByPosition(Season season, int position, SeasonStatus* status) {
    if (season == NULL || season->teams == NULL || season->numOfTeams <= 0)
        return NULL;
    if (position > season->numOfTeams || position <= 0) {
        *status = SEASON_INVALID_POSITION;
        return NULL;
    }

    *status = SEASON_OK;
    return season->teams[position - 1];
}
Team* SeasonGetTeamsStandings(Season season) {
    if(season == NULL) return NULL;
    return season->teams;
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
/*SeasonStatus SeasonAddRaceResult(Season season, int* results) {
    if (season == NULL || results == NULL || season->numOfDrivers <= 0 ||
        season->drivers == NULL) {
        return SEASON_NULL_PTR;
    }

    for(int i=0; i < season->numOfDrivers ; i++)
    {
        for(int position=0; position < season->numOfDrivers; position++) {
            if (DriverGetId((season->drivers)[i]) == results[position]) {
                DriverStatus status =
                        DriverAddRaceResult((season->drivers)[results[i]],
                                            position);
            }
        }
    }

    driver_bubble_sort(season->drivers, season->numOfDrivers);
    team_bubble_sort(season->teams, season->numOfTeams);
    return SEASON_OK;
}
/*
static Driver * sortDrivers(Driver * drivers, int num){
    Driver * sorted_drivers = malloc(sizeof(Driver)*(SeasonGetNumberOfDrivers(season)));
    if(sorted_drivers == NULL) {
        free(sorted_drivers);
        return NULL;
    }
    //copy_driver_arr(sorted_drivers, SeasonGetDriversStandings(season), season);
    driver_bubble_sort(drivers, season->numOfDrivers);
    return sorted_drivers;
}

static Team * sortTeam(Season season){
    Team * sorted_teams = malloc(sizeof(Team)*(SeasonGetNumberOfTeams(season));
    if(sorted_teams == NULL) {
        free(sorted_teams);
        return NULL;
    }
    //copy_teams_arr(sorted_teams, SeasonGetTeamsStandings(season), season);
    team_bubble_sort(sorted_teams, season);
    return sorted_teams;
}

static void swap_driver(int a, int b, Driver * arr){
    void * temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}
static void swap_teams(int a, int b, Team * arr){
    Team temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}
/*
void copy_driver_arr(Driver * arr1, Driver * arr2, Season season){
    for(int i=0; i < SeasonGetNumberOfDrivers(season); i++) {
        arr1[i] = arr2[i];
    }
}

void copy_teams_arr(Team * arr1, Team * arr2, Season season){
    for(int i=0; i < SeasonGetNumberOfTeams(season); i++) {
        arr1[i] = arr2[i];
    }
}

static void driver_bubble_sort(Driver * drivers, int number_or_drivers){
    int len = number_or_drivers-1, not_sorted=1;
    while(len > 1 && not_sorted)
    {
        not_sorted=0;
        for(int i=0; i<len; i++)
        {
            DriverStatus status1, status2;
            if(DriverGetPoints(drivers[i-1], &status1) >=
               DriverGetPoints(drivers[i], &status2)) {
                swap_driver(i, i - 1, drivers);
                not_sorted = 1;
            }
        }
        len--;
    }
}

static void team_bubble_sort(Team * sorted_teams, Season season){
    int len = SeasonGetNumberOfTeams(season)-1, not_sorted=1;
    while(len > 1 && not_sorted)
    {
        not_sorted=0;
        for(int i=0; i<len; i++)
        {
            TeamStatus status1, status2;
            if(TeamGetPoints(sorted_teams[i-1], &status1) >=
               TeamGetPoints(sorted_teams[i], &status1)) {
                swap_teams(i, i - 1, sorted_teams);
                not_sorted = 1;
            }
        }
        len--;
    }
}*/