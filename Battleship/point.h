#ifndef POINT_H
#define POINT_H

typedef struct Point 
{
    // (x,y)
    int x, y;
} Point;

// Allocs a point.
Point* new_Point(int x, int y);

// Change the coordinates of a point.
void set_Point(Point* p, int x, int y);

// Frees the point.
void free_Point(Point* p);

#endif