/*
Copyright (C) 2019-2020 by Kari Systä

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/
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
