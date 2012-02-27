//
// Array class -- Fortran style arrays in C++.
//
// Constructors allow programs to create 1-, 2-, or 3D arrays.
//
// An N x M 2D array will occupy N*M contiguous words in memory,
// laid out in column major order so the array can be passed to
// Fortran procedures (including the highly optimized BLAS
// routines provided by SGI).  An L x M x N 3D array is organized
// similarly.
//

#ifndef _Array_h_
#define _Array_h_

#include <iostream>
using std::ostream;

class Array {
public:

  // Constructors.  The default constructor is invoked when a
  // variable of type Array is declared but not initialized.
  // The copy constructor is needed because some functions
  // return arrays.  The optional parameter in the other 
  // constructors assigns a name that is printed along with 
  // the array contents.

  Array();
  Array(const Array&);
  Array(int r, char *name = NULL);
  Array(int r, int c, char *name = NULL);
  Array(int r, int c, int p, char *name = NULL);

  // Destructor -- necessary to deallocate storage used for 
  // array elements

  ~Array();

  // Assignment operator -- this will make a "deep copy" of
  // the source array, i.e. the two arrays will have separate
  // storage areas and separate elements.

  Array& operator= (const Array&);

  // Array reference operators.  Alas, we have to use the 
  // FORTRAN style "round brackets" syntax for higher dimension
  // arrays -- there is no binary or ternary [] in C++.  All
  // inlined to make them as efficient as possible.

  double &operator ()(int i) {
    return a[i];
  };

  double &operator ()(int i, int j) {
    return a[j*d_r + i];
  }

  double &operator ()(int i, int j, int k) {
    return a[(k*d_c + j)*d_r + i];
  }

  // Return the size of an array.  With one argument, return
  // the number of elements, assuming the array is one long
  // vector.  With two arguments, return the number of rows
  // and columns; with three args, return the number of rows, 
  // columns, and planes.  Note that all the size functions
  // will return a valid value no matter how many dimensions
  // dimensions were used to create the array -- e.g. calling
  // A.size(&x) where A was defined as A(5,10) will set x to
  // 50 -- the number of elements in A when A is viewed as
  // a single array.  B.size(&x,&y) where B is defined as B(16)
  // will set x to 16 and y to 1.

  void size(int &rows) {
    rows = d_r * d_c * d_p;
  }

  void size(int &rows, int &cols) {
    rows = d_r;
    cols = d_c * d_p;
  }

  void size(int &rows, int &cols, int &planes) {
    rows = d_r;
    cols = d_c;
    planes = d_p;
  }

  // Useful member functions.  zero() will set all array 
  // elements to 0.0 (see note in the implementation file);
  // zero(i,j,nr.nc) will zero out a submatrix;  zero_col(j,nc)
  // will clear entire columns from j to j+nc-1; dump(), 
  // plot(), and operator<< will print a representation
  // of the array.

  void zero();
  void zero(int i, int j, int nr, int nc);
  void zero_col(int j, int nc);
  void dump();
  void plot();
  friend ostream &operator <<(ostream &s, Array &x);

  // Friend functions.  exchange() swaps the elements in the
  // arrays simply by changing the pointers to the underlying
  // storage areas.

  friend void exchange(Array &x, Array &y);

private:
  // The state variables of an array are the sizes of the
  // dimensions, a pointer to the storage area, and the name
  // (optional).  For a 1D array, the Y and Z dimensions will
  // be 1; for a 2D array, the Z dimension will be 1.

  double *a;
  int d_r;
  int d_c;
  int d_p;
  char *d_name;

  // This function is shared by the copy constructor and the
  // assignment operator:

  void copy(const Array &);
};

#endif
