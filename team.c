#include "driver.h"
#include "team.h"
#include "season.h"

#include <stdlib.h>
#include <string.h>

struct team {
    char* name;
    Driver driver1;
    Driver driver2;
};

static void SetStatus(TeamStatus *status, enum teamStatus wanted_status);

Team TeamCreate(TeamStatus* status, char* name){
    // Check input
    if(!name) {
        SetStatus(status, TEAM_NULL_PTR);
        return NULL;
    }

    // Allocate memory
    Team team = malloc(sizeof(*team));
    if(!team){
        SetStatus(status, TEAM_MEMORY_ERROR);
        return  NULL;
    }
    team->name = malloc(sizeof(char)*(strlen(name)+1));
    if(!team->name){
        SetStatus(status, TEAM_MEMORY_ERROR);
        return  NULL;
    }

    // Initialize fields
    strcpy(team->name, name);
    team->driver1 = NULL;
    team->driver2 = NULL;
    SetStatus(status, TEAM_STATUS_OK);

    return team;
}
void TeamDestroy(Team team){
    if(team) free(team->name);
    free(team);
}
TeamStatus TeamAddDriver(Team team, Driver driver){
    // Check input
    if(!team || !driver) return TEAM_NULL_PTR;

    // Try to add driver
    if(team->driver1 && team->driver2) return TEAM_FULL;
    else if(!team->driver1) team->driver1 = driver;
    else team->driver2 = driver;

    return TEAM_STATUS_OK;
}
const char * TeamGetName(Team  team){
    if(!team) return NULL;
    return team->name;
}
Driver TeamGetDriver(Team team, DriverNumber driver_number){
    if(driver_number == FIRST_DRIVER && team) return team->driver1;
    else if(driver_number == SECOND_DRIVER && team) return team->driver2;
    else return NULL;
}
int TeamGetPoints(Team team, TeamStatus *status){
    // Check input
    if(!team == NULL) {
        SetStatus(status, TEAM_NULL_PTR);
        return 0;
    }

    // Return points
    SetStatus(status, TEAM_STATUS_OK);
    return DriverGetPoints(team->driver1, NULL) +
            DriverGetPoints(team->driver2, NULL);
}

static void SetStatus(TeamStatus *status, enum teamStatus wanted_status) {
    if(status) *status = wanted_status;
}
