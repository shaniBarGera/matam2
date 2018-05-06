#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "driver.h"
#include "team.h"
#include "season.h"


void _print_error(int test_num, const char *errorName, int lineNumber) {
    printf("\tTest %d Failed\n", test_num);
    printf("\nError name: %-80s\nError Line: %d.\tIn file: \"%s\"\n\n", errorName, lineNumber - 1, __FILE__);
}

void _print_test_number(int test_number, int line_num) {
    printf("Running test number %2d at line number: %3d.", test_number, line_num);
}

void _print_test_success(int test_number) {
    printf("\tTest %2d passed successfully\n", test_number);
}

void test(bool expression, int line, int *test_number, const char *error_code, int *tests_passed) {
    if (expression)
        _print_error(*test_number, error_code, line);
    else {
        _print_test_success(*test_number);
        *tests_passed += 1;
    }
    *test_number += 1;
    _print_test_number(*test_number, line+1);
}

int DriverUnitTest(int *tests_passed) {
    printf("\n---------------------------------------------------------------------------");
    printf("\n                          Testing Driver's Module:                         ");
    printf("\n---------------------------------------------------------------------------\n");
    int test_number = 1;
    Driver driver;
    DriverStatus ds;
    _print_test_number(test_number, __LINE__);
    driver = DriverCreate(&ds, "Driver_1", -1);
    test(ds == DRIVER_STATUS_OK, __LINE__, &test_number, "DriverCreate returns DRIVER_STATUS_OK on negative DriverID input", tests_passed);
    test( driver != NULL, __LINE__, &test_number, "DriverCreate doesn't return NULL on negative DriverID input", tests_passed);
    DriverDestroy(driver);
    driver = DriverCreate(&ds, "Driver_1", 0);
    test(ds == DRIVER_STATUS_OK, __LINE__, &test_number, "DriverCreate returns DRIVER_STATUS_OK on Zero as DriverID input", tests_passed);
    test( driver != NULL, __LINE__, &test_number, "DriverCreate doesn't return NULL on Zero as DriverID input", tests_passed);
    DriverDestroy(driver);
    driver = DriverCreate(NULL, "Driver_1", 1);
    test( driver == NULL , __LINE__, &test_number, "DriverCreate doesn't allow driverStatus to be NULL", tests_passed);
    DriverDestroy(driver);
    driver = DriverCreate(&ds, "Driver_1", 1);
    test(strcmp(DriverGetName(driver), "Driver_1") != 0, __LINE__, &test_number, "Driver create assigns name incorrectly", tests_passed);
    test(ds != DRIVER_STATUS_OK, __LINE__, &test_number, "Driver create doesn't assign DRIVER_STATUS_OK on success", tests_passed);
    test(DriverGetId(driver) != 1, __LINE__, &test_number, "DriverCreate  assigns id wrong", tests_passed);
    DriverDestroy(driver);
    test(DriverGetName(NULL) != NULL, __LINE__, &test_number, "DriverGetName doesn't return NULL on NULL input", tests_passed);
    test(DriverGetTeam(NULL) != NULL, __LINE__, &test_number, "DriverGetTeam doesn't return NULL on NULL input", tests_passed);
    driver = DriverCreate(&ds, "Driver_1", 1);
    test(DriverGetTeam(NULL) != NULL, __LINE__, &test_number, "DriverGetTeam doesn't return NULL on driver with no team assigned", tests_passed);
    test(DriverGetId(NULL) != 0, __LINE__, &test_number, "DriverGetId doesn't return NULL on NULL driver input.", tests_passed);
    test(DriverGetId(driver) != 1, __LINE__, &test_number, "DriverGetId doesn't return right ID number", tests_passed);
    TeamStatus ts;
    Team team = TeamCreate(&ts, "Team_1");
    DriverSetTeam(driver, team);
    test(DriverGetTeam(driver) != team, __LINE__, &test_number, "DriverSetTeam doesn't assign team correctly", tests_passed);
    test(TeamGetDriver(team, FIRST_DRIVER) != NULL || TeamGetDriver(team, SECOND_DRIVER) != NULL, __LINE__, &test_number, "DriverSetTeam changes the Team", tests_passed);
    SeasonStatus status;
    Season season = SeasonCreate(&status, "2018\nTeam_1\nDriver_1\nDriver_2\n");
    DriverSetSeason(driver, season);
    test(DriverGetPoints(driver, &ds) != 0, __LINE__, &test_number, "DriverSetSeason doesn't initialize points to 0", tests_passed);
    test(ds != DRIVER_STATUS_OK, __LINE__, &test_number, "DriverGetPoints doesn't set driver status to DRIVER_STATUS_OK on success", tests_passed);
    test(DriverGetPoints(NULL, &ds) != 0 || ds != INVALID_DRIVER, __LINE__, &test_number, "DriverGetPoints doesn't return NULL on NULL input(return 0 value/invalid status)", tests_passed);
    test(DriverGetPoints(driver, NULL) != 0, __LINE__, &test_number, "DriverGetPoints doesn't allow NULL status input.", tests_passed);
    test(DriverAddRaceResult(driver, -1) == DRIVER_STATUS_OK, __LINE__, &test_number, "DriverAddRaceResult allows negative position", tests_passed);
    test(DriverAddRaceResult(driver, 0) == DRIVER_STATUS_OK, __LINE__, &test_number, "DriverAddRaceResult allows zero as position", tests_passed);
    Driver driver_3 = DriverCreate(&ds, "Driver_t", 5);
    test(DriverAddRaceResult(driver_3, 1) != SEASON_NOT_ASSIGNED, __LINE__, &test_number, "DriverAddRaceResult allows not assigned season/returns wrong status", tests_passed);
    test(DriverAddRaceResult(NULL, 1) != INVALID_DRIVER, __LINE__, &test_number, "DriverAddRaceResult allows NULL driver/ returns wrong status", tests_passed);
    DriverSetSeason(driver_3, season);
    test(DriverAddRaceResult(driver, 1) != DRIVER_STATUS_OK, __LINE__, &test_number, "DriverAddRaceResult doesn't assign DRIVER_STATUS_OK on success.", tests_passed);
    _print_test_success(test_number);
    *tests_passed += 1;
    free(driver_3);
    free(team);
    free(season);
    return test_number;
}

