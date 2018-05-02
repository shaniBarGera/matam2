#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cmake-build-debug/season.h"

struct season {
    int year;
    int numOfTeams;
    Team* teams;
    int numOfDrivers;
    Driver* drivers;
};

//Should be "Season" not "struct season"!
Season SeasonCreate(SeasonStatus* status, const char* season_info) {
    Season season1 = malloc(sizeof(*season1));
    if (!season1) {
        *status = SEASON_MEMORY_ERROR;
    }
    else {
        int length = strlen(season_info);
        char *season_info_aux = malloc(length);
        memcpy(season_info_aux, season_info, length);
        char *token = strtok(season_info, "\n");
        season1->year = atoi(token);
        token = strtok(NULL, "\n");
        int counter = 0;
        while (token != NULL) {
            if (counter % 3 == 0) {
                as
            }
            season->teams[]
            counter++;
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
        status = SEASON_MEMORY_ERROR;
    }
    if (position < season->numOfDrivers) {
        *status = SEASON_MEMORY_ERROR;
    }
    season->drivers[position];
}
Driver* SeasonGetDriversStandings(Season season) {
    return season->drivers;
}
Team SeasonGetTeamByPosition(Season season, int position, SeasonStatus* status) {
    //nothingForNow
}
Team* SeasonGetTeamsStandings(Season season) {
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
    if (season == NULL || results == NULL) {
        return SEASON_NULL_PTR;
    }
    return SEASON_OK;
}
