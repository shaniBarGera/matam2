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

static void SeasonSetStatus(SeasonStatus *status,
                            enum seasonStatus wanted_status);
static char* CopyString(const char* season_info);
static void MakeDriver(char* word, Season season, int num_of_drivers,
                       int num_of_teams, Driver * drivers, Team * teams);
static int CountRows(const char* info_aux);
static void swap_driver(int a, int b, Driver * arr);
static void swap_teams(int a, int b, Team * arr);
static void QuickSortDrivers(Driver * drivers ,int n);
static void QuickSortTeams(Team * teams ,int n);
static void initializeTeams(Team* teams, int len);

//int GetYear(Season season) {return season->year;}

Season SeasonCreate(SeasonStatus* status, const char* season_info) {
    Season season = malloc(sizeof(*season));

    //Check inputs
    if (!season || !season_info) {
        SeasonSetStatus(status, SEASON_NULL_PTR);
        return NULL;
    }

    char* info_aux = CopyString(season_info);
    int rows_num = CountRows(info_aux);

    //allocate memory to arrays and initialize them
    season->teams = malloc(sizeof(Team) *((rows_num-1)/3));
    season->drivers = malloc(sizeof(Driver)*((rows_num-1)/3)*2);
    if(!season->teams || !season->drivers){
        SeasonSetStatus(status ,SEASON_MEMORY_ERROR);
        free(info_aux);
        return NULL;
    }

    initializeTeams(season->teams, (rows_num-1)/3);
    initializeDrivers(season->drivers, ((rows_num-1)/3)*2);


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
            MakeDriver(word, season, season->numOfDrivers, season->numOfTeams-1,
                        season->drivers, season->teams);
            season->numOfDrivers++;
        }

        counter++;
        word = strtok(NULL, "\n");
    }
    free(info_aux);
    return season;
}
void SeasonDestroy(Season season) {
    for(int i=0; i<season->numOfDrivers; i++)
        DriverDestroy(season->drivers[i]);
    for(int i=0; i<season->numOfTeams; i++)
        TeamDestroy(season->teams[i]);
    free(season->teams);
    free(season->drivers);
    free(season);
}
Driver SeasonGetDriverByPosition(Season season, int position, SeasonStatus* status) {
    if (season == NULL)
        return NULL;
    if (position > season->numOfDrivers || position <= 0) {
        SeasonSetStatus(status, BAD_SEASON_INFO);
        return NULL;
    }

    SeasonSetStatus(status, SEASON_OK);
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
    return driver;
}

Driver* SeasonGetDriversStandings(Season season) {
    if(season == NULL) return NULL;
    Driver * sorted_drivers = malloc(sizeof(Driver)*season->numOfDrivers);
    sorted_drivers = season->drivers;
    QuickSortDrivers(sorted_drivers, season->numOfDrivers);
    return sorted_drivers;
}

Team SeasonGetTeamByPosition(Season season, int position, SeasonStatus* status) {
    if (season == NULL)
        return NULL;
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
    if(season == NULL) return NULL;
    Team * sorted_teams = malloc(sizeof(Team)*season->numOfTeams);
    if(sorted_teams == NULL) return NULL;
    sorted_teams = season->teams;
    QuickSortTeams(sorted_teams, season->numOfTeams);
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

}

static void SeasonSetStatus(SeasonStatus *status, SeasonStatus wanted_status) {
    if (status != NULL) *status = wanted_status;
}

static char* CopyString(const char* season_info){
    int len = strlen(season_info);
    char* info_aux = malloc(len);
    strcpy(info_aux, season_info);
    return info_aux;
}

static int CountRows(const char* info_aux) {
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

static void MakeDriver(char* word, Season season, int num_of_drivers,
                       int num_of_teams, Driver * drivers, Team * teams){
    DriverStatus driver_status;
    drivers[num_of_drivers] = DriverCreate(&driver_status, word,
                                           num_of_drivers+1);
    DriverSetTeam(drivers[num_of_drivers], teams[num_of_teams]);
    TeamAddDriver(teams[num_of_teams], drivers[num_of_drivers]);
    DriverSetSeason(drivers[num_of_drivers], season);
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

static void QuickSortDrivers(Driver * drivers ,int n){
    int p, b = 1, t = n-1;
    DriverStatus status;
    if(n<2)
        return;
    swap_driver(0, n/2, drivers);
    p=DriverGetPoints(drivers[0], &status);
    while(b<=t) {
        while (t >= b && DriverGetPoints(drivers[t], &status) >= p)
            t--;
        while (b <= t && DriverGetPoints(drivers[b], &status) < p)
            b++;
        if (b < t)
            swap_driver(b++, t--, drivers);
    }

    swap_driver(0, t, drivers);
    QuickSortDrivers(drivers, t);
    QuickSortDrivers(drivers+t+1, n-t-1);
}

static void QuickSortTeams(Team * teams ,int n){
    int p, b = 1, t = n-1;
    TeamStatus status;
    if(n<2)
        return;
    swap_teams(0, n/2, teams);
    p=TeamGetPoints(teams[0], &status);
    while(b<=t) {
        while (t >= b && TeamGetPoints(teams[t], &status) >= p)
            t--;
        while (b <= t && TeamGetPoints(teams[b], &status) < p)
            b++;
        if (b < t)
            swap_teams(b++, t--, teams);
    }

    swap_teams(0, t, teams);
    QuickSortTeams(teams, t);
    QuickSortTeams(teams+t+1, n-t-1);
}

static void initializeTeams(Team* teams, int len) {
    for (int i = 0; i < len; i++) {
        teams[i] = NULL;
    }
}
static void initializeDrivers(Driver* drivers, int len) {
    for (int i = 0; i < len; i++) {
        drivers[i] = NULL;
    }
}
