/*
  piece.h
  Representation of a piece.

  The piece has a type, 
  the bitmap itself, to represent the state and
  it's position in the map (posX, posY), corresponding to the center of the bitmap on the map, 
*/

#ifndef PIECE_H
#define PIECE_H

#include "bitmap.h"

typedef struct Piece 
{
    // Type of the piece
    char type;

    // Where's the center of the bitmap lies in the map.
    int posX, posY;

    // Bitmap 
    BitMap* bitmap;
} Piece;

// Alloc, dynamically, a new piece
Piece* new_Piece();

// Updates all the fields of the piece
void update_Piece(Piece* piece, char type, int posX, int posY, int rotation);

/*
  Attack the piece on position (x,y)
  Note: (x,y)'s are relative to the map and can go from (posX - 2, posY - 2) to (posX + 2, posY + 2), inclusive, with (posX, posY) being the center of the bitmap
*/
void registerAttack_Piece(Piece* p, int x, int y);

/*
  Get the status of the piece, that is, if it's hitted or not.
  Note: (x,y)'s are relative to the map and can go from (posX - 2, posY - 2) to (posX + 2, posY + 2), inclusive, with (posX, posY) being the center of the bitmap
*/
byte getStatus_Piece(Piece* p, int x, int y);

// Returns the type of the piece, that is, 'I', 'P', 'T', 'X' or 'Z'.
char getType_Piece(Piece* piece);

// Deallocs the piece, including all resources allocated by the piece
void free_Piece(Piece* p);

#endif
