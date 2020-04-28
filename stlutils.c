/*
Copyright (C) 2019-2020 by Kari Systä

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "stlutils.h"
#include "grprimitives.h"

int debug = 0;
#define RANDOM 0
#define HOR_OR_VER 1



// Testing if running on big- or little-endian
const int i = 1;
#define is_bigendian() ( (*(char*)&i) == 0 )


// Read and ignore the 80-byte header of the input (STL) file.
void skipPrefix(FILE *f) {
  int i;
  for (i=0; i<80; ++i)
    getc(f);
}

// Make a header to STL file. As far as I know this us usuallyt ignored.
void makePrefix(FILE *f) {
  int i;
  char text[50];
  char header[80];
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(text, sizeof(text)-1, "%d.%m.%Y %H:%M", t);
  strcpy(header,"Created by Kari's SW ");
  strcat(header,text);
  //  printf("Header: %s\n", header);
  for(i=0; i<strlen(header); ++i) {
    putc(header[i], f);
  } 
  for (i=strlen(header); i<80; ++i)
    putc(' ', f);
}

// Read one 32bit number from file
uint32_t readUI32(FILE *f) {
  union {
    int32_t n;
    char raw[4];
  } data;
  if (is_bigendian()) {
    if (debug) {
      printf("Running on big-endian computer\n");
    }
    data.raw[3] = getc(f);
    data.raw[2] = getc(f);
    data.raw[1] = getc(f);
    data.raw[0] = getc(f);
  } else {
    if (debug) {
      printf("Running on little-endian computer\n");
    }
    data.raw[0] = getc(f);
    data.raw[1] = getc(f);
    data.raw[2] = getc(f);
    data.raw[3] = getc(f);
  }
  return data.n;
}

// Write one 32bit number to file
void writeUI32(FILE *f, uint32_t n) {
  union {
    int32_t n;
    char raw[4];
  } data;
  data.n = n;
  if (is_bigendian()) {
    if (debug) {
      printf("Running on big-endian computer\n");
    }
    putc(data.raw[3], f);
    putc(data.raw[2], f);
    putc(data.raw[1], f);
    putc(data.raw[0], f);
  } else {
    if (debug) {
      printf("Running on little-endian computer\n");
    }
    putc(data.raw[0], f);
    putc(data.raw[1], f);
    putc(data.raw[2], f);
    putc(data.raw[3], f);
  }
}


// Read one 16bit number from file
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

// Write one 32bit number to file
void writeUI16(FILE *f, uint16_t n) {
  union {
    uint16_t n;
    char raw[2];
  } data;
  data.n = n;
  if (is_bigendian()) {
    putc(data.raw[1], f);
    putc(data.raw[0], f);
  } else {
    putc(data.raw[0], f);
    putc(data.raw[1], f);
  }
}

// Read one float from a file
double readFloat(FILE *f) {
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

// Write one float to a file 
void writeFloat(FILE *f, double n) {
  union {
    float flo;
    char raw[4];
  } data;
  data.flo = n;
  if (is_bigendian()) {
    putc(data.raw[3], f);
    putc(data.raw[2], f);
    putc(data.raw[1], f);
    putc(data.raw[0], f);
  } else {
    putc(data.raw[0], f);
    putc(data.raw[1], f);
    putc(data.raw[2], f);
    putc(data.raw[3], f);
  }
}

// Read three floats from file. They are in groups of three 
void read3Floats(FILE *f, double flo[]) {
  flo[0] = readFloat(f);
  flo[1] = readFloat(f);
  flo[2] = readFloat(f);
}

// Write three floats to file. They are in groups of three 
void write3Floats(FILE *f, double flo[]) {
  writeFloat(f, flo[0]);
  writeFloat(f, flo[1]);
  writeFloat(f, flo[2]);
}

// Read information about one triangle
triangle readTriangle(FILE *f) {
  triangle tr;
  read3Floats(f, tr.normal);
  read3Floats(f, tr.vertex1);
  read3Floats(f, tr.vertex2);
  read3Floats(f, tr.vertex3);
  tr.attrByteCount = readUI16(f);
  return tr;
}

// Write n triangles from an array to a STL file
void writeTriangles(FILE *f, uint32_t n, triangle *trs) {
  int i;
  for (i=0; i<n; i++) {
    write3Floats(f, trs[i].normal);
    write3Floats(f, trs[i].vertex1);
    write3Floats(f, trs[i].vertex2);
    write3Floats(f, trs[i].vertex3);
    writeUI16(f, trs[i].attrByteCount);
  }
}

// Print content of a triange to console. A debugging tool.
void printTriangle(triangle tr) {
  double vec1[3],vec2[3],norm[3];
  vectorOfVertices(tr.vertex1, tr.vertex2, vec1);
  vectorOfVertices(tr.vertex1, tr.vertex3, vec2);
  calculateNormal(vec1, vec2, norm);
  scaleToUnit(norm, norm);
  if (debug) {
    printf("vec1: %f %f %f\n", vec1[0], vec1[1], vec1[2]);
    printf("vec2: %f %f %f\n", vec2[0], vec2[1], vec2[2]);
    printf("norm: %f %f %f\n", norm[0], norm[1], norm[2]);
    printf("\nNormal (read / calculated):\n"); 
    printf("  X:%f / %f", tr.normal[0], norm[0]); 
    printf("  Y:%f / %f", tr.normal[1], norm[1]); 
    printf("  Z:%f / %f\n", tr.normal[2], norm[2]);
    printf("Vertex1:"); 
    printf("  X:%f ", tr.vertex1[0]); 
    printf("  Y:%f ", tr.vertex1[1]); 
    printf("  Z:%f\n", tr.vertex1[2]); 
    printf("Vertex2:"); 
    printf("  X:%f ", tr.vertex2[0]); 
    printf("  Y:%f ", tr.vertex2[1]); 
    printf("  Z:%f\n", tr.vertex2[2]); 
    printf("Vertex3:"); 
    printf("  X:%f ", tr.vertex3[0]); 
    printf("  Y:%f ", tr.vertex3[1]); 
    printf("  Z:%f\n", tr.vertex3[2]);
    printf("Attr byte count: %d\n", (int) tr.attrByteCount);
  }
}

// Copy a vector
void copyVec(int n, double v1[], double v2[]) {
  int i;
  for (i=0; i<n; ++i) {
    v2[i] = v1[i];
  }
}



// Calculate unit normal vector or a triagle
// "norm" is a an out-parameter
void normalOfTriangle(double p0[], double p1[], double p2[], double norm[]) {
  double vec1[3];
  double vec2[3];
  vectorOfVertices(p0, p1, vec1);
  vectorOfVertices(p0, p2, vec2);
  calculateNormal(vec1, vec2, norm);
  scaleToUnit(norm, norm);
}

// Initialize a triangle pointed by "tr".
void fillTriangle(triangle *tr, double p1[], double p2[], double p3[],
		  double norm[]) {
  int i;
  for (i=0; i<3; ++i) {
    tr->normal[i] = norm[i];
    tr->vertex1[i] = p1[i];
    tr->vertex2[i] = p2[i];
    tr->vertex3[i] = p3[i];
  }
  tr->attrByteCount = 0;
}  

/*
 * Create a rotation matrix around unit vector
 * Should be IdentityMatrix + C*sin(a) + C^2(1-cos(a)
 */
