/*
 * This example is a direct adaptation of code I found from 
 * https://stackoverflow.com/questions/499208/mapping-hilbert-values-to-3d-points
 *
 * The results seem to be quite a challenge to cheap printer I have been using.
 */
#include <stdio.h>
#include <stdlib.h>
#include "stlutils.h"
#define A_LOT (1024*1024)
#define K3 1
double a_lot_of_data[A_LOT];
long len = 0;
void pntAdd(double *pnt, double x) {
  if (len >= A_LOT-1) {
    fprintf(stderr, "Too much data\n");
  } else {
    a_lot_of_data[len] = x;
    ++len;
  }
}

void Hilbert3D(double *pnt,double x,double y,double z,double a,int n) {
  int i,j,m;
  double x0,y0,z0,x1,y1,z1,q;
  for (m=8*3,i=1,j=2;j<=n;j++,i+=i+1)
    m*=8; a/=i; // m = needed size of pnt[]
  printf("m=%d\n", m);
  //  pnt.num=0;
  // init generator
  pntAdd(pnt,x); pntAdd(pnt,y); pntAdd(pnt,z);
  z-=a; pntAdd(pnt,x); pntAdd(pnt,y); pntAdd(pnt,z);
  x+=a; pntAdd(pnt,x); pntAdd(pnt,y); pntAdd(pnt,z);
  z+=a; pntAdd(pnt,x); pntAdd(pnt,y); pntAdd(pnt,z);
  y+=a; pntAdd(pnt,x); pntAdd(pnt,y); pntAdd(pnt,z);
  z-=a; pntAdd(pnt,x); pntAdd(pnt,y); pntAdd(pnt,z);
  x-=a; pntAdd(pnt,x); pntAdd(pnt,y); pntAdd(pnt,z);
  z+=a; pntAdd(pnt,x); pntAdd(pnt,y); pntAdd(pnt,z);
  x0=x+0.5*a; // center of generator
  y0=y-0.5*a;
  z0=z-0.5*a;
  // iterative subdivision
  for (j=2;j<=n;j++) {
    // mirror/rotate qudrants
    x1=x0; y1=y0; z1=z0; m=len;
    for (i=m;i>=3;) {
      i--; z = pnt[K3*i]-z0; //z=pnt.dat[i]-z0;
      i--; y = pnt[K3*i]-y0; //y=pnt.dat[i]-y0;
      i--; x = pnt[K3*i]-x0; // x=pnt.dat[i]-x0;
      q=y; y=-z; z=+q;    // x-
      pnt[K3*(i+0)]=(x1+x);  //      pnt.dat[i+0]=(x1+x);
      pnt[K3*(i+1)]=(y1+y);  //      pnt.dat[i+1]=(y1+y);
      pnt[K3*(i+2)]=(z1-z);  //      pnt.dat[i+2]=(z1-z);
    }
    for (z1-=2.0*a,i=m;i>=3;) {
      i--; z=pnt[K3*i]-z0; // z=pnt.dat[i]-z0;
      i--; y=pnt[K3*i]-y0; // y=pnt.dat[i]-y0;
      i--; x=pnt[K3*i]-x0; // x=pnt.dat[i]-x0;
      q=z; z=+x; x=-q;    // y+
      q=y; y=+z; z=-q;    // x+
      pntAdd(pnt,x1-x);
      pntAdd(pnt,y1+y);
      pntAdd(pnt,z1+z);
    }
    for (x1+=2.0*a,i=m;i>=3;) {
      i--; z=pnt[K3*i]-z0;// z=pnt.dat[i]-z0;
      i--; y=pnt[K3*i]-y0; // y=pnt.dat[i]-y0;
      i--; x=pnt[K3*i]-x0; // x=pnt.dat[i]-x0;
      q=y; y=+z; z=-q;    // x+
      pntAdd(pnt,x1+x);
      pntAdd(pnt,y1+y);
      pntAdd(pnt,z1+z);
    }
    for (z1+=2.0*a,i=m;i>=3;) {
      i--; z=pnt[K3*i]-z0; // z=pnt.dat[i]-z0;
      i--; y=pnt[K3*i]-y0; // y=pnt.dat[i]-y0;
      i--; x=pnt[K3*i]-x0; // x=pnt.dat[i]-x0;
      q=z; z=+x; x=-q;    // y+
      pntAdd(pnt,x1-x);
      pntAdd(pnt,y1-y);
      pntAdd(pnt,z1+z);
    }
    // mirror octants
    x0+=a; y0+=a; z0-=a; m=len;
    for (i=m;i>=3;) {
      i--; z=pnt[K3*i]-z0; // z=pnt.dat[i]-z0;
      i--; y=pnt[K3*i]-y0; // y=pnt.dat[i]-y0;
      i--; x=pnt[K3*i]-x0; // x=pnt.dat[i]-x0;
      pntAdd(pnt,x0+x);
      pntAdd(pnt,y0-y);
      pntAdd(pnt,z0+z);
    }
      a*=2.0;
  }
  /*
        // rotations
        q=z; z=+x; x=-q;    // y+
        q=z; z=-x; x=+q;    // y-
        q=y; y=+z; z=-q;    // x+
        q=y; y=-z; z=+q;    // x-
        q=x; x=+y; y=-q;    // z+
        q=x; x=-y; y=+q;    // z-
  */
}

