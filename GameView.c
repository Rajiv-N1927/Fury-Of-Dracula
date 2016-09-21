// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Queue.h"

#include "Map.h"
// #include "Map.h" ... if you decide to use the Map ADT

#define MAX_TRAPS 6

#define TURN_SIZE 8 

#define GAME_IN_PROGRESS 0
#define HUNTER_WIN 1 
#define DRACULA_WIN -1 
#define DRAW 100

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

typedef struct trap {
  Round tRound;
  LocationID tLoc;
} Trap;


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
  Trap traps = initTraps();
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

/* NOW IN DracView.c
// Initalises the array of traps a
Trap initTraps(void)
{
  int i;

  // Not sure if this is necessary
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
Trap setTrap(Gameview currentView, Trap traps[TRAIL_SIZE])
{
  int i;

  for(i = 0; i < TRAIL_SIZE; i++)
  {
    if(traps[i] == 0)
    {
      traps[i].tRound = currentView->roundNo;
      traps[i].tLoc = currentView->player[PLAYER_DRACULA].curPos;
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
*/

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

/// Creates a new GameView to summarise the current state of the game 
// Add this function and the defines at the top 
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{ 
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView gameView = init();

    int turnIndex = 0; 
    int actionIndex = 0; 
    int roundNo = 0; 
    int turnNo = 1; 
    int actionLoop;
    int gameStatus = GAME_IN_PROGRESS; 

    while (pastPlays[turnIndex] != ' ' && gameStatus == GAME_IN_PROGRESS) { // can alternatively have condition while turnIndex < strlen(pastPlays) && gamestatus...

        PlayerID currentPlayer = charToPlayerID(pastPlays[turnIndex]);

        if (currentPlayer == PLAYER_DRACULA) {

            gameView->score -= SCORE_LOSS_DRACULA_TURN;

        } else if (gameView->player[currentPlayer].health <= 0) { // else if hunter who died last round - need to restore hp 

            gameView->player[currentPlayer].health = GAME_START_HUNTER_LIFE_POINTS; 

        }

        actionIndex = turnIndex + 1; // first location char

        char turnAbbrevLocation[3] = {pastPlays[actionIndex], pastPlays[actionIndex+1], '\0'};
        LocationID turnLocID = abbrevToID(turnAbbrevLocation);
        gameView->player[currentPlayer].curPos = turnLocID; 

        // void setTrail(GameView currentView, PlayerID pID, LocationID locID) 
        setTrail(gameView, currentPlayer, turnLocID); // updating trail for current player 

        if (currentPlayer == PLAYER_LORD_GODALMING  || // determining if character rested
            currentPlayer == PLAYER_DR_SEWARD       || 
            currentPlayer == PLAYER_VAN_HELSING     || 
            currentPlayer == PLAYER_MINA_HARKER     ) { 


                    if (gameView->player[currentPlayer].trail[0] == gameView->player[currentPlayer].trail[1]) { // hunter has rested and deserves some hp 

                        gameView->player[currentPlayer].health += LIFE_GAIN_REST; 

                        if (gameView->player[currentPlayer].health > GAME_START_HUNTER_LIFE_POINTS) { // if hunters health has exceeded limited, cap it

                            gameView->player[currentPlayer].health = GAME_START_HUNTER_LIFE_POINTS; 

                        }

                    }

                    // DO WE NEED TO DO THIS ? VVVVV 
                    // This may be for HunterView 

                    /* if (gameView->player[currentPlayer].trail[0] == 'ANY CITY IN DRACULAS TRAIL') { 

                        // all moves that resulted in dracula being in that city 
                        // have their trail indexes revealed to the hunters 

                    } 

                    switch(gameView->player[currentPlayer].trail[0]) { 

                        case gameView->player[PLAYER_DRACULA].trail[0] : 
                        case gameView->player[PLAYER_DRACULA].trail[1] :
                        case gameView->player[PLAYER_DRACULA].trail[2] :
                        case gameView->player[PLAYER_DRACULA].trail[3] :
                        case gameView->player[PLAYER_DRACULA].trail[4] :
                        case gameView->player[PLAYER_DRACULA].trail[5] :

                            // all moves that resulted in dracula being in that city 
                            // have their trail indexes revealed to the hunters

                            break; 

                        default : 

                            break; 

                    } */ 

        } 

        if (currentPlayer == PLAYER_DRACULA) {

            if (gameView->player[PLAYER_DRACULA].trail[0] == gameView->player[PLAYER_DRACULA].trail[1] &&
                    isLand(turnLocID)) { // dracula's location is repeated and is in a city 

                // dracula has hidden 
                // must we do anything with this ? 
                // note: the hunters will see 'HI' as location if they discover this 

            } 

            if (!isLand(turnLocID))  { // dracula is at sea and therefore loses 2 hp 

                gameView->player[PLAYER_DRACULA].health -= LIFE_LOSS_SEA;

            }

        }

        actionIndex += 2; // now up to first 'action character' GLO >X< .... 
        // analyse action and take into account effect
        // change score, location and hp if need be 

        actionLoop = 0;

        if (currentPlayer == PLAYER_LORD_GODALMING  || // determine action effect for hunters 
            currentPlayer == PLAYER_DR_SEWARD       || 
            currentPlayer == PLAYER_VAN_HELSING     || 
            currentPlayer == PLAYER_MINA_HARKER     ) { 

                while (actionLoop < 4 && gameView->player[currentPlayer].health > 0 
                        && gameStatus == GAME_IN_PROGRESS) { // loops through actions and accounts for effects (only 4 actions per string)

                    if (pastPlays[actionIndex] == 'T') { // trap encountered 

                        gameView->player[currentPlayer].health -= LIFE_LOSS_TRAP_ENCOUNTER;  

                    }

                    if (pastPlays[actionIndex] == 'V') { // immature sk encountered 

                        // may not need to do anything in gameview? 
                        // need to update immature vampire now and remove so from this location
                        // updateVampire(turnAbbrevLocation, roundNo, actionVariable)
                        // action variable can be defined to be the placing, vanquishing or maturing of a vampire

                    }

                    if (pastPlays[actionIndex] == 'D') { // big boy D encountered 

                        gameView->player[currentPlayer].health -= LIFE_LOSS_DRACULA_ENCOUNTER;
                        gameView->player[PLAYER_DRACULA].health -= LIFE_LOSS_HUNTER_ENCOUNTER;  

                    }


                    if (gameView->player[currentPlayer].health <= 0) { // player has died - must TP them and deduct score // we don't restore HP, see below 

                        gameView->score -= SCORE_LOSS_HUNTER_HOSPITAL; 
                        // apparently we don't have the TP in game history so I guess not in their trail
                        gameView->player[currentPlayer].curPos = ST_JOSEPH_AND_ST_MARYS; 
                        // apparently we also don't restore their health till their next turn 

                    }

                    actionLoop++; 
                    actionIndex++;

                    if (gameView->score <= 0) { 

                        gameStatus = DRACULA_WIN; 

                    }

                    if (gameView->player[PLAYER_DRACULA].health <= 0) {

                        gameStatus = HUNTER_WIN; 

                    }

                    if (gameView->score <= 0 && gameView->player[PLAYER_DRACULA].health <= 0) { 

                        gameStatus = DRAW;

                    }

                }

        } 
        
        if (turnNo%5 == 0) { // end of draculas turn and therefore end of that round 

            roundNo++;

        }

        if (currentPlayer == PLAYER_DRACULA && turnLocID == CASTLE_DRACULA) { 

            gameView->player[PLAYER_DRACULA].health += LIFE_GAIN_CASTLE_DRACULA; // dracula regains 10 hp at the end of a turn if he is in his hood

        }

        turnIndex += TURN_SIZE; 
        turnNo++;  

    }

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
LocationID *connectedRail(GameView currentView, LocationID lID, Round round) {
  //start->nPos->n2Pos
  //Probably have to crawl(graph) it
  QHead graph = initQ();
  int index = 0;
  LocationID *locs = malloc( sizeof(int)*NUM_MAP_LOCATIONS );
  VList start;
  addQ(graph, lID);
  //printf("%d\n", graph->head->next->Loc);
  while ( QSize(graph) > 0 ) {
    int setLoc = deQ(graph);
    //dispQ(graph);
    for ( start = currentView->newMap->connections[setLoc];
      start != NULL; start=start->next )
    {
      if ( inVList(currentView->newMap->connections[setLoc], start->v, RAIL) ) {
        if ( CheckUniqueLoc(locs, start->v) ) {
          locs[index] = start->v;
          index++;
          addQ(graph, start->v);
        }
      }
    }
  }
  index++;
  locs[index] = '\0';
  for ( int i = 0; locs[i] != '\0'; i++ ) {
    printf("RAIL: %s\n", idToName(locs[i]));
  }
  return locs;
}

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
  LocationID *arr = malloc( sizeof(LocationID) * NUM_MAP_LOCATIONS );
  int index = 0;
  VList head = currentView->newMap->connections[from], start;
  for ( start = head; start != NULL; start = start->next ) {
    if ( sea && inVList(head, start->v, BOAT) ) {
      if ( CheckUniqueLoc( arr, start->v ) ) {
        arr[index] = start->v;
        index++;
      }
    } if ( road && inVList(head, start->v, ROAD) ) {
      if ( CheckUniqueLoc( arr, start->v ) ) {
        arr[index] = start->v;
        index++;
      }
    } if ( rail && player != PLAYER_DRACULA ) {
      LocationID *toAdd = connectedRail(currentView, from, round);
      for ( int k = 0; toAdd[k] != '\0'; k++ ) {
       if( CheckUniqueLoc( arr, toAdd[k] ) ) {
         arr[index] = toAdd[k];
         index++;
       }
      }
      free(toAdd);
      rail = FALSE;
    }
  }
  arr[index++] = '\0';
  return arr;
}