void createRotationMatrix(double uv[], double angle, double result [][3]) {
  double C[3][3];
  double temp[3][3];
  double s = sin(angle);
  double c = cos(angle);
  int i,j,k;
  C[0][0]=0; C[0][1]=0-uv[2]; C[0][2]=uv[1];
  C[1][0]=uv[2]; C[1][1]=0; C[1][2]=0-uv[0];
  C[2][0]=0-uv[1]; C[2][1]=uv[0]; C[2][2]=0;
  for (i=0; i<3; ++i) {
    for (j=0; j<3; ++j) {
      result[i][j] = (i==j)?1.0:0.0;
    }
  }
  for (i=0; i<3; ++i) {
    for (j=0; j<3; ++j) {
      result[i][j] += C[i][j]*s;
    }
  }
  // Next temp = C^2;
  for (i=0; i<3; ++i) {
    for (j=0; j<3; ++j) {
      temp[i][j] = 0.0;
      for (k=0; k<3; ++k) {
	temp[i][j] += C[i][k]*C[k][j];
      }
    }
  }
  for (i=0; i<3; ++i) {
    for (j=0; j<3; ++j) {
      result[i][j] += temp[i][j]*(1-c);
    }
  }
}

void rotateAround(double uv[], double angle, double target[]) {
  double M[3][3];
  double temp[3];
  int i,j;
  copyVec(3, target, temp);
  createRotationMatrix(uv, angle, M);
  for (i=0; i<3; ++i) {
    target[i] = 0.0;
    for (j=0; j<3; ++j) {
      target[i] += M[i][j]*temp[j];
    }
  }
}

/*
 * Box defined with 8 corner points. Assume that points 0,1,2,3 (clockwise)
 * define a side facing you a. Then point 4 is behind 0, 5 is behing 1 etc
 */
