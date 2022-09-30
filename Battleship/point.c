#include "point.h"

#include <stdlib.h>
#include "io.h"

Point* new_Point(int x, int y)
{
    Point* point = (Point*) malloc(sizeof(Point));
    if(point == NULL)
        prompt_IO(ERROR_IO, "point.c, new_Point(): malloc failed");

    point->x = x;
    point->y = y;

    return point;
}

void set_Point(Point* p, int x, int y)
{
    p->x = x;
    p->y = y;
}

void free_Point(Point* p)
{
    free(p);
}