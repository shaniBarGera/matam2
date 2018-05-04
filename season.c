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

static void MakeDriver(char* word, Season season, int num_of_drivers,
                       int num_of_teams, Driver * drivers, Team * teams){
    DriverStatus driver_status;
    drivers[num_of_drivers] = DriverCreate(&driver_status, word,
                                           num_of_drivers+1);
    DriverSetTeam(drivers[num_of_drivers], teams[num_of_teams]);
    TeamAddDriver(teams[num_of_teams], drivers[num_of_drivers]);
    DriverSetSeason(drivers[num_of_drivers], season);
}

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
    season->teams = malloc(sizeof(Team)*((rows_num-1)/3));
    season->drivers = malloc(sizeof(Driver)*((rows_num-1)/3)*2);

    if(!season->teams || !season->drivers){
        *status = SEASON_MEMORY_ERROR;
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

    return season;
}

void SeasonDestroy(Season season) { //LO TOV! We have to free everything one by one
    free(season);
}
Driver SeasonGetDriverByPosition(Season season, int position, SeasonStatus* status) {
    if (season == NULL || season->drivers == NULL || season->numOfDrivers <= 0) {
        *status = SEASON_NULL_PTR;
        return NULL;
    }
    if (position > season->numOfDrivers || position <= 0) {
        *status = SEASON_MEMORY_ERROR;
        return NULL;
    }

    *status = SEASON_OK;
    return season->drivers[position-1];
}

Driver* SeasonGetDriversStandings(Season season) {
    if(season == NULL) {
        return NULL;
    }
    return season->drivers;
}

Team SeasonGetTeamByPosition(Season season, int position, SeasonStatus* status) {
    if (season == NULL || season->teams == NULL || season->numOfTeams <= 0) {
        *status = SEASON_NULL_PTR;
        return NULL;
    }
    if (position > season->numOfTeams || position <= 0) {
        *status = SEASON_MEMORY_ERROR;
        return NULL;
    }

    *status = SEASON_OK;
    return season->teams[position - 1];
}
Team* SeasonGetTeamsStandings(Season season) {
    if(season == NULL) {
        return NULL;
    }
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
SeasonStatus SeasonAddRaceResult(Season season, int* results) {
            if (season == NULL || results == NULL || season->numOfDrivers <= 0 ||
                season->drivers == NULL) {
                return SEASON_NULL_PTR;
            }
            for(int i = 0; i < season->numOfDrivers ; i++)
            {
                for(int position=0; position < season->numOfDrivers; position++) {
                 if (DriverGetId((season->drivers)[i]) == results[position]) {
                        DriverStatus status =
                             DriverAddRaceResult((season->drivers)[results[i]],
                                            position);
                     //We can make it more yail by stopping the loop once getting to the right driver
            }
        }
    }
    driver_max_sort(season->drivers, season->numOfDrivers);
    team_max_sort(season->teams, season->numOfTeams);
    return SEASON_OK;
}
void driver_max_sort(Driver* drivers, int num_of_drivers) {
    int highest = 0;
    int highest_pos = 0;
    int id = 0;
    for (int i = 0; i < num_of_drivers; i++) {
        for (int j = i; j < num_of_drivers; j++) {
            id = DriverGetId(drivers[j]);
            if (id > highest) {
                highest = id;
                highest_pos = j;
            }
        }
        swap_driver(i, highest_pos, drivers);
    }
}
void team_max_sort(Team* teams, int num_of_teams) {
    int highest = 0;
    int highest_pos = 0;
    int id = 0;
    for (int i = 0; i < num_of_teams; i++) {
        for (int j = i; j < num_of_teams; j++) {
            id = DriverGetId(teams[j]);
            if (id > highest) {
                highest = id;
                highest_pos = j;
            }
        }
        swap_teams(i, highest_pos, teams);
    }
}
/*static Driver* sortDrivers(Driver* drivers, int num) {
    Driver* sorted_drivers = malloc(sizeof(Driver)*(SeasonGetNumberOfDrivers(season)));
    if(sorted_drivers == NULL) {
        free(sorted_drivers);
        return NULL;
    }
    copy_driver_arr(sorted_drivers, SeasonGetDriversStandings(season), season);
    driver_bubble_sort(drivers, season->numOfDrivers);
    return sorted_drivers;
}
static Team * sortTeam(Season season){
    Team * sorted_teams = malloc(sizeof(Team)*(SeasonGetNumberOfTeams(season));
    if(sorted_teams == NULL) {
        free(sorted_teams);
        return NULL;
    }
    copy_teams_arr(sorted_teams, SeasonGetTeamsStandings(season), season);
    team_bubble_sort(sorted_teams, season);
    return sorted_teams;
}
*/
void swap_driver(int a, int b, Driver* arr){
    void* temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}
void swap_teams(int a, int b, Team* arr){
    Team temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

/*void copy_driver_arr(Driver * arr1, Driver * arr2, Season season){
    for(int i=0; i < SeasonGetNumberOfDrivers(season); i++) {
        arr1[i] = arr2[i];
    }
}
void copy_teams_arr(Team * arr1, Team * arr2, Season season){
    for(int i=0; i < SeasonGetNumberOfTeams(season); i++) {
        arr1[i] = arr2[i];
    }
}

void driver_bubble_sort(Driver * drivers, int number_of_drivers) {
    int len = number_of_drivers-1, not_sorted=1;
    while(len > 1 && not_sorted)
    {
        not_sorted=0;
        for(int i=1; i<len; i++)
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

void team_bubble_sort(Team * sorted_teams, int num_of_teams){
    int len = num_of_teams-1, not_sorted=1;
    while(len > 1 && not_sorted)
    {
        not_sorted=0;
        for(int i=1; i<len; i++)
        {
            TeamStatus status1, status2;
            if(TeamGetPoints(sorted_teams[i-1], &status1) >=
               TeamGetPoints(sorted_teams[i], &status2)) {
                swap_teams(i, i - 1, sorted_teams);
                not_sorted = 1;
            }
        }
        len--;
    }
}
*/
