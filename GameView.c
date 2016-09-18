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
  LocationID *trail;  //The trail
  int health;         //The current health of the player
} Player;
struct gameView {
    Player player[NUM_PLAYERS];
    int score;
    int turnNo;
    Round roundNo;
    Map newMap;
};
static Player *setPlayers(Player p[NUM_PLAYERS]) {
  int i;
  for ( i = 0; i < NUM_PLAYERS; i++ ) {
    p[i].playerID = i;
    p[i].curPos   = NOWHERE;
    p[i].trail    = NULL;
    p[i].health   = (i == PLAYER_DRACULA ? GAME_START_BLOOD_POINTS
                    : GAME_START_HUNTER_LIFE_POINTS);
  }
  return p;
}

static GameView init() {
  GameView gameView = (GameView)malloc(sizeof(struct gameView));
  setPlayers(gameView->player);
  gameView->roundNo = 0;
  gameView->turnNo  = 0;
  gameView->score   = GAME_START_SCORE;
  gameView->newMap  = newMap();

  return gameView;
}
//
// int main( int argc, char* argv[] ) {
//   GameView newView = init();
//   newView->turnNo = 4;
//   printf("testing: %d playerID: %d\n", newView->turnNo,
//                             newView->player[newView->turnNo].playerID);
//   return 0;
// }

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView gameView = init();
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
    freeTrail( toBeDeleted->player );
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
  return currentView->player[currentView->turnNo--].playerID;
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
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}
