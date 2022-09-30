#include "cell.h"

#include <stdlib.h>
#include "io.h"

Cell* new_Cell()
{
    Cell* cell = (Cell*) malloc(sizeof(Cell));
    
    // Case malloc failed, print that malloc failed and abort execution    
    if(cell == NULL)
        prompt_IO(ERROR_IO, "cell.c, new_Cell(): malloc failed");

    // Set field piece to NULL and the field shot to 0
    cell->piece = NULL;
    cell->shot = 0;

    return cell;
}

void free_Cell(Cell* cell)
{
    free(cell);
}

