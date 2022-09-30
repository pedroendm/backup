/*
  cell.h
  Representation of a cell.

  Each cell has a pointer to a piece (if equals to NULL then the cell hasn't a piece), and field 'shot' meaning:
    0 -> No shot;
    1 -> Missed shot;
    2 -> Shot that hit a piece I;
    3 -> Shot that hit a piece P;
    4 -> Shot that hit a piece T;
    5 -> Shot that hit a piece X; 
    6 -> Shot that hit a piece Z;
*/

#ifndef CELL_H
#define CELL_H

#include "piece.h"

// Definition of the cell
typedef struct Cell 
{
    Piece* piece;
    byte shot;
} Cell;

// Alloc, dynamically, a new cell and sets the field piece to NULL and the field shot to 0
Cell* new_Cell();

/*
  Frees the cell. 
  NOTE: The piece isn't freed. 
*/
void free_Cell(Cell*);

#endif