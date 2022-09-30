#ifndef QUADTREE_H
#define QUADTREE_H

#include "cell.h"
#include "quadtree.h"
#include "point.h"

typedef struct QuadNode {
  // (x,y)
  Point p;
  // The data on the position (x,y)
  Cell* cell;
} QuadNode;

typedef struct QuadTree {
  // Limits
  Point topLeft;
  Point botRight;

  // Node
  QuadNode* n;

  // Subtrees/Quadrants (TL (topleft), TR (topright), BL (botleft), BR(botright), respectively).
  struct QuadTree* quadrants[4];
} QuadTree;

// Allocs a new quadtree.
QuadTree* new_QuadTree(int size);

// Insert a node in the quadtree representing the position (x,y) with a Cell cell.
void insert_QuadTree(QuadTree* qt, Cell* cell, int x, int y);

// Search the quadtree for the point (x,y). If found, set *cell_found to the cell of the node. 
// That is, we can give this function a pointer, and this function makes the pointer point to the cell in position (x,y). 
void search_QuadTree(QuadTree* qt, Cell** cell_found, int x, int y);

// Returns true if exists a node, representing the point (x,y),
// and has a Cell ('cell' not null), in it. Otherwise, returns false.
bool hasCell_QuadTree(QuadTree* qt, int x, int y);

// Frees the quadtree and all the memory allocated in it.
void free_QuadTree(QuadTree* qt);

#endif