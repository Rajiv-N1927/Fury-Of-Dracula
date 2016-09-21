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
typedef struct encounter {
  Round tRound;
  LocationID tLoc;      // DOES THIS BELONG IN GAMEVIEW?
} Encounter;

//// Trap functions

// Initalises the array of traps/vampires
Encounter initEncounters(void)
{
  int i;

  // Not sure if this line is necessary
  Encounter encs[TRAIL_SIZE] = malloc(TRAIL_SIZE*sizeof(struct encounter));

  for(i = 0; i < TRAIL_SIZE; i++)
  {
    encs[i].tRound = 0;
    encs[i].tLoc = -1;
  }
  return encs;
}


// Sets a trap/vampire - stores location and round when set
// then adds the new encounter to traps[]/vamps[]
// This functions assumes that Dracula's position is updated before
// this function is called, noting that a trap/vampire is set as he ENTERS a city
Trap setEnc(Round roundNo, Trap encs[TRAIL_SIZE])
{
  int i;

  for(i = 0; i < TRAIL_SIZE; i++)
  {
    if(encs[i] == 0)
    {
      encsi].tRound = roundNo;
      encs[i].tLoc = // curPos[PLAYER_DRACULA];
      return encs; 
    }   
  }
  return encs;
}

// Checks if a trap falls off the trail/vampire matures.
// Should be called at the start of every round.
Trap updateTraps(Gameview currentView, Trap encs[TRAIL_SIZE])
{
  int i;

  for(i = 0; i < TRAIL_SIZE; i++)
  {
    if(encs[i].tRound + TRAIL_SIZE <= roundNo)
    {
      encs[i].tRound = 0;
      encs[i].tLoc = -1;
    }
  }  
}


// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    DracView dracView = malloc(sizeof(struct dracView));
    dracView->hello = 42;

    Encounter traps[TRAIL_SIZE] = initEncounters();
    Encounter vamps[TRAIL_SIZE] = initEncounters();
    
    // THE BELOW WAS TAKEN FROM GAMEVIEW AND HAS TO BE TWEAKED FOR DRACVIEW 
     /* I guess these loops can just be put in dracview

        if (currentPlayer == PLAYER_DRACULA) { 

                while (actionLoop < 2) { // placement phase i.e. if trap or vamp was placed

                    if (pastPlays[actionIndex] == 'V') { // vamp placed

                       // updateVampire(turnAbbrevLocation, roundNo, actionInt)
                            // need to place an immature vampire here 
                            // should be something laong the lines of...
                            // updateVampire(turnAbbrevLocation, roundNo, actionVariable)
                            // action variable can be defined to be the placing or vanquishing or maturing of a vampire
                            // we could DEFINE VAMPIRE_PLACE, VAMPIRE_VANQUISH & VAMPIRE_MATURE to determine that 

                    }


                    if (pastPlays[actionIndex] == 'T') { // trap placed

                        // updateTraps(turnAbbrevLocation, roundNo, actionInt) 
                                                        // something like... 
                            // updateTraps(turnAbbrevLocation, roundNo, actionInt) // actionInt will tell if added or disarmed
                            // in this case an actionInt of 1 could mean adding a trap here
                            // we could DEFINE TRAP_ADD & TRAP_DISARM & TRAP_VANISH to determine that 

                    }

                    actionIndex++;
                    actionLoop++;
 
                }

                actionLoop = 0; // resetting for his 'action' phase

               while (actionLoop < 2 && gameStatus == GAME_IN_PROGRESS) { // loops through actions and accounts for effects (only 4 actions per string)

                    if (pastPlays[actionIndex] == 'V') { // vamp matured

                        // updateVampire(turnAbbrevLocation, roundNo, actionInt)
                            // action int should signify the maturing of a vampire 
                            // input location here should not matter 
                            // based on the round No input, the function should search for the respective vampire and remove it from the array 
                        gameView->score -= SCORE_LOSS_VAMPIRE_MATURES; 


                    }


                    if (pastPlays[actionIndex] == 'T') { // trap vanished

                        // updateTraps(turnAbbrevLocation, roundNo, actionInt)
                            // input location here should not matter  
                            // based on the action int, the function should knwo to merely search its array and remove any outdated traps

                    }

                    actionIndex++;
                    actionLoop++;


                    if (gameView->score <= 0) { 

                        gameStatus = DRACULA_WIN; 

                    }
 
              }  



        } */ 
        
    return dracView;
}
     
     
// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    free( toBeDeleted );
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
