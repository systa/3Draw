/*
Copyright (C) 2019-2020 by Kari Systä

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "stlutils.h"

void exit(int code);


int main (int argc, char *argv[]) {
  FILE *f;
  int i;
  triangle *t;
  double len = 10;
  int nLayers = 10;
  float wid = 1.0;
  double start1[3], start2[3], start3[3];
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filemame.stl>\n", argv[0]);
    exit (1);
  }
  if ((f = fopen(argv[1], "w")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", argv[1]);
    exit (1);
  }
  makePrefix(f);

  start1[0] = 0.0;
  start1[1] = 0.0;
  start1[2] = 0.0;
  start2[0] = len;
  start2[1] = 0.0;
  start2[2] = 0.0;
  start3[0] = len*cos(3.14121/3);
  start3[1] = len*sin(3.14121/3);
  start3[2] = 0.0;
  pushTriangles(12, create3DLine(start1[0], start1[1], start1[2],
				 start1[0], start1[1], start1[2]+len*nLayers,
				 wid));
  pushTriangles(12, create3DLine(start2[0], start2[1], start2[2],
				 start2[0], start2[1], start2[2]+len*nLayers,
				 wid));
  pushTriangles(12, create3DLine(start3[0], start3[1], start3[2],
				 start3[0], start3[1], start3[2]+len*nLayers,
				 wid));

  for (i=0; i<=len; ++i) {
    pushTriangles(12, create3DLine(start1[0], start1[1], start1[2]+len*i,
				   start2[0], start2[1], start2[2]+len*i,
				   wid));
    
    pushTriangles(12, create3DLine(start1[0], start1[1], start1[2]+len*i,
				   start3[0], start3[1], start3[2]+len*i,
				   wid));
    
    pushTriangles(12, create3DLine(start3[0], start3[1], start3[2]+len*i,
				   start2[0], start2[1], start2[2]+len*i,
				   wid));    
  }
  writeAllPushedTriangles(f);
  fclose(f);
  return 0;
}
