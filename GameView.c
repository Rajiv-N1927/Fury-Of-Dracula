// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"

#include "Map.h"
// #include "Map.h" ... if you decide to use the Map ADT
typedef struct player {
  PlayerID playerID;  //Establish an ID for the player
  LocationID curPos;  //The current position of the Player
  LocationID trail[TRAIL_SIZE];  //The trail
  int health;         //The current health of the player
} Player;

struct gameView {
    Player player[NUM_PLAYERS];
    int score;
    int turnNo;
    Round roundNo;
    Map newMap;
};
//Initialise all the players
static Player *setPlayers(Player p[NUM_PLAYERS]) {
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
  return p;
}
//Initialise the gameView struct
static GameView init() {
  GameView gameView = (GameView)malloc(sizeof(struct gameView));
  setPlayers(gameView->player);
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

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView gameView = init();
    char character = pastPlays[0];

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
//Try to fix this up
LocationID *connectedRail(GameView currView, LocationID lID, Round round) {
  LocationID *railCons = NULL;
  railCons[0] = lID;
  LocationID setLoc = lID;
  VList start;
  int index;
  for ( index = 1, start = currView->newMap->connections[setLoc]; start!= NULL;
    start = start->next ) {
    if ( inVList(currView->newMap->connections[setLoc], start->v, RAIL)
        && CheckUniqueLoc(railCons, start->v) ) {
      railCons[index] = start->v;
      index++;
      setLoc = start->v;
    }
  }
  railCons[index++] = '\0';
  return railCons;
}

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    LocationID *locations = NULL;
    VList head = currentView->newMap->connections[from];
    locations[0] = head->v;
    VList start;
    int locIndex;
    for ( locIndex = 1, start = head; start != NULL; start = start->next ) {
      if ( road && inVList(head, start->v, ROAD) ) {
        if ( CheckUniqueLoc( locations, start->v ) ) {
          locations[locIndex] = start->v;
          locIndex++;
        }
      } if ( sea && inVList(head, start->v, BOAT) ) {
        if ( CheckUniqueLoc( locations, start->v ) ) {
          locations[locIndex] = start->v;
          locIndex++;
        }
      } if ( rail && player != PLAYER_DRACULA ) {
        int k;
        LocationID *toAdd = connectedRail(currentView, from, round);
        for ( k = 0; toAdd[k] != '\0'; k++ ) {
          if ( CheckUniqueLoc(locations, toAdd[k]) )
            locations[locIndex] = toAdd[k];
            locIndex++;
        }
        rail = FALSE;
      }
    }
    numLocations = &locIndex;
    return locations;
}
