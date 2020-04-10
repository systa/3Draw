#include "math.h"
#include "grprimitives.h"
void calculateNormal(double u[], double v[], double nor[]) {
  nor[0] = u[1]*v[2]-v[1]*u[2];
  nor[1] = -(u[0]*v[2]-v[0]*u[2]);
  nor[2] = u[0]*v[1]-v[0]*u[1];
}

void scaleToUnit(double src[], double dst[]) {
  double len = sqrt(src[0]*src[0] + src[1]*src[1] + src[2]*src[2]);
  if (len == 0) // need to so something
    len = 1;
  dst[0] = src[0] / len;
  dst[1] = src[1] / len;
  dst[2] = src[2] / len;
}

void vectorOfVertices(double a[], double b[], double res[]) {
  res[0] = b[0] - a[0];
  res[1] = b[1] - a[1];
  res[2] = b[2] - a[2];
}

