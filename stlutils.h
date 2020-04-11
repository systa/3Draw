/*
Copyright (C) 2019-2020 by Kari Systä

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/
#include <stdio.h>
#include <stdint.h>

// Data of one triangle. Values are double for accuracy, but
// are stored as float in the STL file
typedef struct {
  double normal[3];
  double vertex1[3];
  double vertex2[3];
  double vertex3[3];
  uint16_t attrByteCount;
}  triangle;


// Read and ignore the 80-byte header of the input (STL) file.
void skipPrefix(FILE *f);

// Make a header to STL file. As far as I know this is usually ignored.
void makePrefix(FILE *f);

// Write n triangles from an array to a STL file
void writeTriangles(FILE *f, uint32_t n, triangle *trs);

// Print content of a triange to console. A debugging tool.
void printTriangle(triangle tr);

/*
 * Creat a new box.
 * Box is defined with 8 corner points. Assume that points 0,1,2,3 (clockwise)
 * define a side facing you. Then point 4 is behind 0, 5 is behing 1 etc.
 * Returns a pointer to an array of 12 triangles. This array has been dynamically
 * allocated from the heap. Deletion - if needed - is reponsibility of caller.
 * Typically, there should not be a need.
 */
triangle *createBox(double points[ ][3] );


/*
 * Create a 3d-line (stick) from point 1 to point 2. The line has "width" of wid. 
 * Returns a pointer to an array of 12 triangles. This array has been dynamically
 * allocated from the heap. Deletion - if needed - is reponsibility of caller.
 * Typically, there should not be a need.
 */
triangle *create3DLine(double x1, double y1, double z1,
		       double x2, double y2, double z2,
		       double wid );

// Push n triangles to an internal stack for later use.
void pushTriangles(uint32_t n, triangle *trs);

// Write all pushed triangles to a file
// Note: these are ALL triangles put to the file.
void writeAllPushedTriangles(FILE *f);

