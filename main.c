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
/*
   // printf("year:%d\n",GetYear(season));
    Team *teams = SeasonGetTeamsStandings(season);
    int i=0;
    printf("team num:%d\n",SeasonGetNumberOfTeams(season));
    while(i < SeasonGetNumberOfTeams(season))
    {
        printf("name:%s\ndriver1:%s\ndriver2:%s\n",TeamGetName(teams[i]),
        DriverGetName(TeamGetDriver(teams[i],FIRST_DRIVER)),
               DriverGetName(TeamGetDriver(teams[i],SECOND_DRIVER)));
        i++;
        printf("%d\n",i);
    }

    Driver * drivers = SeasonGetDriversStandings(season);
    i=0;
    printf("drivers num:%d\n",SeasonGetNumberOfDrivers(season));
    while(i < SeasonGetNumberOfDrivers(season))
    {
        DriverStatus driver_status;
        /*printf("name:%s\nId:%d\nteam:%s\npoints:%d\nseason:%d\n",
               DriverGetName(drivers[i]), DriverGetId(drivers[i]),
               TeamGetName(DriverGetTeam(drivers[i])),
               //DriverGetPoints(drivers[i], &driver_status),GetYear(DriverGetSeason(drivers[i])));
        i++;
        printf("%d\n",i);
    }
*/
    printf("exit loop\n");

    int race_results[7] = {7, 1,3,2,4,5,6};

    //SeasonAddRaceResult(season, race_results);
    printf("finish\n");
    DriverDestroy(Lewis);
    TeamDestroy(Mercedes);
    printf("destroyed\n");
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
