/*
Copyright (C) 2019-2020 by Kari Systä

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/
#include <stdio.h>
#include <stdint.h>
#include <math.h>

int debug = 1;

const int i = 1;
#define is_bigendian() ( (*(char*)&i) == 0 )

typedef struct {
  float normal[3];
  float vertex1[3];
  float vertex2[3];
  float vertex3[3];
  uint16_t attrByteCount;
}  triangle;

void exit(int stat);

void calculateNormal(float u[], float v[], float nor[]) {
  nor[0] = u[1]*v[2]-v[1]*u[2];
  nor[1] = -(u[0]*v[2]-v[0]*u[2]);
  nor[2] = u[0]*v[1]-v[0]*u[1];
}

void scaleToUnit(float src[], float dst[]) {
  float len = sqrt(src[0]*src[0] + src[1]*src[1] + src[2]*src[2]);
  if (len == 0) // need to so something
    len = 1;
  dst[0] = src[0] / len;
  dst[1] = src[1] / len;
  dst[2] = src[2] / len;
}

void vectorOfVertices(float a[], float b[], float res[]) {
  res[0] = b[0] - a[0];
  res[1] = b[1] - a[1];
  res[2] = b[2] - a[2];
}

void skipPrefix(FILE *f) {
  int i;
  for (i=0; i<80; ++i)
    getc(f);
}
uint32_t readUI32(FILE *f) {
  union {
    int32_t n;
    char raw[4];
  } data;
  if (is_bigendian()) {
    if (debug) {
      printf("Run in big-endian computer\n");
    }
    data.raw[3] = getc(f);
    data.raw[2] = getc(f);
    data.raw[1] = getc(f);
    data.raw[0] = getc(f);
  } else {
    if (debug) {
      printf("Run in little-endian computer\n");
    }
    data.raw[0] = getc(f);
    data.raw[1] = getc(f);
    data.raw[2] = getc(f);
    data.raw[3] = getc(f);
  }
  return data.n;
}

uint16_t readUI16(FILE *f) {
  union {
    uint16_t n;
    char raw[2];
  } data;
  if (is_bigendian()) {
    data.raw[1] = getc(f);
    data.raw[0] = getc(f);
  } else {
    data.raw[0] = getc(f);
    data.raw[1] = getc(f);
  }
  return data.n;
}

float readFloat(FILE *f) {
  union {
    float flo;
    char raw[4];
  } data;
  if (is_bigendian()) {
    data.raw[3] = getc(f);
    data.raw[2] = getc(f);
    data.raw[1] = getc(f);
    data.raw[0] = getc(f);
  } else {
    data.raw[0] = getc(f);
    data.raw[1] = getc(f);
    data.raw[2] = getc(f);
    data.raw[3] = getc(f);
  }
  return data.flo;
}

void read3Floats(FILE *f, float flo[]) {
  flo[0] = readFloat(f);
  flo[1] = readFloat(f);
  flo[2] = readFloat(f);
}
triangle readTriangle(FILE *f) {
  triangle tr;
  read3Floats(f, tr.normal);
  read3Floats(f, tr.vertex1);
  read3Floats(f, tr.vertex2);
  read3Floats(f, tr.vertex3);
  tr.attrByteCount = readUI16(f);
  return tr;
}

void printTriangle(triangle tr) {
  float vec1[3],vec2[3],norm[3];
  vectorOfVertices(tr.vertex1, tr.vertex2, vec1);
  vectorOfVertices(tr.vertex1, tr.vertex3, vec2);
  calculateNormal(vec1, vec2, norm);
  scaleToUnit(norm, norm);
  if (debug) {
    printf("vec1: %f %f %f\n", vec1[0], vec1[1], vec1[2]);
    printf("vec2: %f %f %f\n", vec2[0], vec2[1], vec2[2]);
    printf("norm: %f %f %f\n", norm[0], norm[1], norm[2]);
  }
  printf("\nNormal (read - calculated):\n"); 
  printf("  X:%f - %f\n", tr.normal[0], norm[0]); 
  printf("  Y:%f - %f\n", tr.normal[1], norm[1]); 
  printf("  Z:%f - %f\n", tr.normal[2], norm[2]);
  printf("Vertex1:\n"); 
  printf("  X:%f\n", tr.vertex1[0]); 
  printf("  Y:%f\n", tr.vertex1[1]); 
  printf("  Z:%f\n", tr.vertex1[2]); 
  printf("Vertex2:\n"); 
  printf("  X:%f\n", tr.vertex2[0]); 
  printf("  Y:%f\n", tr.vertex2[1]); 
  printf("  Z:%f\n", tr.vertex2[2]); 
  printf("Vertex3:\n"); 
  printf("  X:%f\n", tr.vertex3[0]); 
  printf("  Y:%f\n", tr.vertex3[1]); 
  printf("  Z:%f\n", tr.vertex3[2]);
  printf("Attr byte count: %d\n", (int) tr.attrByteCount);
}
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
