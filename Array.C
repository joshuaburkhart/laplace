//							
// Array class -- Fortran style arrays in C++		
//							

#include <iostream>
using std::cout;
using std::endl;
using std::hex;
using std::dec;

#include <string>

#include "Array.h"

// Default constructor -- an unitialized array will have no
// storage allocated, and the pointer to the storage area
// should be NULL.

Array::Array() {
  a = NULL;
  d_r = d_c = d_p = 0;
  d_name = NULL;
}

// The "copy constructor" makes a complete copy of an existing
// array.

Array::Array(const Array &A) {
  copy(A);
}

// The following three constructors will be invoked to create
// actual arrays.  The number of integer arguments determines
// the dimension of the array, e.g. Array(3,4) will create a
// 3 x 4 2D array.  The last parameter is an optional string
// that will be used as an array name and printed by the
// output routines.

Array::Array(int r, char *name) {
  d_r = r;
  d_c = d_p = 1;
  d_name = name;
  a = new double[r];
  zero();
}

Array::Array(int r, int c, char *name) {
  d_r = r;
  d_c = c;
  d_p = 1;
  d_name = name;
  a = new double[r*c];
  zero();
}

Array::Array(int r, int c, int p, char *name) {
  d_r = r;
  d_c = c;
  d_p = p;
  d_name = name;
  a = new double[r*c*p];
  zero();
}

// The destructor has to deallocate the cells used by the
// array if it was initialized:

Array::~Array() {
  if (a)
    delete a;
}

// The assignment operator does a "deep copy" -- it makes
// an exact copy of the state information (number of rows,
// name, etc) but allocates a new storage area.  The storage
// of the new array will be copied over from the existing one.

Array& Array::operator= (const Array &A) {
  copy(A);
  return *this;
}

// Set every cell in the array to 0.0.  Note: this is semi-dangerous;
// it assumes the representation of 0.0 as a double precision floating
// point number is a string of 0 bits.  True for IEEE 754,
// but this may not be portable.......

void Array::zero() {
  int n = d_r * d_c * d_p;	

  if (n)
    memset(a,0,n*sizeof(double));
}

// Zero out a rectangular submatrix.  The upper left cell in the
// cleared region is at (i,j), the width of the region is nc
// (number of columns), and the height is nr (number of rows).

void Array::zero(int i, int j, int nr, int nc) {
  int ii, jj;

  for (ii = i; ii < i+nr; ii++)
    for (jj = j; jj < j+nc; jj++)
      a[jj*d_r+ii] = 0.0;
}

// Special case of clearing a submatrix: set a contiguous group
// of columns to zero:

void Array::zero_col(int j, int nc) {
  memset(a+j*d_r,0,nc*d_r*sizeof(double));
}

// Output routines.
//
// (1) The dump() member function will print the array contents
//     only, one element per line, in the order they are stored
//     in memory (mainly for debugging only, and to check how
//     elements are blocked).
// (2) The plot() member function will print the array in a format
//     that will allow it to be plotted with GNUPLOT (printing
//     only 25 elements in each dimension)
// (3) The overloaded << operator will print every element in a
//     2D grid format, suitable for loading into Matlab

void Array::dump() {
  int i;

  if (d_name)
    cout << d_name << " @ " << hex << (void *)a << dec << endl;
  for (i = 0; i < d_r*d_c*d_p; i++)
    cout << a[i] << endl;
}

void Array::plot() {
  int i, j;
  int d = (d_r > d_c) ? d_r/25 : d_c/25;

  if (d_name)
    cout << "# " << d_name << ":" << endl;

  for (i = 0; i < d_r; i += d) {
    for (j = 0; j < d_c; j += d)
      cout << i << " " << j << " " << (*this)(i,j) << endl;
    cout << endl;
  }
}

ostream &operator <<(ostream &s, Array &A) {
  int i, j;

  for (i = 0; i < A.d_r; i++) {
    for (j = 0; j < A.d_c; j++)
      s << A(i,j) << " ";
    s << endl;
  }

  return s;
}


// Swap arrays x and y (which must be the same size and shape)

void exchange(Array &x, Array &y) {
  double *t = x.a;
  x.a = y.a;
  y.a = t;
}

// Private function for making a copy of an array; copies
// dimensions and contents, but not the name.  It should
// really also check to make sure the dimensions are the
// same before copying....

void Array::copy(const Array &A) {
  if (a) {			// recycle old contents
    delete a;
  }

  d_r = A.d_r;
  d_c = A.d_c;
  d_p = A.d_p;

  int n = d_r * d_c * d_p;	

  if (n) {
    a = new double[n];
    memcpy(a,A.a,n*sizeof(double));
  }
  else				// copying an unitialized
    a = NULL;			// array -- no contents
}

