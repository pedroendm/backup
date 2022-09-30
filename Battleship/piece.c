 #include "piece.h"

#include <stdlib.h>
#include "utils.h"
#include "io.h"

Piece* new_Piece()
{
    Piece* piece = (Piece*) malloc(sizeof(Piece));

    // Case malloc failed, print that malloc failed and abort execution 
    if(piece == NULL)
        prompt_IO(ERROR_IO, "piece.c, new_Piece(): malloc failed");
    
    // Alloc the bitmap of the piece
    piece->bitmap = new_BitMap();

    return piece;
}

void update_Piece(Piece* piece, char type, int posX, int posY, int n)
{
    // Update the type of the piece
    piece->type = type;

    // Update the position of the piece
    piece->posX = posX;
    piece->posY = posY;

    // Update the bitmap
    update_BitMap(piece->bitmap, type, n);
}

void registerAttack_Piece(Piece* p, int x, int y)
{
    // Since the (x,y) is relative to the map, with (posX, posY) being the center of the piece, the (x,y) is the position (x - (p->posX - 2),  y - (p->posY - 2)) in the bitmap.
    setValue_BitMap(p->bitmap, x - (p->posX - 2), y - (p->posY - 2), 2);
}

byte getStatus_Piece(Piece* p, int x, int y)
{
    // Since the (x,y) is relative to the map, with (posX, posY) being the center of the piece, the (x,y) is the position (x - (p->posX - 2),  y - (p->posY - 2)) in the bitmap.
    return getValue_BitMap(p->bitmap, x - (p->posX - 2), y - (p->posY - 2));
}

char getType_Piece(Piece* piece)
{
    return piece->type;
}

void free_Piece(Piece* piece) 
{
    free_BitMap(piece->bitmap);
    free(piece);
}
 