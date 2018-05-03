#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "season.h"

struct season {
    int year;
    int numOfTeams;
    Team* teams;
    int numOfDrivers;
    Driver* drivers;
};

Season SeasonCreate(SeasonStatus* status, const char* season_info) {
    Season season1 = malloc(sizeof(*season1));
    if (!season1 || !season_info) {
        *status = SEASON_NULL_PTR;
        return NULL;
    }
    int length = strlen(season_info);
    char* info_aux = malloc(length);
    strcpy(info_aux, season_info);
    int numOfRows  = 1;
    char* i = info_aux;
    while(i != '\0') {
        if (*i == '\n') {
            numOfRows++;
        }
        i++;
    }
    season1->numOfTeams = (numOfRows - 1) / 3;
    season1->teams = malloc(sizeof(Team) * season1->numOfTeams);
    char* word = strtok(info_aux, "\n");
    season1->year = atoi(word);
    word = strtok(NULL, "\n");
    int counter = 0;
    season1->numOfDrivers = 0;
    while (word) {
        if (counter % 3 == 0) {
            void *status; //???
            season1->teams[counter / 3] = TeamCreate(status, word); //???
        } else {
            if (strcmp(word, "None")) {
                season1->numOfDrivers++;
            }
        }
        counter++;
        word = strtok(NULL, "\n");
    };
    season1->drivers = malloc(sizeof(Driver)*numOfDrivers);
    strcpy(info_aux, season_info);
    char* word = strtok(info_aux, "\n");
    char* word = strtok(NULL, "\n");
    counter = 0;
    while (word) {
        if (counter % 3 != 0) {
            if (strcmp(word, "None")) {
                //Add driver
                //Then add driver to team
                //hopefully done
            }
        }
    }
    return season;
}
void SeasonDestroy(Season season) {
    free(season);
}
Driver SeasonGetDriverByPosition(Season season, int position, SeasonStatus* status) {
    if (season == NULL) {
        *status = SEASON_NULL_PTR;
    }
    if (position < season->numOfDrivers) {
        *status = SEASON_MEMORY_ERROR;
    }
    return season->drivers[position];
}
Driver* SeasonGetDriversStandings(Season season) { //check NULL?
    return season->drivers;
}
Team SeasonGetTeamByPosition(Season season, int position, SeasonStatus* status) {
    if (season == NULL) {
        *status = SEASON_NULL_PTR;
    }
    if (position < season->numOfTeams) {
        *status = SEASON_MEMORY_ERROR;
    }
    return season->teams[position];
}
Team* SeasonGetTeamsStandings(Season season) { //check NULL?
    return season->teams;
}
int SeasonGetNumberOfDrivers(Season season) {
    if (!season) {
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
    if (season == NULL || results == NULL) {
        return SEASON_NULL_PTR;
    }
    return SEASON_OK;
}
