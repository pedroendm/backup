#include "map.h"

#include "utils.h"
#include "io.h"
#include <stdlib.h>

#ifdef MATRIX

/*
    Function that see's if a piece can be added to the map.
    If it returns 0, than the piece can be added.
    If it returns 1, than (parts of) the piece would be outside the map
    If it returns 2, than (parts of) the piece would intersect with previous pieces, that is, there is already a (at least one) piece (or parts of), where this piece lies.
    Note: Nothing to the map happens, in all the cases.
*/
static int canAddPiece(Map* map, Piece* piece)
{
    /*
        Check if the attack is inside the map.
        Note: Check this first. or the next call made 'getStatus_Piece', may cause a segmentation fault.
    */
    if(piece->posX < 0 || piece->posX >= map->size || piece->posY < 0 || piece->posY >= map->size )
        return 1;

    for(int x = piece->posX - 2; x <= piece->posX + 2; x++) {
        for(int y = piece->posY - 2; y <= piece->posY + 2; y++) {
            // For the positions (x,y) that we need to attactch the piece, we need to see if:
            if(getStatus_Piece(piece, x, y) == 1) {
                // it's a valid position on the map
                if(x < 0 || x >= map->size || y < 0 || y >= map->size)
                    return 1;
                // and there's a piece there already.
                if(map->cells[x][y]->piece != NULL)
                    return 2;
            }
        }
    }
    // At this point, we know that the piece can be attactched
    return 0;
}

// Simple function to attactch the piece to the map.
static void attactchPiece(Map* map, Piece* piece)
{
    for(int i = piece->posX - 2; i <= piece->posX + 2; i++)
        for(int j = piece->posY - 2; j <= piece->posY + 2; j++)
            if(getStatus_Piece(piece, i, j) == 1)
                map->cells[i][j]->piece = piece;
}


Map* new_Map(int map_size)
{
    Map* map = (Map*) malloc(sizeof(Map));
    // Case malloc failed, print that malloc failed and abort execution
    if(map == NULL)
        prompt_IO(ERROR_IO, "map.c, new_Map(): first malloc failed");

    // Update size of the map
    map->size = map_size;

    map->cells = (Cell***) malloc(map->size * sizeof(Cell**));
    // Case malloc failed, print that malloc failed and abort execution
    if(map->cells == NULL)
        prompt_IO(ERROR_IO, "map.c, new_Map(): second malloc failed");

    for(int x = 0; x < map->size; x++) {
        map->cells[x] = (Cell**) malloc(map->size * sizeof(Cell*));
        // Case malloc failed, print that malloc failed and abort execution
        if(map->cells[x] == NULL)
            prompt_IO(ERROR_IO, "map.c, new_Map(): third malloc failed");

        for(int y = 0; y < map->size; y++)
            map->cells[x][y] = new_Cell();
    }
    return map;
}


int addPiece_Map(Map* map, Piece* piece)
{
    int resultAddingPiece = canAddPiece(map, piece);
    // If the result of the function canAddPiece is 0, then the piece can be added and it's added
    if(resultAddingPiece == 0)
        attactchPiece(map, piece);
    return resultAddingPiece;
}

int getPieceStatus_Map(Map* map, int x, int y)
{
    // Case there's no piece, return 0
    if(map->cells[x][y]->piece == NULL) return 0;
    // Case there's a piece, return the status of the piece on position (x,y)
    return getStatus_Piece(map->cells[x][y]->piece, x, y);
}

int getShotStatus_Map(Map* map, int x, int y)
{
    return map->cells[x][y]->shot;
}

int registerAttack_Map(Map* map, int x, int y)
{
    /*
        Attack outside the map.
        This needs to be verified first, to not cause a segmentation fault
        on the call made next to the function getPieceStatus,
        which would try to acess invalid positions on the map.
    */
    if(x < 0 || x >= map->size || y < 0 || y >= map->size)
        return -1;

    switch(getPieceStatus_Map(map, x, y)) {
        // Case there's no piece
        case 0: return 0;

        // Case there's a piece, not hitted
        case 1: {
            // Mark on the state of the piece that the position was hitted.
            registerAttack_Piece(map->cells[x][y]->piece, x, y);

            // Return accordingly to piece hitted
            switch(getType_Piece(map->cells[x][y]->piece)) {
                case 'I': return 1;
                case 'P': return 2;
                case 'T': return 3;
                case 'X': return 4;
                case 'Z': return 5;
                default: prompt_IO(ERROR_IO, "map.c, registerAttack_Map(): invalid piece type");
            }
        }
        // Case there's a piece, but already hitted
        case 2: return 6;

        // Case it's an invalid piece status: notify and abort execution
        default: prompt_IO(ERROR_IO, "map.c, registerAttack_Map(): invalid piece status");
    }

    // unreachable statement (Since, if it gets to the default case, the execution is aborted). Just to shutdown warning.
    return 0;
}

void registerShot_Map(Map* map, int x, int y, byte b)
{
    map->cells[x][y]->shot = b;
}

char getPieceType_Map(Map* map, int x, int y)
{
    return getType_Piece(map->cells[x][y]->piece);
}

void free_Map(Map* map)
{
    for(int x = 0; x < map->size; x++) {
        for(int y = 0; y < map->size; y++) {
            if(map->cells[x][y]->piece != NULL) {
                // The piece is only dealloced in the center position to avoid that we dealloc more than one time.
                if(map->cells[x][y]->piece->posX == x && map->cells[x][y]->piece->posY == y)
                    free_Piece(map->cells[x][y]->piece);
            }
            free_Cell(map->cells[x][y]);
        }
    }
    free(map);
}