int onlyOneDiff(double *p) {
  int n = 0;
  if ((*(p-3)-*(p+0)) * (*(p-3)-*(p+0)) > 0.0001)
    ++n;
  if ((*(p-2)-*(p+1)) * (*(p-2)-*(p+1)) > 0.0001)
    ++n;
  if ((*(p-1)-*(p+2)) * (*(p-1)-*(p+2)) > 0.0001)
    ++n;
  return (n == 1);
}

//---------------------------------------------------------------------------
void pnt_draw2(double *pnt) {  // piecewise linear
  int i;
  for (i=3;i<len;i+=3) {
    //    glVertex3dv(pnt.dat+i);
    double *p = pnt+i;
    if (onlyOneDiff(p)) {
      pushTriangles(12, create3DLine(*(p-3), *(p-2), *(p-1), *p, *(p+1), *(p+2), 0.8));
      printf("(%5.2f, %5.2f, %5.2f) -> (%5.2f, %5.2f, %5.2f)\n", *(p-3), *(p-2), *(p-1), *p, *(p+1), *(p+2));
    }
    printf("PTR: %x - %x = %ld\n", (int)p, (int)pnt, ((int)p - (int)pnt) / (sizeof (double *)));
    printf("%d: %5.2f, %5.2f, %5.2f\n", i, p[0], p[1], p[2]);
    
  }
}
void main (int argc, char *argv[]) {
  FILE *f;
  Hilbert3D(a_lot_of_data,-8.0,-8.0,8.0,16.0,3);
  printf("Created %ld points\n", len);
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filemame.stl>\n", argv[0]);
    exit (1);
  }
  if ((f = fopen(argv[1], "w")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", argv[1]);
    exit (1);
  }
  makePrefix(f);
  pnt_draw2(a_lot_of_data);

  writeAllPushedTriangles(f);
  fclose(f);
}


//---------------------------------------------------------------------------
/*
void pnt_draw4(List<double> &pnt)   // piecewise cubic
{
  int i,j;
  double  d1,d2,t,tt,ttt,*p0,*p1,*p2,*p3,a0[3],a1[3],a2[3],a3[3],p[3];
  glBegin(GL_LINE_STRIP);
  for (i=-3;i<len;i+=3) {
    j=i-3; if (j>len-3) j=len-3; if (j<0) j=0; p0=pnt.dat+j;
    j=i  ; if (j>len-3) j=len-3; if (j<0) j=0; p1=pnt.dat+j;
    j=i+3; if (j>len-3) j=len-3; if (j<0) j=0; p2=pnt.dat+j;
    j=i+6; if (j>len-3) j=len-3; if (j<0) j=0; p3=pnt.dat+j;
    for (j=0;j<3;j++) {
      d1=0.5*(p2[j]-p0[j]);
      d2=0.5*(p3[j]-p1[j]);
      a0[j]=p1[j];
      a1[j]=d1;
      a2[j]=(3.0*(p2[j]-p1[j]))-(2.0*d1)-d2;
      a3[j]=d1+d2+(2.0*(-p2[j]+p1[j]));
    }
    for (t=0.0;t<=1.0;t+=0.1) {  // single curve patch/segment
      tt=t*t;
      ttt=tt*t;
      for (j=0;j<3;j++)
	p[j]=a0[j]+(a1[j]*t)+(a2[j]*tt)+(a3[j]*ttt);
      glVertex3dv(p);
    }
  }
  glEnd();
}

*/
