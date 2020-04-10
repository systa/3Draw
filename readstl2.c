#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "stlutils.h"

void exit(int code);

main (int argc, char *argv[]) {
  FILE *f;
  uint32_t nTri;
  int i;
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filemame.stl>\n", argv[0]);
    exit (1);
  }
  if ((f = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", argv[1]);
    exit (1);
  }
  skipPrefix(f);
  nTri = readUI32(f);
  printf("Got %d triangles\n", (int)nTri);
  for (i=0;i<nTri;++i) {
    printTriangle(readTriangle(f));
  }
}
