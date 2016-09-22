// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h"
// #include "Map.h" ... if you decide to use the Map ADT

struct hunterView {
    GameView newGV;
};


// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    HunterView hunterView = malloc(sizeof(struct hunterView));
    hunterView->newGV = newGameView(pastPlays, messages);
    return hunterView;
}


// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    free(toBeDeleted->newGV);
    free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(HunterView currentView)
{
    return getRound(currentView->newGV);
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
    return getCurrentPlayer(currentView->newGV);
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    return getScore(currentView->newGV);
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
    return getHealth(currentView->newGV, player);
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return getLocation(currentView->newGV, player);
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    getHistory(currentView->newGV, player, trail);
}

//Helper function for whereCanIgo (just a base)
//Get the List size
// int ListSize(VNode list, int road, int rail, int sea ) {
//   int i; VNode cur;
//   for (cur = list, i = 0; cur != NULL; cur = cur->next ) {
//     if ( sea && inVList(list, cur->v, BOAT) ) i++;
//     if ( rail && inVList(list, cur->v, RAIL) ) i++;
//     if ( road && inVList(list, cur->v, ROAD) ) i++;
//   }
//   return i;
// }
//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                        int road, int rail, int sea)
{
  PlayerID whome = whoAmI(currentView);
  return connectedLocations(currentView->newGV, numLocations,
    whereIs(currentView, whome), whome, giveMeTheRound(currentView),
      road, rail, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
  PlayerID whome = whoAmI(currentView)-1;
  printf("PLAYERID: %d\n", whome);
  LocationID loc = whereIs(currentView, whome);
  printf("%d %s\n",whome, idToName(loc));
  return connectedLocations(currentView->newGV, numLocations,
    loc, whome, giveMeTheRound(currentView),
      road, rail, sea);
}
