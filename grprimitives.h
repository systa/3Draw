#include "math.h"
// Calculate normal of two vectors. Right-hand approach.
// "nor" is an output parameter, u and for input only.
// Length of all vectors is 3.
void calculateNormal(double u[], double v[], double nor[]);

// Scale a vector to a unit-vector (length = 1)
// "dst" is an output parameter, src input only
// Length of all vectors is 3.
void scaleToUnit(double src[], double dst[]);

// Create a vector from two points.
// "res" is an output parameter, a and b input only.
// Length of all vectors is 3.
void vectorOfVertices(double a[], double b[], double res[]);
