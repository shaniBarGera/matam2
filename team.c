//
// Created by shani on 01/05/2018.
//
#include "driver.h"
#include "team.h"
#include "season.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct team {
    char* name;
    Driver driver1;
    Driver driver2;
};

static void TeamSetStatus(TeamStatus *status, enum teamStatus wanted_status);

void printTeam(Team team){
    if(team == NULL) {
        printf("NULL\n---\n---\n");
    }
    else {
        TeamStatus status;
        printf("%s   %d\n", team->name, TeamGetPoints(team, &status));
        if (team->driver1 == NULL) printf("---\n");
        else printDriver(team->driver1);
        if (team->driver2 == NULL) printf("---\n");
        else printDriver(team->driver2);
    }
    printf("\n");

}

Team TeamCreate(TeamStatus* status, char* name){
    if( name == NULL) {
        TeamSetStatus(status, TEAM_NULL_PTR);
        return NULL;
    }
    Team team = malloc(sizeof(*team));
    if(team == NULL){
        TeamSetStatus(status, TEAM_MEMORY_ERROR);
        return  NULL;
    }
    TeamSetStatus(status, TEAM_STATUS_OK);
    team->name = name;
    team->driver1 = NULL;
    team->driver2 = NULL;
    return team;
}
void TeamDestroy(Team team){
    free(team);
}
TeamStatus TeamAddDriver(Team team, Driver driver){
    if(team == NULL || driver == NULL) return TEAM_NULL_PTR;
    if(team->driver1 == NULL) {
        team->driver1 = driver;
        return TEAM_STATUS_OK;
    }
    else if(team->driver2 == NULL) {
        team->driver2 = driver;
        return TEAM_STATUS_OK;
    }
    return TEAM_FULL;
}
const char * TeamGetName(Team  team){
    if(team == NULL || team->name == NULL) return "null";
    return team->name;
}
Driver TeamGetDriver(Team team, DriverNumber driver_number){
    if(driver_number == FIRST_DRIVER && team != NULL && team->driver1 != NULL)
        return team->driver1;
    else if(driver_number == SECOND_DRIVER && team != NULL &&
            team->driver2 != NULL)
        return team->driver2;
    else return NULL;
}
int TeamGetPoints(Team  team, TeamStatus *status){
    if(status == NULL || team == NULL) {
        TeamSetStatus(status, TEAM_NULL_PTR);
        return 0;
    }
    TeamSetStatus(status, TEAM_STATUS_OK);
    DriverStatus driver_status;
    return DriverGetPoints(team->driver1, &driver_status) +
           DriverGetPoints(team->driver2, &driver_status);
}

static void TeamSetStatus(TeamStatus *status, enum teamStatus wanted_status) {
    if (status != NULL) *status = wanted_status;
}
