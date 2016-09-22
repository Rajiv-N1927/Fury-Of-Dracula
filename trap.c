// Things for the Trap.

#define MAX_TRAPS 3 ?

typedef struct trap {
  Round tRound;
  LocationID tLoc;
} Trap;

//-> add an array of traps in the gameView adt i.e.
//   Trap traps[MAX_TRAPS + 1];
//-> create a function for setting traps setTrap()
//-> Check trap doesn't expire updateTrap()
//-> Implement the rest of the checks for trap (explicit and implicit)