int TeamUnitTest(int *tests_passed) {
    printf("\n---------------------------------------------------------------------------");
    printf("\n                          Testing Team's Module:                           ");
    printf("\n---------------------------------------------------------------------------\n\n");
    TeamStatus ts = TEAM_STATUS_OK;
    DriverStatus ds = DRIVER_STATUS_OK;
    Team team;
    int test_number = 1;
    _print_test_number(test_number, __LINE__);
    test( TeamCreate(NULL, "team_1") == NULL , __LINE__, &test_number, "TeamCreate doesn't allow NULL status", tests_passed);
    test( TeamCreate(&ts, NULL) != NULL, __LINE__,  &test_number, "Assignment of NULL as team name by TeamCreate function", tests_passed);
    team = TeamCreate(&ts, "Test_team");
    test(strcmp(TeamGetName(team), "Test_team") != 0, __LINE__, &test_number, "Team name assignment is incorrect.", tests_passed);
    test(TeamGetDriver(team, FIRST_DRIVER) != NULL || TeamGetDriver(team, SECOND_DRIVER) != NULL, __LINE__, &test_number, "TeamGetDriver doesn't return NULL on not assigned drivers in team.", tests_passed);
    Driver driver = TeamGetDriver(team, 3);
    test(driver != NULL, __LINE__, &test_number, "TeamGetDriver doesn't check for correct driverNumber input", tests_passed);
    test(TeamGetDriver(NULL, FIRST_DRIVER) != NULL, __LINE__, &test_number, "Team get driver doesn't check for NULL team input.", tests_passed);
    TeamGetPoints(NULL, &ts);
    test(ts != TEAM_NULL_PTR, __LINE__, &test_number, "TeamGetPoints doesn't check for NULL team input.", tests_passed);
    int points = TeamGetPoints(team, &ts);
    test(ts != TEAM_STATUS_OK, __LINE__, &test_number, "TeamGetPoints doesn't Update status to STATUS_OK", tests_passed);
    test(points != 0, __LINE__, &test_number, "Points aren't initialised to be 0 on TeamCreation", tests_passed);
    driver = DriverCreate(&ds, "Michael", 1);
    test(TeamAddDriver(NULL, driver) != TEAM_NULL_PTR, __LINE__, &test_number, "TeamAddDriver doesn't handle NULL team input \\ doesn't return TEAM_NULL_PTR.", tests_passed);
    test(TeamAddDriver(team, NULL) != TEAM_NULL_PTR, __LINE__, &test_number, "TeamAddDriver doesn't handle  NULL driver input \\ doesn't return TEAM_NULL_PTR.", tests_passed);
    test(TeamAddDriver(team, driver) != TEAM_STATUS_OK, __LINE__, &test_number, "TeamAddDriver doesn't Update status to STATUS_OK", tests_passed);
    test(TeamGetDriver(team, FIRST_DRIVER) != driver, __LINE__, &test_number, "TeamAddDriver doesn't add first driver correctly", tests_passed);
    TeamDestroy(team);
    team = TeamCreate(&ts, "Test_team");
    driver = DriverCreate(&ds, "Michael", 1);
    ts = TeamAddDriver(team, driver);
    Driver driver_2 = DriverCreate(&ds, "Driver_2", 2);
    Driver driver_3 = DriverCreate(&ds, "Driver_3", 3);
    TeamAddDriver(team, driver_2);
    test(TeamGetDriver(team, SECOND_DRIVER) != driver_2, __LINE__, &test_number, "TeamAddDriver doesn't add second driver correctly.", tests_passed);
    test(TeamAddDriver(team, driver_3) != TEAM_FULL, __LINE__, &test_number, "TeamAddDriver doesn't return TEAM_FULL status when trying to too much drivers", tests_passed);
    test(TeamGetDriver(team, FIRST_DRIVER) != driver, __LINE__, &test_number, "TeamAddDriver replaces first driver when full.", tests_passed);
    test(TeamGetDriver(team, SECOND_DRIVER) != driver_2, __LINE__, &test_number, "TeamAddDriver replaces second driver when full.", tests_passed);
    _print_test_success(test_number);
    *tests_passed += 1;
    TeamDestroy(team);
    DriverDestroy(driver_3);
    return test_number;
}

