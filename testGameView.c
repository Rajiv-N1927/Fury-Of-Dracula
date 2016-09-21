// testGameView.c ... test the GameView ADT    took so long omg

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "GameView.h"

int main()
{
    int i;
    GameView gv;

    printf("Test 1 - basic empty initialisation\n");
    PlayerMessage messages1[] = {};
    gv = newGameView("", messages1);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 0);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula trail and basic functions\n");
    PlayerMessage messages2[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GST.... SAO.... HZU.... MBB.... DC?....", messages2);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(getLocation(gv,PLAYER_DRACULA) == CITY_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for encountering Dracula and hunter history\n");
    PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", messages3);
    assert(getLocation(gv,PLAYER_DRACULA) == GENEVA);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 5);
    assert(getHealth(gv,PLAYER_DRACULA) == 30);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == GENEVA);
    LocationID history[TRAIL_SIZE];
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == GENEVA);
    assert(history[1] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == STRASBOURG);
    assert(history[2] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DR_SEWARD,history);
    assert(history[0] == ATLANTIC_OCEAN);
    assert(history[1] == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Hunter View)\n");
    PlayerMessage messages4[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages4);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == SEA_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Drac View)\n");
    PlayerMessage messages5[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DEC.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages5);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == ENGLISH_CHANNEL);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for connections\n");
    int size, seen[NUM_MAP_LOCATIONS], *edges;
    gv = newGameView("", messages1);
    printf("Checking Galatz road connections\n");
    edges = connectedLocations(gv,&size,GALATZ,PLAYER_LORD_GODALMING,0,1,0,0);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 5); assert(seen[GALATZ]); assert(seen[CONSTANTA]);
    assert(seen[BUCHAREST]); assert(seen[KLAUSENBURG]); assert(seen[CASTLE_DRACULA]);
    free(edges);
    printf("Checking Ionian Sea sea connections\n");
    edges = connectedLocations(gv,&size,IONIAN_SEA,PLAYER_LORD_GODALMING,0,0,0,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[IONIAN_SEA]); assert(seen[BLACK_SEA]);
    assert(seen[ADRIATIC_SEA]); assert(seen[TYRRHENIAN_SEA]);
    assert(seen[ATHENS]); assert(seen[VALONA]); assert(seen[SALONICA]);
    free(edges);
    printf("Checking Athens rail connections (none)\n");
    edges = connectedLocations(gv,&size,ATHENS,PLAYER_LORD_GODALMING,0,0,1,0);
    assert(size == 1);
    assert(edges[0] == ATHENS);
    free(edges);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula doubling back to city two position ago (Hunter View)\n");
    PlayerMessage messages6[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DC?T... "
                     "GST.... SST.... HST.... MST.... DC?T..."
                     "GFR.... SFR.... HFR.... MFR.... DD2T...", messages6);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_2);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_2);
    assert(history[1] == CITY_UNKNOWN);
    assert(history[2] == CITY_UNKNOWN);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula doubling back to city two position ago (Drac View)\n");
    PlayerMessage messages7[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DBDT... "
                     "GST.... SMI.... HST.... MST.... DKLT..."
                     "GFR.... SGO.... HFR.... MBU.... DD2T...", messages7);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_2);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_2);
    assert(history[1] == KLAUSENBURG);
    assert(history[2] == BUDAPEST);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula hiding in the city + placing an immature vampirewhen he hides(Hunter View)\n");
    PlayerMessage messages8[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DC?T... "
                     "GST.... SMI.... HST.... MST.... DC?V...", messages8);
    assert(getLocation(gv,PLAYER_DRACULA) == HIDE);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == CITY_UNKNOWN);
    assert(history[1] == CITY_UNKNOWN);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula hiding in the city + placing an immature vampire when he hides (Drac View)\n");
    PlayerMessage messages8[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DHAT... "
                     "GST.... SMI.... HST.... MST.... DHIV...", messages8);
    assert(getLocation(gv,PLAYER_DRACULA) == HIDE);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == HIDE);
    assert(history[1] == HAMBURG);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dr. Seward encountering Dracula and a trap \n");
    PlayerMessage messages9[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GST.... SVE.... HCD.... MAO.... DMIT... GGE.... SMITD..", messages9);
    assert(getLocation(gv,PLAYER_DRACULA) == MILAN);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == 3);
    assert(getHealth(gv,PLAYER_DRACULA) == 30);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == MILAN);
    printf("passed\n", );
    disposeGameView(gv);

    printf("Test for Smiting a vampire with holy stake and van helsing's trial\n");
    PlayerMessage messages10[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GST.... SAO.... HCF.... MAO.... DPAV..."
                     "GGE.... SBB.... HNA.... MEC.... DC?T..."
                     "GMI.... SBO.... HPAV...", messages10);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == PARIS);
    LocationID history[TRAIL_SIZE];
    getHistory(gv,PLAYER_VAN_HELSING,history);
    assert(history[0] == PARIS);
    assert(history[1] == NANTES);
    assert(history[2]==CLERMONT_FERRAND);
    printf("passed\n", );
    disposeGameView(gv);

    printf("Test for Dr. Seward encountering 2 traps (Drac view) + Drac's history\n");
    PlayerMessage messages11[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GST.... SVE.... HCD.... MAO.... DMIT..."
                     "GGE.... SFL.... HCD.... MEC.... DHIT..."    //HI is hide so 2 traps placed
                     "GGE.... SGO.... HGA.... MLO.... DMUT..."
                     "GCF.... SMITT..", messages11);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == 5);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == MILAN);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == MUNICH);
    assert(history[1] == HIDE);
    assert(history[2] == MILAN);
    printf("passed\n", );
    disposeGameView(gv);

    prinf("Test for Hunter history for 2 turns with no encounters occuring\n");
    PlayerMessage messages12[] = {""};
    gv = newGameView("GBS.... SVA.... HJM.... MCD.... DC?T..."
                     "GCN.... SIO.... HZA.... MKL.... DC?T...", mesages12);
    getHistory(gv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == BLACK_SEA);
    assert(history[1] == CONSTANTA);
    getHistory(gv,PLAYER_DR_SEWARD,history);
    assert(history[0] == IONIAN_SEA);
    assert(history[1] == VALONA);
    getHistory(gv,PLAYER_VAN_HELSING,history);
    assert(history[0] == ZAGREB);
    assert(history[1] == ST_JOSEPH_AND_ST_MARYS);
    getHistory(gv,PLAYER_MINA_HARKER,history);
    assert(history[0] == KLAUSENBURG);
    assert(history[1] == CASTLE_DRACULA);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == CITY_UNKNOWN);
    assert(history[1] == CITY_UNKNOWN);

    printf("Test for rail movement\n");
    PlayerMessage messages13[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GMU.... SAO.... HZU.... MBB.... DC?T..."
                     "GLI....", messages13);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 2);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == LEIPZIG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(getLocation(gv,PLAYER_DRACULA) == CITY_UNKNOWN);
    printf("passed\n");
    disposeGameView(gv);

    prinf("Test for resting twice after hitting a triple trap (Hunter View)\n");
    PlayerMessage messages14[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GSTTT.. SVE.... HCD.... MAO.... DC?T..."
                     "GGE.... SFL.... HCD.... MEC.... DC?T..."
                     "GGE.... ", messages14);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 9);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == GENEVA;
    getHistory(gv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == GENEVA);
    assert(history[2] == STRASBOURG);
    printf("passed\n", );
    disposeGameView(gv);

    prinf("Test for teleporting to castle dracula on turn 1 to gain 10 blood points\n"); //both sides know he teleported
    PlayerMessage messages15[] = {""};
    gv = newGameView("GVI.... SZA.... HNS.... MMU.... DTP....", messages15);
    assert(getRound(gv) == 1);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getLocation(gv, PLAYER_DRACULA) == CASTLE_DRACULA);
    printf("passed\n");
    disposeGameView(gv);

    prinf("Test for hunter's view when getting to a city in dracula trial\n");
    PlayerMessage messages16[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GMI.... SMR.... HMI.... MMR.... DC?T..."
                     "GGE.... SGE.... HGE.... MGE.... DCOT..."
                     "GST.... SST.... HST.... MST.... DFRT..."
                     "GNU.... SNU.... HNU.... MNU.... DC?T..."
                     "GFRT...", messages14);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 7);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == FRANKFURT;
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == CITY_UNKNOWN);
    assert(history[1] == FRANKFURT);
    assert(history[2] == COLOGNE);
    assert(history[3] == CITY_UNKNOWN);
    printf("passed\n", );
    disposeGameView(gv);

    printf("Test for Lord Godalming encountering 3 traps (Hunter View)\n");
    PlayerMessage messages17[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DPAT... "
                     "GST.... SMI.... HST.... MST.... DHIT..."
                     "GBU.... SBU.... HBU.... MBU.... DC?T..."
                     "GLE.... SCO.... HCO.... MCO.... DCFT..."
                     "GNA.... SAM.... HAM.... MAM.... DD3T..."
                     "GNA.... SCO.... HBU.... MCO.... DC?T..."
                     "GPATTT.", messages8);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 3);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == PARIS);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == CITY_UNKNOWN);
    assert(history[1] == DOUBLE_BACK_3;
    assert(history[2] == CLERMONT_FERRAND;
    assert(history[3] == CITY_UNKNOWN;
    assert(history[4] == HIDE;
    assert(history[5] == PARIS;
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);













    return 0;
}
