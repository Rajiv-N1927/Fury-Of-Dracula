// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
// #include "Map.h" ... if you decide to use the Map ADT
     
struct dracView {
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int hello;
};

// Location and round where/when trap was set
// To be stored in traps[]
typedef struct trap {
  Round tRound;
  LocationID tLoc;      // DOES THIS BELONG IN GAMEVIEW?
} Trap;


// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    DracView dracView = malloc(sizeof(struct dracView));
    dracView->hello = 42;
    return dracView;
}
     
     
// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    free( toBeDeleted );
}

//// Trap functions

// Initalises the array of traps a
Trap initTraps(void)
{
  int i;

  // Not sure if this line is necessary
  Trap traps[TRAIL_SIZE] = malloc(TRAIL_SIZE*sizeof(struct trap));

  for(i = 0; i < TRAIL_SIZE; i++)
  {
    traps[i].tRound = 0;
    traps[i].tLoc = -1;
  }
  return traps;
}

// Sets a trap - stores location and round when set
// then adds the new trap to traps[]
// This functions assumes that Dracula's position is updated before
// this function is called, noting that a trap is set as he ENTERS a city
Trap setTrap(Round roundNo, Trap traps[TRAIL_SIZE])
{
  int i;

  for(i = 0; i < TRAIL_SIZE; i++)
  {
    if(traps[i] == 0)
    {
      traps[i].tRound = roundNo;
      traps[i].tLoc = // curPos[PLAYER_DRACULA];
      return traps; 
    }   
  }
  return traps;
}

// Checks if a trap falls off the trail.
// Should be called at the start of every round.
Trap updateTraps(Gameview currentView, Trap traps[TRAIL_SIZE])
{
  int i;

  for(i = 0; i < TRAIL_SIZE; i++)
  {
    if(traps[i].tRound + TRAIL_SIZE <= roundNo)
    {
      traps[i].tRound = 0;
      traps[i].tLoc = -1;
    }
  }  
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return 0;
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return 0;
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return 0;
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return 0;
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
                 LocationID *start, LocationID *end)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}