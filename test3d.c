#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "stlutils.h"

void exit(int code);


main (int argc, char *argv[]) {
  FILE *f;
  uint32_t nTri;
  int i;
  triangle *t;
  float angle;
  float wid = 1.0;
  float x, y;
  float sc = 2*3.141216/18;
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filemame.stl>\n", argv[0]);
    exit (1);
  }
  if ((f = fopen(argv[1], "w")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", argv[1]);
    exit (1);
  }
  makePrefix(f);

  for (i=0; i<18; ++i) {
    angle = i * 2*3.141216 / 18;
    x = 4 * cos(angle);
    y = 4 * sin(angle);
    t = create3DLine(x, y, 0.0, 0.0, 0.0, 5.0, wid);
    if (t != NULL)
      pushTriangles(12, t);
    t = create3DLine(x, y, 0.0, 4*cos(angle+sc), 4*sin(angle+sc), 0.0, wid);
    if (t != NULL)
      pushTriangles(12, t);
    t = create3DLine(x, y, 0.0, 6*cos(angle), 6*sin(angle), 1.7, wid);
    if (t != NULL)
      pushTriangles(12, t);
    t = create3DLine(0.0, 0.5, 0.0, 3*cos(angle), 3*sin(angle), 3.2, wid);
    if (t != NULL)
      pushTriangles(12, t);
    x = 8 * cos(angle);
    y = 8 * sin(angle);
    t = create3DLine(x, y, 0.0, 0.0, 0.0, 5.0, wid);
    if (t != NULL)
      pushTriangles(12, t);
    t = create3DLine(x, y, 0.0, 8*cos(angle+sc), 8*sin(angle+sc), 0.0, wid);
    if (t != NULL)
      pushTriangles(12, t);
  }

  writeAllPushedTriangles(f);
  fclose(f);
}
