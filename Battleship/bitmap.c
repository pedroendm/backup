#include "bitmap.h"

#include <stdlib.h>
#include "io.h"

/*
 Available formats of bitmaps. 
 They are stored compressed, through an int, which the corresponding binary sequence, corresponds to the values of the bitmap, 
 with the position (4,4) of the bitmap being the less significant digit of the number 
 and the position (0,0) the eighth most significant digit of the number.
 The most 7 significant digits aren't necessary, so they aren't used.

 The reason to store them compressed is memory: 4 bytes is less 25 bytes, for more than a factor of 6.
 To store the five types of bitmaps we would need 5 * 25 = 125 bytes and, in this way, we only need 5 * 4 = 20 bytes. 
*/
static int getCompressedType(char type)
{
  switch(type) {
    case 'I': return 31744;
    case 'P': return 202880;
    case 'T': return 462976;
    case 'X': return 332096;
    case 'Z': return 397504;
    // Case an invalid type is used, notify and abort execution
    default: prompt_IO(ERROR_IO, "bitmap.c, getCompressedType(): invalid type");
  }
  // unreachable statement (Since, if it gets to the default case, the execution is aborted). Just to shutdown warning.
  return 0;
}

/* 
  Function that stores in an array (field) the binary sequence of an int (compressed_form),
  The less significant digits are in the higher positions of the array, with the position 24 of the array (the last one), being the less significant digit.
  Note than an int usually as 32 bits and the array only has 25: the 7 most significant digits aren't stored.
*/
static void decompress(int compressed_form, byte field[25])
{
  for(int p = 24; p >= 0; p--) {
    field[p] = compressed_form % 2;
    compressed_form /= 2;
  }
}

/*
  We have an array representing the square bitmap.
  This functions return the position of a (x,y) in the array when rotated by n degrees, clockwise.

  Function are named 'rotateN', where 'N' is the corresponding degree of the rotation.
*/
static int rotate0(int x, int y)
{
  return x * 5 + y;
}

static int rotate90(int x, int y)
{
  return 20 + x - (5 * y);
}

static int rotate180(int x, int y)
{
  return 24 - (5 * x) - y;
}

static int rotate270(int x, int y)
{
  return 4 - x + (5 * y);
}


BitMap* new_BitMap() 
{
  BitMap* bm = (BitMap*) malloc(sizeof(BitMap));

  // Case malloc failed, print that malloc failed and abort execution 
  if(bm == NULL)
    prompt_IO(ERROR_IO, "bitmap.c, new_BitMap(): malloc failed");

  return bm;
}

void update_BitMap(BitMap* bm, char type, int n)
{
  /* 
    Get the function to the rotation, acordingly to the the degree 'n'.
    It's better to choose and store the function to be used first, than, in the loop, where the bitmap is updated, choose, every iteration, what function to use.
  */
  int (*rotateFunction)(int, int);
  switch(n) {
      case 0: rotateFunction = rotate0; break;
      case 90: rotateFunction = rotate90; break;
      case 180: rotateFunction = rotate180; break;
      case 270: rotateFunction = rotate270; break;
      // Case it's an invalid rotation, notify and abort execution.
      default: prompt_IO(ERROR_IO, "bitmap.c, update_BitMap(): invalid rotation");
  }

  // Get the compressed format of the type
  int compressed_form = getCompressedType(type);

  // Decompression of the compressed format of the type to an array field
  byte field[25];
  decompress(compressed_form, field);

  // Updating the field of the bitmap.
  for(int x = 0; x < 5; x++)
    for(int y = 0; y < 5; y++)
      // The position (x,y) of the bitmap is set to the position given by the respective rotating function of the field decompressed.
      bm->field[x * 5 + y] = field[rotateFunction(x, y)];
}

// The position (x,y) is the position x * 5  + y in the array
void setValue_BitMap(BitMap* bm, int x, int y, byte b)
{
  bm->field[x * 5 + y] = b;
}

// The position (x,y) is the position x * 5  + y in the array
byte getValue_BitMap(BitMap* bm, int x, int y) 
{
  return bm->field[x * 5 + y];
}

void free_BitMap(BitMap* bm) 
{
  free(bm);
}