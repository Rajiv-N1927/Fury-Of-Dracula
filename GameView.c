// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Queue.h"
#include <string.h> 

#include "Map.h"


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

//The GameViewer struct
struct gameView {
    Player player[NUM_PLAYERS];
    // Trap traps[MAX_TRAPS];
    int score;
    int turnNo;
    Round roundNo;
    Map newMap;
};

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

static GameView initGV() {

  GameView gameView = (GameView)malloc(sizeof(struct gameView));

  initPlayers(gameView->player);

  gameView->roundNo = 0;
  gameView->turnNo  = 1;
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

/// Creates a new GameView to summarise the current state of the game 
// Add this function and the defines at the top 
GameView newGameView(char *pastPlays, PlayerMessage messages[]) { 

    GameView gameView = initGV();

    if (pastPlays[0] == '\0') { 

      return gameView; 

    }

    int turnIndex = 0; 
    int actionIndex = 0; 
    int actionLoop;
    int gameStatus = GAME_IN_PROGRESS; 

    while (turnIndex < strlen(pastPlays) && gameStatus == GAME_IN_PROGRESS) { // can alternatively have condition while turnIndex < strlen(pastPlays) && gamestatus...

        PlayerID currentPlayer = charToPlayerID(pastPlays[turnIndex]);

        if (currentPlayer == PLAYER_DRACULA) {

            gameView->score -= SCORE_LOSS_DRACULA_TURN;

        } else if (gameView->player[currentPlayer].health <= 0) { // else if hunter who died last round - need to restore hp 

            gameView->player[currentPlayer].health = GAME_START_HUNTER_LIFE_POINTS; 

        }

        actionIndex = turnIndex + 1; // first location char

        char turnAbbrevLocation[3] = {pastPlays[actionIndex], pastPlays[actionIndex+1], '\0'};

        LocationID turnLocID; 

        if (!strcmp(turnAbbrevLocation, "C?")) { 

            turnLocID = CITY_UNKNOWN; 

        }

       else if (!strcmp(turnAbbrevLocation, "S?")) { 

            turnLocID = SEA_UNKNOWN; 

        }

       else if (!strcmp(turnAbbrevLocation,"HI")) { 

            turnLocID = HIDE; 

        }

       else if (!strcmp(turnAbbrevLocation, "D1")) { 

            turnLocID = DOUBLE_BACK_1; 

        }

        else if (!strcmp(turnAbbrevLocation, "D2")) { 


            turnLocID = DOUBLE_BACK_2; 

        }

        else if (!strcmp(turnAbbrevLocation, "D3")) { 

            turnLocID = DOUBLE_BACK_3; 

        }

        else if (!strcmp(turnAbbrevLocation, "D4")) { 

            turnLocID = DOUBLE_BACK_4; 

        }

        else if (!strcmp(turnAbbrevLocation,"D5")) { 

            turnLocID = DOUBLE_BACK_5; 

        }

        else if (!strcmp(turnAbbrevLocation, "TP")) { 

            turnLocID = TELEPORT; 

        } else {

            //printf("flag 30000\n"); 

            turnLocID = abbrevToID(turnAbbrevLocation);

        } 

        setTrail(gameView, currentPlayer, turnLocID); // updating trail for current player 
        gameView->player[currentPlayer].curPos = turnLocID; 


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
               
        } 

        if (currentPlayer == PLAYER_DRACULA) {

            //printf("flag 4\n"); 

            if (turnLocID == SEA_UNKNOWN) { 

               gameView->player[PLAYER_DRACULA].health -= LIFE_LOSS_SEA;

            }

            else if (turnLocID == CITY_UNKNOWN) { 

               // good on dracula, he's ok 

            }

            else if (turnLocID >= MIN_MAP_LOCATION && turnLocID <= MAX_MAP_LOCATION) {

               if (!isLand(gameView->player[currentPlayer].curPos))  { // dracula is at sea and therefore loses 2 hp 

                   gameView->player[PLAYER_DRACULA].health -= LIFE_LOSS_SEA;

               } 

            } else if (turnLocID >= 102 && turnLocID <= 108) { 

               LocationID dbID; 

                  switch (turnLocID) { 

                     case DOUBLE_BACK_1: dbID = gameView->player[currentPlayer].trail[1]; break;
                     case DOUBLE_BACK_2: dbID = gameView->player[currentPlayer].trail[2]; break;
                     case DOUBLE_BACK_3: dbID = gameView->player[currentPlayer].trail[3]; break;
                     case DOUBLE_BACK_4: dbID = gameView->player[currentPlayer].trail[4]; break;
                     case DOUBLE_BACK_5: dbID = gameView->player[currentPlayer].trail[5]; break;
                     case TELEPORT: dbID = CASTLE_DRACULA; 
                     case HIDE: dbID = CITY_UNKNOWN; 
                     default: break;

                  }  

                  if (dbID == SEA_UNKNOWN) { 

                     gameView->player[PLAYER_DRACULA].health -= LIFE_LOSS_SEA;

                   }

                  else if (dbID == CITY_UNKNOWN || dbID == UNKNOWN_LOCATION) { 

                     // good on dracula, he's ok 

                  }

                  else if (dbID >= MIN_MAP_LOCATION && dbID <= MAX_MAP_LOCATION) {

                     if (!isLand(dbID))  { // dracula is at sea and therefore loses 2 hp 

                         gameView->player[PLAYER_DRACULA].health -= LIFE_LOSS_SEA;

                     } 

                  }

            } 

      }

        //printf(" actionflag 3\n"); 
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
        
        if (gameView->turnNo%5 == 0) { // end of draculas turn and therefore end of that round 

            gameView->roundNo++;

        }

        if (currentPlayer == PLAYER_DRACULA && turnLocID == CASTLE_DRACULA) { 

            gameView->player[PLAYER_DRACULA].health += LIFE_GAIN_CASTLE_DRACULA; // dracula regains 10 hp at the end of a turn if he is in his hood

        }

        turnIndex += TURN_SIZE; 
        gameView->turnNo++;  
        // printf("turn no [%d] previous char [%d]\n", gameView->turnNo, currentPlayer);

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
   // printf("currentView->turnNo is %d\n", currentView->turnNo);
   return currentView->player[((currentView->turnNo%5)-1)].playerID;

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


