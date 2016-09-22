// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Queue.h"

#include "Map.h"

#define MAX_TRAPS 3
// #include "Map.h" ... if you decide to use the Map ADT
//The Trap struct
typedef struct trap {
  LocationID tID;
  Round tRound;
} Trap;
//The Player struct
typedef struct player {
  PlayerID playerID;  //Establish an ID for the player
  LocationID curPos;  //The current position of the Player
  LocationID trail[TRAIL_SIZE];  //The trail
  int health;         //The current health of the player
} Player;
//The GameViewer struct
struct gameView {
    Player player[NUM_PLAYERS];
    Trap traps[MAX_TRAPS];
    int score;
    int turnNo;
    Round roundNo;
    Map newMap;
};
//Function initialisers
//Initialise all the players
static void initPlayers(Player p[NUM_PLAYERS]) {
  int i, j;
  for ( i = 0; i < NUM_PLAYERS; i++ ) {
    p[i].playerID = i;
    p[i].curPos   = UNKNOWN_LOCATION;
    p[i].health   = (i == PLAYER_DRACULA ? GAME_START_BLOOD_POINTS
                    : GAME_START_HUNTER_LIFE_POINTS);
    for ( j = 0; j < TRAIL_SIZE; j++ ) {
      p[i].trail[j] = UNKNOWN_LOCATION;
    }
  }
}
static void initTraps(Trap t[MAX_TRAPS]) {
  int i;
  for ( i = 0; i < MAX_TRAPS; i++ ) {
    t[i].tID = UNKNOWN_LOCATION;
    t[i].tRound = -1;
  }
}
//Initialise the gameView struct
static GameView initGV() {
  GameView gameView = (GameView)malloc(sizeof(struct gameView));
  initPlayers(gameView->player);
  initTraps(gameView->traps);
  gameView->roundNo = 0;
  gameView->turnNo  = 0;
  gameView->score   = GAME_START_SCORE;
  gameView->newMap  = newMap();

  return gameView;
}
//How this would be used setTrail(currentView, )
void setTrail(GameView currentView, PlayerID pID, LocationID locID) {
  int i;
  LocationID refTrail[TRAIL_SIZE];
  for ( i = 0; i < 5; i++ ) {
    refTrail[i+1] = currentView->player[pID].trail[i];
  }
  refTrail[0] = locID;
  for ( i = 0; i < TRAIL_SIZE; i++ ) {
    currentView->player[pID].trail[i] = refTrail[i];
  }
}
//Converts the player char to ID
PlayerID charToPlayerID(char p) {
  PlayerID thePlayer;
  switch(p) {
    case 'G': thePlayer = PLAYER_LORD_GODALMING;
      break;
    case 'S': thePlayer = PLAYER_DR_SEWARD;
      break;
    case 'H': thePlayer = PLAYER_VAN_HELSING;
      break;
    case 'M': thePlayer = PLAYER_MINA_HARKER;
      break;
    case 'D': thePlayer = PLAYER_DRACULA;
      break;
  }
  return thePlayer;
}
//For the trap adts

int isTrapExpired(GameView currView) {
  int i = 0;
  Trap *t = currView->traps;
  for (; t[i].tID != -1; i++ ) {
    if ( t[i].tRound + 6 <= currView->roundNo ) {
      return TRUE;
    }
  }
  return FALSE;
}
int hasStepped(GameView currView, PlayerID pID) {
  return FALSE;
}

int getTrapLoc(Trap t) {
  return t.tID;
}

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView gameView = initGV();

    return gameView;
}


// Frees all memory previously allocated for the GameView toBeDeleted
void freeTrail(Player p[NUM_PLAYERS]) {
  int i;
  for ( i = 0; i < NUM_PLAYERS; i++ ) {
    free(p[i].trail);
  }
}
void disposeGameView(GameView toBeDeleted)
{
    //Need this checked.
    //freeTrail( toBeDeleted->player );
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
  return currentView->roundNo;
}
// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
  return currentView->player[(currentView->turnNo%5)+1].playerID;
}

// Get the current score
int getScore(GameView currentView)
{
  return currentView->score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    return currentView->player[player].health;
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    return currentView->player[player].curPos;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    int i;
    for ( i = 0; i < TRAIL_SIZE; i++ ) {
      trail[i] = currentView->player[player].trail[i];
    }
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations
int CheckUniqueLoc ( LocationID *arr, LocationID lID ) {
  int i;
  for ( i = 0; arr[i] != '\0'; i++ ) {
    if ( arr[i] == lID ) return FALSE;
  }
  return TRUE;
}
void connectedRail(Map g, LocationID lID, LocationID *locs,
  int *index, int maxDist)
  {
  VList start;
  //printf("CHECKING: %s\n", idToName(lID));
  if ( maxDist == 0 || *index == maxDist ) return;
  for (start = g->connections[lID]; start != NULL; start = start->next ) {
    if ( *index < maxDist ) {
      if ( start->type == RAIL ) {
        printf("%s\n", idToName(start->v));
        if ( CheckUniqueLoc( locs, start->v ) ) {
          //printf("IndexCRRA %d: %s\n", *index, idToName(start->v));
          locs[*index+=1] = start->v;
          connectedRail(g, start->v, locs, index, maxDist);
        }
      }
    }
  }
  //printf("-------------------\n");
  //printf("%d\n", index);
}
LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
  LocationID *arr = malloc(sizeof(LocationID)*NUM_MAP_LOCATIONS);
  int test = 0;
  int *index = &test;
  arr[*index] = from;
  VList start = currentView->newMap->connections[from];
  for (; start != NULL; start = start->next) {
    if ( sea == 1 && start->type == BOAT ) {
      if ( CheckUniqueLoc(arr, start->v) ) {
        //printf("IndexCRB %d: %s\n", *index, idToName(start->v));
        arr[*index+=1] = start->v;
      }
    } if ( player != PLAYER_DRACULA && rail == 1 && start->type == RAIL ) {
        //printf("Curr index: %d maxDist %d\n", *index, (round+player)%4 + *index);
        connectedRail(currentView->newMap, from, arr, index, (round+player)%4 + *index);
        //rail = 0;
    } if ( road == 1 && start->type == ROAD ) {
      if ( CheckUniqueLoc(arr, start->v) ) {
        //printf("IndexCRRO %d: %s\n", *index, idToName(start->v));
        arr[*index+=1] = start->v;
      }
    }
  }
  //printf("index: %d\n", *index);
  arr[*index+=1] = -1;
  *numLocations = test;
  return arr;
}
