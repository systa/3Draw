#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "stlutils.h"

void exit(int code);

#define TORAD (2*3.141215/360)

void trig (double size, double x, double y, double z, double angle, double wid) {
  triangle *t;
  double x1 = x+cos((angle+90)*TORAD)*size;
  double y1 = y+sin((angle+90)*TORAD)*size;
  double x2 = x+cos((angle+210)*TORAD)*size;
  double y2 = y+sin((angle+210)*TORAD)*size;
  double x3 = x+cos((angle+330)*TORAD)*size;
  double y3 = y+sin((angle+330)*TORAD)*size;
  t = create3DLine(x,y,z,x1,y1,z-size,wid);
  if (t != NULL)
    pushTriangles(12, t);
  t = create3DLine(x,y,z,x2,y2,z-size,wid);
  if (t != NULL)
    pushTriangles(12, t);
  t = create3DLine(x,y,z,x3,y3,z-size,wid);
  if (t != NULL)
    pushTriangles(12, t);
  t = create3DLine(x1,y1,z-size,x2,y2,z-size,wid);
  if (t != NULL)
    pushTriangles(12, t);
  t = create3DLine(x2,y2,z-size,x3,y3,z-size,wid);
  if (t != NULL)
    pushTriangles(12, t);
  t = create3DLine(x3,y3,z-size,x1,y1,z-size,wid);
  if (t != NULL)
    pushTriangles(12, t);

  if(z > size) {
    trig(size, x1, y1, z-size, angle, wid);
   trig(size, x2, y2, z-size, angle, wid);
    trig(size, x3, y3, z-size, angle, wid);
  }
}

main (int argc, char *argv[]) {
  FILE *f;
  uint32_t nTri;
  int i;
  triangle *t;
  double angle;
  double wid = 1.0;
  double x, y;
  double sc = 2*3.141216/18;
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filemame.stl>\n", argv[0]);
    exit (1);
  }
  if ((f = fopen(argv[1], "w")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", argv[1]);
    exit (1);
  }
  makePrefix(f);

  trig(4.0, 0.0, 0.0, 20.0, 0.0, 1.0);


  writeAllPushedTriangles(f);
  fclose(f);
}
