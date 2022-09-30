#include "quadtree.h"

#include "io.h"
#include "utils.h"
#include <stdlib.h>

// Check is a point is inside the boundaries of a square, define by the coordinates that limit the quadtree qt.
static bool inside(QuadTree* qt, Point* p) 
{ 
    if(p->x >= qt->topLeft.x && p->x <= qt->botRight.x && p->y >= qt->topLeft.y && p->y <= qt->botRight.y)
        return true;
    return false;
} 

// Allocs a new quadnode.
static QuadNode* new_QuadNode(Cell* cell, int x, int y) {
    QuadNode* newNode = (QuadNode*) malloc(sizeof(QuadNode));
    if(newNode == NULL)
        prompt_IO(ERROR_IO, "quadtree.c, new_QuadNode(): malloc failed");

    newNode->cell = cell;
    set_Point(&newNode->p, x, y);
    
    return newNode;
}

// Alocs a new quadtree with boundaries defined by the upper corner (x1,y1) and bottom corner (x2,y2).
QuadTree* newAux(int x1, int y1, int x2, int y2)
{
    QuadTree* qt = (QuadTree*) malloc(sizeof(QuadTree));
    if(qt == NULL)
        prompt_IO(ERROR_IO, "quadtree.c, newAux(): malloc failed");

    qt->n = NULL;
    for(int i = 0; i < 4; i++)
        qt->quadrants[i] = NULL;
    
    set_Point(&qt->topLeft, x1, y1);
    set_Point(&qt->botRight, x2, y2);
    
    return qt;
}

// Allocs a new quadtree with boundaries defined by the upper corner (0,0) and bottom corner (size,size).
QuadTree* new_QuadTree(int size)
{
    QuadTree* qt = (QuadTree*) malloc(sizeof(QuadTree));
    if(qt == NULL)
        prompt_IO(ERROR_IO, "quadtree.c, new_QuadTree(): malloc failed");

    qt->n = NULL;
    for(int i = 0; i < 4; i++)
        qt->quadrants[i] = NULL;
    
    set_Point(&qt->topLeft, 0, 0);
    set_Point(&qt->botRight, size, size);
    
    return qt;
}

static void insertAux(QuadTree* qt, QuadNode* qn) 
{
    // Check if it's inside the boundaries
    if(!inside(qt, &qn->p))
        return;
    
    // Reached a unit quad
    if(abs(qt->topLeft.x - qt->botRight.x) <= 1 && abs(qt->topLeft.y - qt->botRight.y) <= 1) {
        if(qt->n == NULL)
            qt->n = qn;
        return;
    }

    // Choose the appropriate subtree to add.
    // L
    if ((qt->topLeft.x + qt->botRight.x) / 2 >= qn->p.x) 
    { 
        // TL 
        if ((qt->topLeft.y + qt->botRight.y) / 2 >= qn->p.y) 
        { 
            if (qt->quadrants[0] == NULL)
                qt->quadrants[0] = newAux(qt->topLeft.x, qt->topLeft.y, (qt->topLeft.x + qt->botRight.x) / 2, (qt->topLeft.y + qt->botRight.y) / 2);
            insertAux(qt->quadrants[0], qn); 
        } 
        // BL 
        else
        { 
            if (qt->quadrants[2] == NULL)
                qt->quadrants[2] = newAux(qt->topLeft.x, (qt->topLeft.y + qt->botRight.y) / 2, (qt->topLeft.x + qt->botRight.x) / 2, qt->botRight.y); 
            insertAux(qt->quadrants[2], qn); 
        } 
    } 
    // R
    else
    { 
        // TR
        if ((qt->topLeft.y + qt->botRight.y) / 2 >= qn->p.y) 
        { 

            if (qt->quadrants[1] == NULL)
                qt->quadrants[1] = newAux((qt->topLeft.x + qt->botRight.x) / 2, qt->topLeft.y, qt->botRight.x, (qt->topLeft.y + qt->botRight.y) / 2); 
            insertAux(qt->quadrants[1], qn); 
        } 
  
        // BR 
        else
        { 
            if (qt->quadrants[3] == NULL)
                qt->quadrants[3] = newAux((qt->topLeft.x + qt->botRight.x) / 2, (qt->topLeft.y + qt->botRight.y) / 2, qt->botRight.x, qt->botRight.y); 
            insertAux(qt->quadrants[3], qn); 
        } 
    } 
}   

void insert_QuadTree(QuadTree* qt, Cell* cell, int x, int y) {
    QuadNode* qn = new_QuadNode(cell, x, y);
    insertAux(qt, qn);
}

static void searchAux(QuadTree* qt, Cell** cell_found, Point* p) {
    // Not in this region
    if (!inside(qt, p)) 
        return; 
  
    // Unit QuadTree 
    if (qt->n != NULL) {
        *cell_found = qt->n->cell;
        return; 
    } 

    // L
    if ((qt->topLeft.x + qt->botRight.x) / 2 >= p->x) 
    { 
        // TL
        if ((qt->topLeft.y + qt->botRight.y) / 2 >= p->y) 
        { 
            if (qt->quadrants[0] == NULL) 
                return; 
            searchAux(qt->quadrants[0], cell_found, p); 
        } 
  
        // BL 
        else
        { 
            if (qt->quadrants[2] == NULL) 
                return; 
            searchAux(qt->quadrants[2], cell_found, p); 
        } 
    } 
    // R
    else
    { 
        // TR 
        if ((qt->topLeft.y + qt->botRight.y) / 2 >= p->y) 
        { 
            if (qt->quadrants[1] == NULL) 
                return; 
            searchAux(qt->quadrants[1], cell_found, p); 
        } 
  
        // BR 
        else
        { 
            if (qt->quadrants[3] == NULL) 
                return; 
            searchAux(qt->quadrants[3], cell_found, p); 
        } 
    }  
}

void search_QuadTree(QuadTree* qt, Cell** cell_found, int x, int y) 
{
    Point* p = new_Point(x, y);
    searchAux(qt, cell_found, p);
    free_Point(p);
}

bool hasCell_QuadTree(QuadTree* qt, int x, int y)
{
    // Search for the cell on position (x,y).
    Cell* cell_found = NULL;
    search_QuadTree(qt, &cell_found, x, y);
    
    if(cell_found != NULL) return true;
    return false;
}

// Frees a quadnode and all the memory allocated in it.
void free_QuadNode(QuadNode* qn)
{
    if(qn != NULL) {
        if(qn->cell != NULL)
            free_Cell(qn->cell);
        free(qn);
    }
}

void free_QuadTree(QuadTree* qt)
{
    if(qt != NULL) {
        free_QuadNode(qt->n);
        for(int i = 0; i < 4; i++)
            free_QuadTree(qt->quadrants[i]);
        free(qt); 
    }
}

