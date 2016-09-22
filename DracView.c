// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Map.h"
#include <string.h>

// #include "Map.h" ... if you decide to use the Map ADT

#define IMM_VAMP 0
#define SET_TRAP 1
#define NOSET    2

#define TURN_SIZE 8

#define GAME_IN_PROGRESS 0
#define HUNTER_WIN 1
#define DRACULA_WIN -1
#define DRAW 100

typedef struct encounter {
  int type;
  LocationID tLoc;      
} Encounter;


struct dracView {
  //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
  GameView newGV;
  Encounter encs[TRAIL_SIZE];
};
// Location and round where/when trap was set
// To be stored in traps[]
void initEncounters(Encounter encs[TRAIL_SIZE]);
void setEnc(DracView currentView, Encounter encs[TRAIL_SIZE], int type, LocationID curLoc);
int CheckUniqueLoc ( LocationID *arr, LocationID lID );
void updateEncs(DracView currentView, Encounter encs[TRAIL_SIZE]);
void rmTrpNVmp(DracView currentView, PlayerID player, Encounter encs[TRAIL_SIZE], LocationID curLoc);
void rmMature(DracView currentView);
int isUniqueLoc ( LocationID *arr, LocationID lID );

//// Trap functions

// Initalises the array of traps/vampires
void initEncounters(Encounter encs[TRAIL_SIZE])
{
  int i;
  for(i = 0; i < TRAIL_SIZE; i++)
  {
    encs[i].type = NOSET;
    encs[i].tLoc = UNKNOWN_LOCATION;
  }
}
// Remove a trap due to a player stepping on it or killing imm vamp
void rmTrpNVmp(DracView currentView, PlayerID player, Encounter encs[TRAIL_SIZE], LocationID curLoc) {
  int i;
  int trapsHit; 
  for ( i = 0; i < TRAIL_SIZE; i++ ) {
      if ( curLoc == currentView->encs[i].tLoc ) {
        if ( currentView->encs[i].type == SET_TRAP && trapsHit <= 3 )
          currentView->encs[i].type = NOSET;
          currentView->encs[i].tLoc = UNKNOWN_LOCATION; 
          trapsHit++;
        if ( currentView->encs[i].type == IMM_VAMP )
          currentView->encs[i].type = NOSET;
          currentView->encs[i].tLoc = UNKNOWN_LOCATION; 
     }
    }
}
void rmMature(DracView currentView) {
  int i;
  for ( i = 0; i < TRAIL_SIZE; i++ ) {
    if ( currentView->encs[i].type == IMM_VAMP ) {
      currentView->encs[i].type = NOSET;
      currentView->encs[i].tLoc = UNKNOWN_LOCATION; 
    }
  }
}
// Sets a trap/vampire - stores location and round when set
// then adds the new encounter to traps[]/vamps[]
// This functions assumes that Dracula's position is updated before
// this function is called, noting that a trap/vampire is set as he ENTERS a city
void setEnc(DracView currentView, Encounter encs[TRAIL_SIZE], int type, LocationID curLoc)
{
  updateEncs(currentView, encs);
  currentView->encs[0].type = type;
  currentView->encs[0].tLoc = curLoc;
}

// Checks if a trap falls off the trail/vampire matures.
// Should be called at the start of every round.
void updateEncs(DracView currentView, Encounter encs[TRAIL_SIZE])
{
  int i;
  Encounter refTrail[TRAIL_SIZE];
  for ( i = 0; i < 5; i++ ) {
    refTrail[i+1] = currentView->encs[i];
  }
  refTrail[0].type = NOSET;
  refTrail[0].tLoc = UNKNOWN_LOCATION;
  for ( i = 0; i < TRAIL_SIZE; i++ ) {
    currentView->encs[i] = refTrail[i];
  }
}


// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    DracView currentView = malloc(sizeof(struct dracView));
    currentView->newGV = newGameView(pastPlays, messages);
    initEncounters(currentView->encs);

    // THE BELOW WAS TAKEN FROM GAMEVIEW AND HAS TO BE TWEAKED FOR DRACVIEW
    //I guess these loops can just be put in dracview
    int turnIndex = 0;
    int actionIndex = 0;
    int actionLoop = 0;
    // int gameStatus = GAME_IN_PROGRESS;

    while (turnIndex < strlen(pastPlays)) {

      if (pastPlays[0] == '\0') { 

        return currentView; 

      }

      updateEncs(currentView, currentView->encs);

      PlayerID currentPlayer;

      switch(pastPlays[turnIndex]) {

          case 'G': currentPlayer = PLAYER_LORD_GODALMING;
            break;
          case 'S': currentPlayer = PLAYER_DR_SEWARD;
            break;
          case 'H': currentPlayer = PLAYER_VAN_HELSING;
            break;
          case 'M': currentPlayer = PLAYER_MINA_HARKER;
            break;
          case 'D': currentPlayer = PLAYER_DRACULA;
            break;

        }

        actionIndex = turnIndex + 1; // first location char

        char turnAbbrevLocation[3] = {pastPlays[actionIndex], pastPlays[actionIndex+1], '\0'};

        LocationID turnLocID; 

        turnLocID = abbrevToID(turnAbbrevLocation);

        actionIndex += 2; 
 
        actionLoop = 0; 

        if (currentPlayer == PLAYER_DRACULA) {
                
                while (actionLoop < 2) { // placement phase i.e. if trap or vamp was placed

                    if (pastPlays[actionIndex] == 'V') { // vamp placed
                      setEnc(currentView, currentView->encs, IMM_VAMP, turnLocID);
                    }

                    if (pastPlays[actionIndex] == 'T') { // trap placed
                      setEnc(currentView, currentView->encs, SET_TRAP, turnLocID);
                    }

                    actionIndex++;
                    actionLoop++;

                }

                actionLoop = 0; // resetting for his 'action' phase

               while (actionLoop < 2) { // loops through actions and accounts for effects (only 4 actions per string)

                    if (pastPlays[actionIndex] == 'V') { // vamp matured
                      rmMature(currentView);
                      
                    }

                    if (pastPlays[actionIndex] == 'T') { // trap vanished
                      rmTrpNVmp(currentView, currentPlayer,
                        currentView->encs, turnLocID);
                    }

                    actionIndex++;
                    actionLoop++;
              }
        } else { // hunters 

          while (actionLoop < 4) { // placement phase i.e. if trap or vamp was placed
              if (pastPlays[actionIndex] == 'V') { // vamp placed
                rmTrpNVmp(currentView, currentPlayer,
                  currentView->encs, turnLocID);
              } if (pastPlays[actionIndex] == 'T') { // trap placed
                rmTrpNVmp(currentView, currentPlayer,
                  currentView->encs, turnLocID);
              }
              actionIndex++;
              actionLoop++;
          }

        }

      turnIndex += TURN_SIZE;

    }

    return currentView;

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
  if ( player == PLAYER_DRACULA ) {
    LocationID ret[TRAIL_SIZE];
    getHistory(currentView->newGV, PLAYER_DRACULA, ret);
    if ( curLoc >= DOUBLE_BACK_1 && curLoc <= DOUBLE_BACK_5) {
      int dist = 5 - (DOUBLE_BACK_5 - curLoc);
      curLoc = ret[dist];
    }
    else if ( curLoc == HIDE ) curLoc = ret[1];
    else if ( curLoc == TELEPORT ) curLoc = CASTLE_DRACULA;
  }
  return curLoc;
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
                 LocationID *start, LocationID *end)
{
  LocationID *trail = NULL;
  getHistory(currentView->newGV, player, trail);
  *start = trail[0];
  *end = trail[1];
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps)
{
    int traps = 0;
    int vamps = 0;
    int i;
    for ( i = 0; i < TRAIL_SIZE; i++ ) {
      if ( currentView->encs[i].tLoc == where) { 
        if ( currentView->encs[i].type == IMM_VAMP ) vamps++;
        if ( currentView->encs[i].type == SET_TRAP ) traps++;
      }
    }
    *numTraps = vamps;
    *numVamps = traps;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
  getHistory(currentView->newGV, player, trail);
}

int isUniqueLoc ( LocationID *arr, LocationID lID ) {
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
      if ( isUniqueLoc(trailcheck, toCheck[i])
        && toCheck[i] != ST_JOSEPH_AND_ST_MARYS ) {
        ret[index] = toCheck[i];
        index++;
      }
    } else {
      //Latest move is a DOUBLE BACK move
      if ( (toCheck[0] >= DOUBLE_BACK_1 && toCheck[0] <= DOUBLE_BACK_5) ) {
        int dist = 5 - (DOUBLE_BACK_5 - toCheck[0]);
        if ( DB == FALSE ) {
          ret[index] = trailcheck[dist];
          index++;
        }
      } if ( toCheck[0] == HIDE ) {
          //Make sure its not a sea
        if ( idToType(trailcheck[1]) != SEA  && HD == FALSE ) {
          ret[index] = trailcheck[1];
          index++;
        }
      }
    }
  }
  *numLocations = index + 1;
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
    where = connectedLocations(currentView->newGV, numLocations,
      whereIs(currentView, player), player, giveMeTheRound(currentView),
        road, rail, sea);
  }
  return where;
}
