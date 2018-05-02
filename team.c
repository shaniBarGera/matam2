//
// Created by shani on 01/05/2018.
//
#include "driver.h"
#include "team.h"
#include "season.h"
#include <stdlib.h>
#include <stdio.h>

struct team {
    char* name;
    Driver driver1;
    Driver driver2;
};

Team TeamCreate(TeamStatus* status, char* name){
    Team team = malloc(sizeof(*team));
    if(team == NULL) *status = TEAM_MEMORY_ERROR;
    else {
        team->name = name;
        *status = TEAM_STATUS_OK;
        team->driver1 = NULL;
        team->driver2 = NULL;
    }
    return team;
}

void TeamDestroy(Team team){
    free(team);
}

TeamStatus TeamAddDriver(Team team, Driver driver){
    if(team == NULL || driver == NULL) return TEAM_NULL_PTR;
    if(team->driver1 != NULL) {
        if (team->driver2 != NULL) return TEAM_FULL;
        else team->driver2 = driver;
    }
    else team->driver1 = driver;
    return TEAM_STATUS_OK;
}

const char * TeamGetName(Team  team){
    if(team == NULL) return NULL;
    return team->name;
}

Driver TeamGetDriver(Team  team, DriverNumber driver_number){
    if(driver_number == FIRST_DRIVER && team != NULL && team->driver1 != NULL)
        return team->driver1;
    else if(driver_number == SECOND_DRIVER && team != NULL &&
            team->driver2 != NULL)
        return team->driver2;
    else return NULL;
}

int TeamGetPoints(Team  team, TeamStatus *status){
    if(*status == TEAM_STATUS_OK && team != NULL && team->driver1 != NULL &&
            team->driver2 != NULL) {
        DriverStatus driver_status;
        return DriverGetPoints(team->driver1, &driver_status) +
               DriverGetPoints(team->driver2, &driver_status);
    }
    return 0;
}