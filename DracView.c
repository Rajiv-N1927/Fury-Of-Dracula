// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Map.h"
// #include "Map.h" ... if you decide to use the Map ADT

#define IMM_VAMP 0
#define SET_TRAP 1
#define NOSET    2

typedef struct encounter {
  int type;
  LocationID tLoc;      // DOES THIS BELONG IN GAMEVIEW?
} Encounter;


struct dracView {
  //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
  GameView newGV;
  Encounter encs[TRAIL_SIZE];
};
// Location and round where/when trap was set
// To be stored in traps[]
void initEncounters(Encounter encs[TRAIL_SIZE]);
void setEnc(DracView currentView, Encounter encs[TRAIL_SIZE], int type);
int CheckUniqueLoc ( LocationID *arr, LocationID lID );
void updateEncs(DracView currentView, Encounter encs[TRAIL_SIZE]);

//// Trap functions

// Initalises the array of traps/vampires
void initEncounters(Encounter encs[TRAIL_SIZE])
{
  int i;
  for(i = 0; i < TRAIL_SIZE; i++)
  {
    encs[i].type = NOSET;
    encs[i].tLoc = -1;
  }
}

// Sets a trap/vampire - stores location and round when set
// then adds the new encounter to traps[]/vamps[]
// This functions assumes that Dracula's position is updated before
// this function is called, noting that a trap/vampire is set as he ENTERS a city
void setEnc(DracView currentView, Encounter encs[TRAIL_SIZE], int type)
{
  updateEncs(currentView, encs);
  currentView->encs[0].type = type;
  currentView->encs[0].tLoc = giveMeTheRound(currentView);
}

// Checks if a trap falls off the trail/vampire matures.
// Should be called at the start of every round.
void updateEncs(DracView currentView, Encounter encs[TRAIL_SIZE])
{
  int i;

  for(i = 0; i < TRAIL_SIZE - 1; i++)
  {
    currentView->encs[i+1] = currentView->encs[i];
    // if(currentView->encs[i].tRound + TRAIL_SIZE <= giveMeTheRound(currentView))
    // {
    //   currentView->encs[i].type = NOSET;
    //   currentView->encs[i].tRound = 0;
    //   currentView->encs[i].tLoc = -1;
    // }
  }
}


// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    DracView dracView = malloc(sizeof(struct dracView));
    initEncounters(dracView->encs);

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
    free(toBeDeleted->newGV);
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{
    return getRound(currentView->newGV);
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    return getScore(currentView->newGV);
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
  return getHealth(currentView->newGV, player);
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
  LocationID curLoc = getLocation(currentView->newGV, player);
  if (curLoc >= HIDE && curLoc <= TELEPORT) {
        LocationID ret[TRAIL_SIZE];
        getHistory(currentView->newGV, PLAYER_DRACULA, ret);
        switch (curLoc) {
            // Hide
            case HIDE:          curLoc = ret[1];
                break;
            //DB 1
            case DOUBLE_BACK_1: curLoc = ret[1];
                break;
            //DB 2
            case DOUBLE_BACK_2: curLoc = ret[2];
                break;
            //DB 3
            case DOUBLE_BACK_3: curLoc = ret[3];
                break;
            //DB 4
            case DOUBLE_BACK_4: curLoc = ret[4];
                break;
            //DB 5
            case DOUBLE_BACK_5: curLoc = ret[5];
              break;
            // Tele Castle Dracula
            case TELEPORT: curLoc = CASTLE_DRACULA;
              break;
        }
    }
    return curLoc;
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
    int traps = 0;
    int vamps = 0;
    int i;
    //for ( i = 0; currentView->encs[i] )
    return;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
  getHistory(currentView->newGV, player, trail);
}

int CheckUniqueLoc ( LocationID *arr, LocationID lID ) {
  int i;
  for ( i = 0; arr[i] != '\0'; i++ ) {
    if ( arr[i] == lID ) return FALSE;
  }
  return TRUE;
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
  LocationID *ret = malloc(sizeof(LocationID)*NUM_MAP_LOCATIONS);
  LocationID trailcheck[TRAIL_SIZE];
  giveMeTheTrail(currentView, PLAYER_DRACULA, trailcheck);

  LocationID *toCheck = connectedLocations(currentView->newGV, numLocations,
    whereIs(currentView, PLAYER_DRACULA), PLAYER_DRACULA,
      giveMeTheRound(currentView), road, 0, sea);

  int i, index = 0;
  int DB = FALSE, HD = FALSE;
  //Check if the most recent move is not a DB or HD
  for (i = 1; i < TRAIL_SIZE; i++) {
    if ( trailcheck[i] == HIDE ) HD = TRUE;
    if ( trailcheck[i] > HIDE && trailcheck[i] < TELEPORT )
      DB = TRUE;
  }
  for (i = 0; toCheck[i] != -1; i++ ) {
    if ( HD && DB ) {
      if ( CheckUniqueLoc(trailcheck, toCheck[i])
        && toCheck[i] != ST_JOSEPH_AND_ST_MARYS ) {
        ret[index] = toCheck[i];
        index++;
      }
    } else {
      //Latest move is a DOUBLE BACK move
      if ( toCheck[0] >= DOUBLE_BACK_1 && toCheck[0] <= DOUBLE_BACK_5 ) {
        int dist = 5 - (DOUBLE_BACK_5 - toCheck[0]);
        ret[index] = trailcheck[dist];
        index++;
      } if ( toCheck[0] == HIDE ) {
          if ( idToType(trailcheck[1]) != SEA ) {
            ret[index] = trailcheck[1];
            index++;
          }
      }
    }
  }
  numLocations = &index + 1;
  return ret;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
  LocationID *where;
  if ( player == PLAYER_DRACULA ) {
    where = whereCanIgo(currentView, numLocations, road, sea);
  } else {
    connectedLocations(currentView->newGV, numLocations,
      whereIs(currentView, player), player, giveMeTheRound(currentView),
        road, rail, sea);
  }
  return where;
}
