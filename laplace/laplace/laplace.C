//
// Parallel evaluation of Laplace's equation
//
// CIS 455/555
// Winter 2102
//

#include <iostream>
using std::cout;
using std::endl;

#include "Array.h"
#include "OptionList.h"

// Global vars, set by command line options

int nx;			// number of points, x dimension
int ny;			// number of points, y dimension
double eps;			// epsilon (stopping condition)
int np;			// number of processors

void init(int argc, char *argv[]) {
  OptionList opt(argc,argv);

  nx = opt.getInt("nx",200);
  ny = opt.getInt("ny",200);
  eps = opt.getDouble("eps",1.0);
  np = opt.getInt("np",1);

  // print the values of runtime parameters in a format that
  // can be loaded into R

  cout << "nx = " << nx << ";" << endl;
  cout << "ny = " << ny << ";" << endl;
  cout << "eps = " << eps << ";" << endl;
  cout << "np = " << np << ";" << endl;
}

int main(int argc, char *argv[]) {
  init(argc,argv);

  // your code here

  return 0;
}

