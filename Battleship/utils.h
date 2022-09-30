/*
  utils.h
  Utilities.  
*/

#ifndef UTILS_H
#define UTILS_H

#define PLAYER_ATTACKING game->player_attacking

#define PLAYER_UNDER_ATTACK (game->player_attacking + 1) % 2

// Boolean data type
typedef unsigned char bool;
#define true 1
#define false 0

// Byte data type
typedef unsigned char byte;

/*
    Simple function used to associate an int to a type of a piece (char).
    Typically used inside for's looping through the types of pieces.
    Given 0, this function returns 'I'.
    Given 1, this function returns 'P'.
    Given 2, this function returns 'T'.
    Given 3, this function returns 'X'.
    Given 4, this function returns 'Z'.
*/
char getType_Utils(int type);

// Returns the absolute value of a number
int abs(int n);

#endif