triangle *createBox(double points[][3] ) {
  int i;
  triangle *triangles = malloc (12*(sizeof(triangle)));
  double norm[3];
  if (debug) {
    printf("points:\n");
    for (i=0; i<8; ++i) {
      printf(" - %f %f %f\n", points[i][0], points[i][1], points[i][2]);
    }
    /*********/normalOfTriangle(points[0], points[3], points[1], norm);
    printf("First normal: %f %f %f\n", norm[0], norm[1], norm[2]);
  }
  fillTriangle(&triangles[0], points[0], points[3], points[1], norm);
  /*********/normalOfTriangle(points[2], points[1], points[3], norm);
  fillTriangle(&triangles[1], points[2], points[1], points[3], norm);
  
  /*********/normalOfTriangle(points[4], points[5], points[7], norm);
  fillTriangle(&triangles[2], points[4], points[5], points[7], norm);
  /*********/normalOfTriangle(points[6], points[7], points[5], norm);
  fillTriangle(&triangles[3], points[6], points[7], points[5], norm);

  /*********/normalOfTriangle(points[4], points[0], points[5], norm);
  fillTriangle(&triangles[4], points[4], points[0], points[5], norm);
  /*********/normalOfTriangle(points[1], points[5], points[0], norm);
  fillTriangle(&triangles[5], points[1], points[5], points[0], norm);

  /*********/normalOfTriangle(points[7], points[6], points[3], norm);
  fillTriangle(&triangles[6], points[7], points[6], points[3], norm);
  /*********/normalOfTriangle(points[2], points[3], points[6], norm);
  fillTriangle(&triangles[7], points[2], points[3], points[6], norm);

  /*********/normalOfTriangle(points[0], points[4], points[3], norm);
  fillTriangle(&triangles[8], points[0], points[4], points[3], norm);
  /*********/normalOfTriangle(points[7], points[3], points[4], norm);
  fillTriangle(&triangles[9], points[7], points[3], points[4], norm);

  /**********/normalOfTriangle(points[2], points[6], points[1], norm);
  fillTriangle(&triangles[10], points[2], points[6], points[1], norm);
  /**********/normalOfTriangle(points[5], points[1], points[6], norm);
  fillTriangle(&triangles[11], points[5], points[1], points[6], norm);

  if (debug) {
    printTriangle(triangles[0]);
    printTriangle(triangles[1]);
    printTriangle(triangles[2]);
    printTriangle(triangles[3]);
  }
  return triangles;
}