int SeasonUnitTest(int *tests_passed) {
    printf("\n---------------------------------------------------------------------------");
    printf("\n                          Testing Season's Module:                         ");
    printf("\n---------------------------------------------------------------------------\n\n");
    int test_number = 1;
    _print_test_number(test_number, __LINE__);
    test(SeasonCreate(NULL, "2018\nTeam_1\nDriver_1\nDriver_2\n") == NULL, __LINE__, &test_number, "SeasonCreate doesn't allow NULL status input.", tests_passed);
    SeasonStatus status;
    Season season = SeasonCreate(&status, "2018\nTeam_1\nDriver_1\nDriver_2\n");
    test(status != SEASON_OK, __LINE__, &test_number, "SeasonCreate doesn't assign SEASON_OK on success", tests_passed);
    Team team = SeasonGetTeamByPosition(season, 1, &status);
    Driver driver1 = SeasonGetDriverByPosition(season, 1, &status);
    Driver driver2 = SeasonGetDriverByPosition(season, 2, &status);
    test( driver1 == NULL, __LINE__, &test_number, "SeasonCreate didn't assign first driver correctly", tests_passed);
    test( driver2 == NULL, __LINE__, &test_number, "SeasonCreate didn't assign second driver correctly", tests_passed);
    test( team == NULL, __LINE__, &test_number, "SeasonCreate didn't assign Team correctly", tests_passed);
    test( SeasonGetNumberOfTeams(season) != 1, __LINE__, &test_number, "SeasonCreate didn't assign number of teams correctly", tests_passed);
    test( SeasonGetNumberOfDrivers(season) != 2, __LINE__, &test_number, "SeasonCreate didn't assign number of teams correctly", tests_passed);
    Season season2 = SeasonCreate(&status, "2018\nTeam_1\nDriver_1\nNone\n");
    test( SeasonGetNumberOfDrivers(season2) != 1, __LINE__, &test_number, "SeasonCreate assigned driver with \"None\" name", tests_passed);
    test( SeasonGetDriverByPosition(NULL, 1, &status) != NULL,  __LINE__, &test_number, "SeasonGetDriverByPosition doesn't return NULL on NULL season input", tests_passed);
    test( SeasonGetDriverByPosition(season, 1, NULL) == NULL,  __LINE__, &test_number, "SeasonGetDriverByPosition doesn't allow NULL status input", tests_passed);
    driver1 = SeasonGetDriverByPosition(season, -1, &status);
    test( status == SEASON_OK ,  __LINE__, &test_number, "SeasonGetDriverByPosition assigns SEASON_OK on negative position input", tests_passed);
    driver1 = SeasonGetDriverByPosition(season, 0, &status);
    test( status == SEASON_OK ,  __LINE__, &test_number, "SeasonGetDriverByPosition assigns SEASON_OK on Zero position input", tests_passed);
    driver1 = SeasonGetDriverByPosition(season, 1, &status);
    test( status != SEASON_OK ,  __LINE__, &test_number, "SeasonGetDriverByPosition doesn't assign SEASON_OK on success", tests_passed);
    test( SeasonGetDriversStandings(NULL) != NULL ,  __LINE__, &test_number, "SeasonGetDriversStandings doesn't return NULL on NULL season input", tests_passed);
    test( SeasonGetTeamsStandings(NULL) != NULL ,  __LINE__, &test_number, "SeasonGetTeamsStandings doesn't return NULL on NULL season input", tests_passed);
    Driver* drivers = SeasonGetDriversStandings(season);
    test( drivers == NULL ,  __LINE__, &test_number, "SeasonGetDriversStandings doesn't return an array of drivers", tests_passed);
    Team* teams = SeasonGetTeamsStandings(season);
    test( teams == NULL ,  __LINE__, &test_number, "SeasonGetTeamsStandings doesn't return an array of teams", tests_passed);
    team = SeasonGetTeamByPosition(NULL, 1, &status);
    test( status != SEASON_NULL_PTR ,  __LINE__, &test_number, "SeasonGetTeamByPosition doesn't assign SEASON_NULL_PTR on NULL season", tests_passed);
    test( SeasonGetTeamByPosition(NULL, 1, &status) != NULL ,  __LINE__, &test_number, "SeasonGetTeamByPosition doesn't return NULL on NULL season input", tests_passed);
    test( SeasonGetTeamByPosition(season, 1, NULL) == NULL ,  __LINE__, &test_number, "SeasonGetTeamByPosition doesn't allow status input", tests_passed);
    team = SeasonGetTeamByPosition(season, 1, &status);
    test( status != SEASON_OK ,  __LINE__, &test_number, "SeasonGetTeamByPosition doesn't return SEASON_OK on success", tests_passed);
    test( SeasonGetTeamByPosition(season, 0, &status) != NULL ,  __LINE__, &test_number, "SeasonGetTeamByPosition doesn't return NULL zero as position", tests_passed);
    test( SeasonGetTeamByPosition(season, -1, &status) != NULL ,  __LINE__, &test_number, "SeasonGetTeamByPosition doesn't return NULL on negative position", tests_passed);
    team = SeasonGetTeamByPosition(season, -1, &status);
    test( status == SEASON_OK ,  __LINE__, &test_number, "SeasonGetTeamByPosition assigns SEASON_OK on negative position", tests_passed);
    team = SeasonGetTeamByPosition(season, 0, &status);
    test( status == SEASON_OK ,  __LINE__, &test_number, "SeasonGetTeamByPosition assigns SEASON_OK on zero position", tests_passed);
    test( SeasonGetNumberOfTeams(NULL) != 0 ,  __LINE__, &test_number, "SeasonGetNumberOfTeams doesn't return 0 on NULL input", tests_passed);
    test( SeasonGetNumberOfDrivers(NULL) != 0 ,  __LINE__, &test_number, "SeasonGetNumberOfDrivers doesn't return 0 on NULL input", tests_passed);
    int result[2] = {1,2};
    test( SeasonAddRaceResult(NULL, result) != SEASON_NULL_PTR ,  __LINE__, &test_number, "SeasonAddRaceResult doesn't return SEASON_NULL_PTR on NULL season input", tests_passed);
    test( SeasonAddRaceResult(season, NULL) != SEASON_NULL_PTR,  __LINE__, &test_number, "SeasonAddRaceResult doesn't return SEASON_NULL_PTR on NULL array input", tests_passed); // Might Cause segmentation fault
    test( SeasonAddRaceResult(season, result) != SEASON_OK ,  __LINE__, &test_number, "SeasonAddRaceResult doesn't return SEASON_OK on success", tests_passed);
    SeasonDestroy(season);
    season = SeasonCreate(&status, "2018\nTeam_1\nDriver_1\nNone\nTeam_2\nDriver_2\nNone\n");
    int result_1[2] = {1,2};
    SeasonAddRaceResult(season, result_1);
    Team team1 = SeasonGetTeamByPosition(season, 1, &status);
    Team team2 = SeasonGetTeamByPosition(season, 2, &status);
    driver1 = SeasonGetDriverByPosition(season, 1, &status);
    driver2 = SeasonGetDriverByPosition(season, 2, &status);
    SeasonAddRaceResult(season, result_1);
    teams = SeasonGetTeamsStandings(season);
    drivers = SeasonGetDriversStandings(season);
    bool correct_driver_update = (driver1 == drivers[0] && driver2 == drivers[1]);
    bool correct_team_update = (team1 == teams[0] && team2 == teams[1]);
    test( !correct_driver_update, __LINE__, &test_number, "Drivers standings are incorrect after simple update.", tests_passed);
    test( !correct_team_update, __LINE__, &test_number, "Teams standings are incorrect after simple update.", tests_passed);
    int result_2[2] = {2, 1};
    SeasonAddRaceResult(season, result_2);
    SeasonAddRaceResult(season, result_2); // That's not a mistake !
    teams = SeasonGetTeamsStandings(season);
    drivers = SeasonGetDriversStandings(season);
    correct_driver_update = (driver1 == drivers[1] && driver2 == drivers[0]);
    correct_team_update = (team1 == teams[1] && team2 == teams[0]);
    test( !correct_driver_update, __LINE__, &test_number, "Drivers standings aren't updated correctly standings according to last race on equal results", tests_passed);
    test( !correct_team_update, __LINE__, &test_number, "Teams standings aren't updated correctly standings according to last race on equal results", tests_passed);
    _print_test_success(test_number);
    *tests_passed += 1;
    SeasonDestroy(season);
    return test_number;
}

