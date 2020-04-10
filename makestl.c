#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "stlutils.h"

/*
 * A simple example and test program for the 3D drawing library.
 * Output (name given as a commandline parameter) is an STL-file
 * of five boxes piled on top of each other.
 */

void exit(int code);


/* Eight corners of a box.
 * Assume that points 0,1,2,3 (clockwise) define a side facing you. Then point 4 is behind 0, 5 is behing 1 etc
 */
double data[8][3] = {
  { 5.0, 5.0, 1.0 },  // point0
  { 5.0, 0.0, 1.0 },
  { 0.0, 0.0, 1.0 },
  { 0.0, 5.0, 1.0 },
  { 5.0, 5.0, 0.0 },
  { 5.0, 0.0, 0.0 },
  { 0.0, 0.0, 0.0 },
  { 0.0, 5.0, 0.0 }   // point7
};

/*
 * Phases of the program
 * - initialize STL-file with "makePrefix"
 * - Five times:
 *    Create a box and push its triagles to buffer
 * - Write all triangles from the buffer to STL file.
 */
int main (int argc, char *argv[]) {
  FILE *f;
  uint32_t nTri;
  int i;
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filemame.stl>\n", argv[0]);
    exit (1);
  }
  if ((f = fopen(argv[1], "w")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", argv[1]);
    exit (1);
  }
  makePrefix(f);
  pushTriangles(12, createBox(data));

  /* Scale X and Y dimenssion and move in Z.
   * Note that libraries should not keep pointers to the data,
   * so we can modify the original data.
   */
  for (i=0; i<8; ++i) {
    data[i][0] *= 0.8;
    data[i][1] *= 0.8;
    data[i][2] += 1.0;
  }
  pushTriangles(12, createBox(data));
  for (i=0; i<8; ++i) {
    data[i][0] *= 0.8;
    data[i][1] *= 0.8;
    data[i][2] += 1.0;
  }
  pushTriangles(12,createBox(data));
  for (i=0; i<8; ++i) {
    data[i][0] *= 0.8;
    data[i][1] *= 0.8;
    data[i][2] += 1.0;
  }
  pushTriangles(12,createBox(data));
  for (i=0; i<8; ++i) {
    data[i][0] *= 0.8;
    data[i][1] *= 0.8;
    data[i][2] += 1.0;
  }
  pushTriangles(12,createBox(data));
  
  writeAllPushedTriangles(f);
  fclose(f);
  return 0;
}