#else // QUADTREE

/*
    Function that see's if a piece can be added to the map.
    If it returns 0, than the piece can be added.
    If it returns 1, than (parts of) the piece would be outside the map
    If it returns 2, than (parts of) the piece would intersect with previous pieces, that is, there is already a (at least one) piece (or parts of), where this piece lies.
    Note: Nothing to the map happens, in all the cases.
*/
static int canAddPiece(Map* map, Piece* piece)
{
    for(int x = piece->posX - 2; x <= piece->posX + 2; x++) {
        for(int y = piece->posY - 2; y <= piece->posY + 2; y++) {
            // For the positions (x,y) that we need to attactch the piece, we need to see if:
            if(getStatus_Piece(piece, x, y) == 1) {
                // it's a valid position on the map
                if(x < 0 || x >= map->size || y < 0 || y >= map->size)
                    return 1;
                // and there's a piece there already.
                if(hasCell_QuadTree(map->qt, x, y))
                    return 2;
            }
        }
    }
    // At this point, we know that the piece can be attactched
    return 0;
}

// Simple function to attactch the piece to the map.
static void attactchPiece(Map* map, Piece* piece)
{
    for(int i = piece->posX - 2; i <= piece->posX + 2; i++) {
        for(int j = piece->posY - 2; j <= piece->posY + 2; j++) {
            if(getStatus_Piece(piece, i, j) == 1){
                Cell* cell = new_Cell();
                cell->piece = piece;
                insert_QuadTree(map->qt, cell, i, j);
            }
        }  
    }
}

Map* new_Map(int size) 
{
    Map* map = (Map*)malloc(sizeof(Map));
    if(map == NULL)
        prompt_IO(ERROR_IO, "map.c, new_Map(): malloc failed");
    
    map->size = size;
    map->qt = new_QuadTree(size);
   
    return map;
}


int addPiece_Map(Map* map, Piece* piece)
{
  int resultAddingPiece = canAddPiece(map, piece);
    // If the result of the function canAddPiece is 0, then the piece can be added and it's added
    if(resultAddingPiece == 0) 
        attactchPiece(map, piece);
    return resultAddingPiece;
  return -1;
}

int registerAttack_Map(Map* map, int x, int y)
{
    // Attack outside the map.
    if(x < 0 || x >= map->size || y < 0 || y >= map->size)
        return -1;
    
    // Search for the cell, on position (x,y)
    Cell* cell_found = NULL;
    search_QuadTree(map->qt, &cell_found, x, y);
    
    // Case there's no piece
    if(cell_found == NULL || cell_found->piece == NULL) return 0; 

    // Case there's a piece
    int val = getStatus_Piece(cell_found->piece, x, y);
    switch(val) {
        // Case there's a piece, hitted
        case 1: {
            // Mark on the state of the piece that the position was hitted.
            registerAttack_Piece(cell_found->piece, x, y);

            // Return accordingly to piece hitted
            switch(getType_Piece(cell_found->piece)) {
                case 'I': return 1;
                case 'P': return 2;
                case 'T': return 3;
                case 'X': return 4;
                case 'Z': return 5;
                default: prompt_IO(ERROR_IO, "map.c, registerAttack_Map(): invalid piece type");
            }
        }
        // Case there's a piece, but already hitted
        case 2: return 6;
        // Case it's an invalid piece status: notify and abort execution
        default: prompt_IO(ERROR_IO, "map.c, registerAttack_Map(): invalid piece status");
    }

    // unreachable statement (Since, if it gets to the default case, the execution is aborted). Just to shutdown warning.
    return 0;
}

void registerShot_Map(Map* map, int x, int y, byte b)
{
  // Search for the cell on position (x,y).
  Cell* cell_found = NULL;
  search_QuadTree(map->qt, &cell_found, x, y);
  
  // If it's NULL then doesn't exist a node on the tree with the point (x,y), we must add one.
  if(cell_found == NULL) {
      Cell* cell = new_Cell();
      cell->shot = b;
      insert_QuadTree(map->qt, cell, x, y);
  }
  // Case it exists. 
  else
    cell_found->shot = b;
}

int getPieceStatus_Map(Map* map, int x, int y)
{
  // Search for the cell on position (x,y).
  Cell* cell_found = NULL;
  search_QuadTree(map->qt, &cell_found, x, y);
  
  // Case no cell or piece.
  if(cell_found == NULL || cell_found->piece == NULL) 
    return 0;
  // Case there's a piece.
  else
    return getStatus_Piece(cell_found->piece, x, y);
}

int getShotStatus_Map(Map* map, int x, int y)
{
  // Search for the cell on position (x,y).
  Cell* cell_found = NULL;
  search_QuadTree(map->qt, &cell_found, x, y);
  
  // Case there's no cell.
  if(cell_found == NULL)
    return 0;
  // Case there's a cell.
  else
    return cell_found->shot;
}

char getPieceType_Map(Map* map, int x, int y)
{
    // Search for the cell on position (x,y).
    Cell* cell;
    search_QuadTree(map->qt, &cell, x, y);

    // As stated in the header file, this function doesn't validate if the piece exists, 
    // since everytime we use this function, we know the piece already exist.
    return getType_Piece(cell->piece);
}

void free_Map(Map* map)
{
  free_QuadTree(map->qt);
  free(map);
}

#endif