// Implementation of the line
triangle *createStick(double points[ ][3], double wid, endtype endStrategy ) {
  double vec0[3], vec0Unit[3], vecTmp[3], vecPer1[3], vecPer2[3], tmp;
  double uv[3];
  static double angle=0.0;
  double box[8][3];
  int i;
  // First we create a vector from one end of stick to the one end
  vectorOfVertices(points[0], points[1], vec0);

  // Then we have challege to create the faces (squares) at both ends.

  if (endStrategy == RANDOM) {
    // The first option is bit random way
    /*
     * A home cooked way to find perpendicular vector. First find a different
     * vector vecTmp and then a normal to vec0 and vecTmp. Then a normal (vecPer1)
     * to vec0 and vecTmp and finally normal to vec0 and vecPer1.
     */
    copyVec(3, vec0, vecTmp);
    if (vecTmp[0] != vecTmp[1]) {
      tmp = vecTmp[0];
      vecTmp[0] = vecTmp[1];
      vecTmp[1] = tmp;
    } else if (vecTmp[0] != vecTmp[2]) {
      tmp = vecTmp[0];
      vecTmp[0] = vecTmp[2];
      vecTmp[2] = tmp;
    } else if (vecTmp[2] != vecTmp[1]) {
      tmp = vecTmp[2];
    vecTmp[2] = vecTmp[1];
    vecTmp[1] = tmp;
    } else {
      return NULL;
    }
    calculateNormal(vec0, vecTmp, vecPer1);
    calculateNormal(vec0, vecPer1, vecPer2);
    if (debug) {
      printf("Vec : %5.2f, %5.2f, %5.2f\n", vecPer1[0], vecPer1[1], vecPer1[2]);
    }
    scaleToUnit(vec0,uv);

    // UGLY!!! I need to go back to my math!! I guess I should start by finding plane at end of line
    if (vec0[2]*vec0[2] > vec0[0]*vec0[0] &&
	vec0[2]*vec0[2] > vec0[1]*vec0[1]) {  // close to vertical => end in x-y plane
      double step = (vecPer1[1]<0) ? 0.1 : -0.1;
      int dir = (vecPer1[1]<0) ? 1 : -1;
      while (vecPer1[1]*vecPer1[1] > 0.00001) {
	rotateAround(uv, step, vecPer1);
	rotateAround(uv, step, vecPer2);
	if (((vecPer1[1]<0) ? 1 : -1) != dir) {
	  dir = -dir;
	  step /= 10;
	}
      }
    } else if (vec0[0]*vec0[0] > vec0[2]*vec0[2] &&
	       vec0[0]*vec0[0] > vec0[1]*vec0[1]) {  // close to X-axis => y-z plane
      double step = (vecPer1[1]<0) ? 0.1 : -0.1;
      int dir = (vecPer1[1]<0) ? 1 : -1;
      while (vecPer1[1]*vecPer1[1] > 0.00001) {
	rotateAround(uv, step, vecPer1);
	rotateAround(uv, step, vecPer2);
	if (((vecPer1[1]<0) ? 1 : -1) != dir) {
	  dir = -dir;
	  step /= 10;
	}
      }
    } else  {  // close to Y-axis => x-z plane
      double step = (vecPer1[0]<0) ? 0.1 : -0.1;
      int dir = (vecPer1[0]<0) ? 1 : -1;
      while (vecPer1[0]*vecPer1[0] > 0.00001) {
	rotateAround(uv, step, vecPer1);
	rotateAround(uv, step, vecPer2);
	if (((vecPer1[0]<0) ? 1 : -1) != dir) {
	  dir = -dir;
	  step /= 10;
	}
      }
    }
    /*
    rotateAround(uv, angle, vecPer1);
    rotateAround(uv, angle, vecPer2);
    angle += 0.2;
    */
    printf("Vec': %5.2f, %5.2f, %5.2f\n", vecPer1[0], vecPer1[1], vecPer1[2]);
  } else {  // endStrategy == HOR_OR_VER
    printf("HOR_OR_VEC\n");
    // Depending on position
    if (vec0[2]*vec0[2] > vec0[0]*vec0[0] &&
	vec0[2]*vec0[2] > vec0[1]*vec0[1]) {  // close to vertical => end in x-y plane
      vecPer1[0] = 1.0;
      vecPer1[1] = 0.0;
      vecPer1[2] = 0.0;
      vecPer2[0] = 0.0;
      vecPer2[1] = 1.0;
      vecPer2[2] = 0.0;    
    } else if (vec0[0]*vec0[0] > vec0[2]*vec0[2] &&
	       vec0[0]*vec0[0] > vec0[1]*vec0[1]) {  // close to X-axis => y-z plane
      vecPer1[0] = 0.0;
      vecPer1[1] = 1.0;
      vecPer1[2] = 0.0;
      vecPer2[0] = 0.0;
      vecPer2[1] = 0.0;
      vecPer2[2] = 1.0;    
    } else  {  // close to Y-axis => x-z plane
      vecPer1[0] = 1.0;
      vecPer1[1] = 0.0;
      vecPer1[2] = 0.0;
      vecPer2[0] = 0.0;
      vecPer2[1] = 0.0;
      vecPer2[2] = 1.0;
    }
  }
  scaleToUnit(vecPer1, vecPer1);
  scaleToUnit(vecPer2, vecPer2);
  

  /*
   * Continue with wid/2 at both ends.
   * WAS A BAD IDEA.
  wid *= 0.5;
  scaleToUnit(vec0, vec0Unit);
  for (i=0; i<3; ++i) {
    points[0][i] -= vec0Unit[i]*wid;
    points[0][i] -= vec0Unit[i]*wid;
    points[0][i] -= vec0Unit[i]*wid;

    points[1][i] += vec0Unit[i]*wid;
    points[1][i] += vec0Unit[i]*wid;
    points[1][i] += vec0Unit[i]*wid;
  }  
  */

  /* Now, with these perpendicular vectors we make a box */
  /*
  for (i=0; i<3; ++i) {
    box[0][i] = points[0][i] + wid * vecPer1[i];   
    box[1][i] = points[0][i] + wid * vecPer2[i];   
    box[2][i] = points[0][i] - wid * vecPer1[i];   
    box[3][i] = points[0][i] - wid * vecPer2[i];   

    box[4][i] = points[1][i] + wid * vecPer1[i];   
    box[5][i] = points[1][i] + wid * vecPer2[i];   
    box[6][i] = points[1][i] - wid * vecPer1[i];   
    box[7][i] = points[1][i] - wid * vecPer2[i];   
  }
  */
  for (i=0; i<3; ++i) {
    box[0][i] = points[0][i] + wid*vecPer1[i] + wid*vecPer2[i];   
    box[1][i] = points[0][i] + wid*vecPer1[i] - wid*vecPer2[i];   
    box[2][i] = points[0][i] - wid*vecPer1[i] - wid*vecPer2[i];   
    box[3][i] = points[0][i] - wid*vecPer1[i] + wid*vecPer2[i];   

    box[4][i] = points[1][i] + wid*vecPer1[i] + wid*vecPer2[i];   
    box[5][i] = points[1][i] + wid*vecPer1[i] - wid*vecPer2[i];   
    box[6][i] = points[1][i] - wid*vecPer1[i] - wid*vecPer2[i];   
    box[7][i] = points[1][i] - wid*vecPer1[i] + wid*vecPer2[i];   
  }
  return createBox(box);
}


