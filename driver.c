#include "driver.h"
#include "team.h"
#include "season.h"

#include <stdlib.h>
#include <string.h>

struct driver {
    char* name;
    int id;
    int points;
    Team team;
    Season season;
};

static void SetStatus(DriverStatus *status, enum driverStatus wanted_status);

Driver DriverCreate(DriverStatus* status, char* driver_name, int driverId) {
    // Check input
    if(!driver_name || driverId <= 0) {
        SetStatus(status, INVALID_DRIVER);
        return NULL;
    }

    // Allocate memory to driver and driver's name
    Driver driver = malloc(sizeof(*driver));
    if(!driver) {
        SetStatus(status, DRIVER_MEMORY_ERROR);
        return NULL;
    }
    driver->name = malloc(sizeof(char) * (strlen(driver_name) + 1));
    if(!driver->name) {
        SetStatus(status, DRIVER_MEMORY_ERROR);
    }

    // Initialize driver's fields
    strcpy(driver->name, driver_name);
    driver->id = driverId;
    driver->points = 0;
    driver->team = NULL;
    driver->season = NULL;
    SetStatus(status, DRIVER_STATUS_OK);

    return driver;
}
void DriverDestroy(Driver driver){
    if(driver != NULL) free(driver->name);
    free(driver);
}
const char* DriverGetName(Driver driver){
    if(!driver) return NULL;
    return driver->name;
}
int DriverGetId(Driver driver){
    if(!driver) return 0;
    return driver->id;
}
Team  DriverGetTeam(Driver driver){
    if(!driver) return NULL;
    return driver->team;
}
void  DriverSetTeam(Driver driver, Team team){
    if(driver) driver->team = team;
}
void  DriverSetSeason(Driver driver, Season season){
    if(driver){
        driver->season = season;
        driver->points = 0;
    }
}
DriverStatus DriverAddRaceResult(Driver driver, int position){
    // Check input
    if(position <= 0) return INVALID_POSITION;
    if(!driver) return INVALID_DRIVER;
    if(!driver->season) return SEASON_NOT_ASSIGNED;

    // Add race results
    driver->points += SeasonGetNumberOfDrivers(driver->season) - position;
    return DRIVER_STATUS_OK;
}
int DriverGetPoints(Driver driver, DriverStatus* status) {
    if(!driver) {
        SetStatus(status, INVALID_DRIVER);
        return 0;
    } else SetStatus(status, DRIVER_STATUS_OK);
    return driver->points;
}

static void SetStatus(DriverStatus *status, enum driverStatus wanted_status) {
    if (status) *status = wanted_status;
}
