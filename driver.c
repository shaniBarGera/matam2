//
// Created by shani on 01/05/2018.
//

#include "driver.h"
#include "team.h"
#include "season.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct driver {
    int Id;
    char* name;
    Team team;
    int points;
    Season season;
};

void printDriver(Driver driver){
    printf("id: %d\n%s\nplays for team %s\npoints: %d\n\n", driver->Id, driver->name,
           TeamGetName(driver->team), driver-> points);
}

static void DriverSetStatus(DriverStatus *status, enum driverStatus wanted_status);

Driver DriverCreate(DriverStatus* status, char* driver_name, int driverId) {
    if(driverId <= 0 || driver_name == NULL) {
        DriverSetStatus(status, DRIVER_MEMORY_ERROR);
        return NULL;
    }
    Driver driver = malloc(sizeof(*driver));
    if(driver == NULL) {
        DriverSetStatus(status, DRIVER_MEMORY_ERROR);
        return NULL;
    }

    else {
        driver->Id = driverId;
        driver->name = driver_name;
        driver->points = 0;
        driver->season = NULL;
        driver->team = NULL;
        DriverSetStatus(status, DRIVER_STATUS_OK);
    }
    return driver;
}
void DriverDestroy(Driver driver){
    free(driver);
}
const char* DriverGetName(Driver driver){
    if(driver == NULL) return NULL;
    return driver->name;
}
int DriverGetId(Driver driver){
    if(driver ==  NULL) return 0;
    return driver->Id;
}
Team  DriverGetTeam(Driver driver){
    if(driver == NULL) return NULL;
    return driver->team;
}
void  DriverSetTeam(Driver driver, Team team){
    if(driver != NULL) driver->team = team;
}
void  DriverSetSeason(Driver driver, Season season){
    if(driver != NULL){
        driver->season = season;
        driver->points = 0;
    }
}
Season DriverGetSeason(Driver driver){
    return driver->season;}
DriverStatus DriverAddRaceResult(Driver driver, int position){
    if(position <= 0) return INVALID_POSITION;
    if(driver == NULL) return INVALID_DRIVER;
    if(driver->season == NULL) return SEASON_NOT_ASSIGNED;
    driver->points += SeasonGetNumberOfDrivers(driver->season) - position;
    return DRIVER_STATUS_OK;
}
int DriverGetPoints(Driver driver, DriverStatus* status) {
    if(driver == NULL) {
        DriverSetStatus(status, INVALID_DRIVER);
        return 0;
    }

    else DriverSetStatus(status, DRIVER_STATUS_OK);
    return driver->points;
}

static void DriverSetStatus(DriverStatus *status, enum driverStatus wanted_status) {
    if (status != NULL) *status = wanted_status;
}