/*
 * Create a line from point 1 to point 2. The line has "width" of wid. 
 * OLD version
 */
triangle *create3DLine(double x1, double y1, double z1,
		       double x2, double y2, double z2,
		       double wid ) {
  double data[2][3];
  data[0][0] = x1;
  data[0][1] = y1;
  data[0][2] = z1;
  data[1][0] = x2;
  data[1][1] = y2;
  data[1][2] = z2;
  return createStick(data, wid, RANDOM);
}

/*
 * Create a line from point 1 to point 2. The line has "width" of wid. 
 * NEW version
 */
triangle *createLine(double x1, double y1, double z1,
		       double x2, double y2, double z2,
		     double wid, endtype endStrategy ) {
  double data[2][3];
  data[0][0] = x1;
  data[0][1] = y1;
  data[0][2] = z1;
  data[1][0] = x2;
  data[1][1] = y2;
  data[1][2] = z2;
  return createStick(data, wid, endStrategy);
}

uint32_t nPushed = 0;

struct pushedBlock {
  struct pushedBlock *next;
  uint32_t n;
  triangle *trs;
} *pushedRoot = NULL;

void pushTriangles(uint32_t n, triangle *trs) {
  if (trs == NULL) {
    if (debug) {
      printf("Tried to push NULL, call skippef\n");
    }
    return;
  }
  if (debug) {
    printf("Pushing %d triangles to %d\n", n,nPushed);
  }
  int i;
  struct pushedBlock *block = malloc(sizeof(struct pushedBlock));
  if (block == NULL) {
    fprintf(stderr, "pushTriagles: malloc1() failed\n");
    return;
  }
  block->trs = malloc(n*sizeof(triangle));
  if (block->trs == NULL) {
    fprintf(stderr, "pushTriagles: malloc1() failed\n");
    return;
  }
  if (debug) {
    printf("pushTriangles: allocated\n");
  }
  nPushed += n;
  block->next = pushedRoot;
  block->n = n;
  pushedRoot = block;
  if (debug) {
    printf("pushTriangles: chained\n");
  }
  for (i=0; i<n; ++i) {
    block->trs[i] = trs[i];
  }
  if (debug) {
    printf("pushTriangles: data copied\n");
  }

} 


// Move along Z-axis so that mininum Z is 0.0
void movePushedTrianglesToZeroLevel() {
  struct pushedBlock *block = pushedRoot;
  double min = 20e10, thisZ;
  int i,j;
  //Phase 1: calculate min
  while (block != NULL) {
    for (i=0; i<block->n; ++i) {
      thisZ = block->trs[i].vertex1[2];
      if (thisZ < min) min = thisZ;
      thisZ = block->trs[i].vertex2[2];
      if (thisZ < min) min = thisZ;
      thisZ = block->trs[i].vertex3[2];
      if (thisZ < min) min = thisZ;
    }
    block = block->next;
  }
  if (debug) {
    printf("Min=%6.2f\n", min);
  }
  //Phase 2: move
  block = pushedRoot;
  while (block != NULL) {
    for (i=0; i<block->n; ++i) {
      block->trs[i].vertex1[2] -= min;
      block->trs[i].vertex2[2] -= min;
      block->trs[i].vertex3[2] -= min;
     }
    block = block->next;
  }
}


void writeAllPushedTriangles(FILE *f) {
  struct pushedBlock *block = pushedRoot;
  writeUI32(f, nPushed);
  while (block != NULL) {
    if (debug) {
       printf("Writing %d triangles\n", block->n);
    }
    writeTriangles(f, block->n, block->trs);
    block = block->next;
  }
  // TODO free memory.
}
