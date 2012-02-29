//
// Parallel evaluation of Laplace's equation
//
// CIS 455/555
// Winter 2102
//

#include <iostream>
using std::cout;
using std::endl;

#include <omp.h>

#include "Array.h"
#include "OptionList.h"

#define HI_TEMP 100.0
#define LO_TEMP 0.0

void output(double** array, int nx, int ny);

// Global vars, set by command line options

int nx;			// number of points, x dimension
int ny;			// number of points, y dimension
double eps;		// epsilon (stopping condition)
int np;			// number of processors

int main(int argc, char *argv[]) {
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

  // your code here

  double **g,**h; //we will read from h and operate on g
  g = (double **) malloc(ny*sizeof(double*));
  h = (double **) malloc(ny*sizeof(double*));
  for(int row_num=0;row_num<ny;row_num++){
    *(g+row_num)= (double*) malloc(nx*sizeof(double));
    *(h+row_num)= (double*) malloc(nx*sizeof(double));
  }
  //initiate top row to HI_TEMP
  for(int col_num=0;col_num<nx;col_num++){
    *(*h+col_num)=HI_TEMP;
  }
  //initiate bottom rows to LO_TEMP
  for(int row_num=1;row_num<ny;row_num++){
    for(int col_num=0;col_num<nx;col_num++){
      *(*(h+row_num)+col_num)=LO_TEMP;
    }
  }
  
  output(h,nx,ny);

  omp_set_num_threads(4);
  #pragma omp parallel
  { 
    #pragma omp critical
    {
      cout << "hello, world from " << omp_get_thread_num() << endl;
    }
  }

  for(int row_num=0;row_num<ny;row_num++){
    free(*(g+row_num));
    free(*(h+row_num));
  }
  free(g);
  free(h);
  return 0; 
}

void output(double** array,int nx,int ny){
  for(int row_num=0;row_num<ny;row_num++){
    for(int col_num=0;col_num<nx;col_num++){
      cout << *(*(array+row_num)+col_num) << " ";
    }
    cout << endl;
  }
}

