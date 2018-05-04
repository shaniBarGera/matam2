#include<stdio.h>
#include<stdlib.h>
#include"driver.h"
#include"team.h"
#include"season.h"
#include<assert.h>
#include<string.h>

void DriverUnitTest(){
    printf("Testing Driver!\n");
    DriverStatus driver_status;
    TeamStatus team_status;
    Driver Lewis = DriverCreate(&driver_status, "Lewis", 44);
    Team Mercedes = TeamCreate(&team_status, "Mercedes");
    DriverSetTeam(Lewis, Mercedes);
    assert(strcmp(DriverGetName(Lewis),"Lewis")==0);
    assert(strcmp(TeamGetName(DriverGetTeam(Lewis)),"Mercedes")==0);

    /*now lets try to create a season
    all drivers/teams created above are not related to the season.*/

    SeasonStatus season_status;
    char* season_info="\
2018\n\
Ferrari\n\
Sebastian Vettel\n\
Kimi Raikonen\n\
Mercedes\n\
Lewis Hamilton\n\
Valtteri Bottas\n\
RedBull Racing\n\
Daniel\n\
Max  Verstappen\n\
McLaren\n\
Fernando Alonso\n\
None\n\
";
    Season season = SeasonCreate(&season_status,season_info);

    int race_results[7] = {7, 1,3,2,4,5,6};

    SeasonAddRaceResult(season, race_results);
    Driver * drivers = SeasonGetDriversStandings(season);

    for(int i=0; i<SeasonGetNumberOfDrivers(season); i++)
    {
        printf("%d.%s\t%d\n",i,
               DriverGetName(drivers[i]), DriverGetPoints(drivers[i],
                                                          &driver_status));
    }
    printf("\n");

    Team * sorted_teams = SeasonGetTeamsStandings(season);

    for(int i=0; i<SeasonGetNumberOfTeams(season); i++)
    {
        printf("%d.%s\t%d\n",i,
               TeamGetName(sorted_teams[i]), TeamGetPoints(sorted_teams[i],
                               &team_status));
    }

    int race_results2[7] = {4,6,5,7,2,3,1};
    printf("\n");
    SeasonAddRaceResult(season, race_results2);
    drivers = SeasonGetDriversStandings(season);

    for(int i=0; i<SeasonGetNumberOfDrivers(season); i++)
    {
        printf("%d.%s\t%d\n",i,
               DriverGetName(SeasonGetDriversStandings(season)[i]),
               DriverGetPoints(SeasonGetDriversStandings(season)[i],
                               &driver_status));
    }

    sorted_teams = SeasonGetTeamsStandings(season);
    printf("\n");
    for(int i=0; i<SeasonGetNumberOfTeams(season); i++)
    {
        printf("%d.%s\t%d\n",i,
               TeamGetName(sorted_teams[i]), TeamGetPoints(sorted_teams[i],
                                                           &team_status));
    }
    free(drivers);
    free(sorted_teams);
    printf("finish\n");
    DriverDestroy(Lewis);
    TeamDestroy(Mercedes);
    //printf("destroyed\n");
    SeasonDestroy(season);

    printf("Finished Testing Driver!\n");
}
void TeamUnitTest(){
}
void SeasonUnitTest(){
}

int main(){
    printf("starting to run the unit tests!\n");
    DriverUnitTest();
    TeamUnitTest();
    SeasonUnitTest();
    return 0;
}
