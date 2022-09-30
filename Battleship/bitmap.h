/*
  bitmap.h
  Representation of a bitmap 5x5.
  
  Each position of the bitmap is represented by a byte, so, in total, a bitmap ocuppies 25 bytes. 
  
  The x-axis go down and the y-axis go right.
  The top-left corner is considered the position (0,0) and the lower-right corner the position (4,4).
*/

#ifndef BITMAP_H
#define BITMAP_H

#include "utils.h"

// Definition of the bitmap.
typedef struct BitMap 
{
  // Internally, the bitmap is represented by an array of length 25.
  byte field[25];
} BitMap;

// Allocs, dynamically, a new bitmap
BitMap* new_BitMap();

/*
  Update the contents of the bitmap to an format given by the type,
  rotated by 'n' degrees, clockwise, with 'n' being 0, 90, 180 or 270.

  Types available: 'I', 'P', 'T', 'X' and 'Z'.
*/
void update_BitMap(BitMap* bitmap, char type, int n);

// Set the (x,y) position of the bitmap to b
void setValue_BitMap(BitMap* bitmap, int x, int y, byte b);

// Returns the value in the position (x,y) of the bitmap
byte getValue_BitMap(BitMap* bitmap, int x, int y);

// Deallocs the bitmap
void free_BitMap(BitMap*);

#endif