int main() {
    /*printf("\nWelcome to the homework 2 tests, written by Vova Parakhin.\n\nPassing those tests won't "
                   "guarantee you a good grade but they might get you close to one "
                   ",make some tests yourself to be sure.\n\n");
    printf("Press any button to start the tests... \n\n");
    //getchar();
    int tests_passed = 0;
    int tests_number = 0;
    tests_number += DriverUnitTest(&tests_passed);
    tests_number += TeamUnitTest(&tests_passed);
    tests_number += SeasonUnitTest(&tests_passed);
    int score = (int) ceil(((double) tests_passed / tests_number) * 100);
    printf("Your grade is: %d\n", score);
    printf("You've passed %d tests out of %d\n", tests_passed, tests_number);
    printf("Enter a char to exit...");
    getchar();*/
    SeasonStatus status;
    Season season = SeasonCreate(&status, "2018\nTeam_1\nDriver_1\nNone\nTeam_2\nDriver_2\nNone\n");
    SeasonPrintDrivers(season);
    SeasonPrintTeams(season);
    int result_1[2] = {1,2};
    SeasonAddRaceResult(season, result_1);
    printf("\nround1\n");
    SeasonPrintDrivers(season);
    SeasonPrintTeams(season);
    printf("\nround2\n");
     SeasonAddRaceResult(season, result_1);
    SeasonPrintDrivers(season);
    SeasonPrintTeams(season);
    int result_2[2] = {2, 1};
    printf("\nround3\n");
    SeasonAddRaceResult(season, result_2);
     SeasonPrintDrivers(season);
    SeasonPrintTeams(season);
    printf("\nround4\n");
    SeasonAddRaceResult(season, result_2);
    SeasonPrintDrivers(season);
    SeasonPrintTeams(season);

}