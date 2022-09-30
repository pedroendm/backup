/*
  map.h
  Representation of a square map.

  The x-axis go down and the y-axis go right.
  The top-left corner is considered the position (0,0) and
  the lower-right corner the position ('size' - 1, 'size' - 1).
 */

#ifndef MAP_H
#define MAP_H

#include "cell.h"

#ifdef MATRIX

//  Definition of the Map
typedef struct Map
{
    // Size of the map
    int size;

    // Cells of the map
    Cell*** cells;
} Map;

#else //QUADTREE

#include "quadtree.h"

typedef struct Map
{
    // Size of the map
    int size;

    // Cells of the map
    QuadTree* qt;
} Map;

#endif


//  Allocs a new square map of width 'size'
Map* new_Map(int size);

/*
    Tries to add a piece to the map and returns an int, signaling the result of adding the piece.
    If the return value is 0, than the piece could be, and is, attatched to the map.
    If the return value is 1, than the piece piece would be outside the map (or parts of it).
    If the return value is 2, than, where the piece lies, there is already some piece.
    In both this last two cases, where the piece can't be added, the map isn't affected.
 */
int addPiece_Map(Map* map, Piece* piece);

/*
    Function used to represent an attack.
    This function returns an int, signaling the sucess of the attack.
    If the return value is -1, than the attack went outside the map.
    If the return value is 0, than was attacked a cell without piece.
    If the return value is 1, than there was a piece of type I, not hitted, that is, was a hit shot.
    If the return value is 2, than there was a piece of type P, not hitted, that is, was a hit shot.
    If the return value is 3, than there was a piece of type T, not hitted, that is, was a hit shot.
    If the return value is 4, than there was a piece of type X, not hitted, that is, was a hit shot.
    If the return value is 5, than there was a piece of type Z, not hitted, that is, was a hit shot.
    If the return value is 6, than there was a piece, but was already destroyed.
 */
int registerAttack_Map(Map* map, int x, int y);


// Sets the field 'shot' of the cell (x,y) on the map to b.
void registerShot_Map(Map* map, int x, int y, byte b);


/*
    Returns the state of the piece on position (x,y) in the map.
    If the return value is 0, than there's no piece at position (x,y).
    If the return value is 1, than there's a piece at position (x,y) and is not hitted.
    If the return value is 2, than there's a piece at position (x,y) and is hitted.
*/
int getPieceStatus_Map(Map* map, int x, int y);

// Returns the shot status of the cell on position (x,y) in the map.
int getShotStatus_Map(Map* map, int x, int y);

// Returns the type of the piece on position (x,y) in the map.
// For efficient reasons, it's not safe, it doesn't verify if the piece exists. 
// If doesn't get verified, because in the program, when we call this function we had always verify if the piece existed, before.
char getPieceType_Map(Map* map, int x, int y);

// Frees the map and all resources in it
void free_Map(Map*);

#endif
