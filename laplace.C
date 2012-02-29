//
// Parallel evaluation of Laplace's equation
//
// CIS 455/555
// Winter 2102
//

#include <iostream>
using std::cout;
using std::endl;
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define HI_TEMP 100.0
#define LO_TEMP 0.0

void output(double** array, int nx, int ny);

// Global vars, set by command line options

int nx=10;			// number of points, x dimension
int ny=10;			// number of points, y dimension
double eps=1.0;		// epsilon (stopping condition)
int np=1;			// number of processors

int main(int argc, char *argv[]) {
  /*
  nx = opt.getInt("nx",10);
  ny = opt.getInt("ny",10);
  eps = opt.getDouble("eps",1.0);
  np = opt.getInt("np",1);

  // print the values of runtime parameters in a format that
  // can be loaded into R

  cout << "nx = " << nx << ";" << endl;
  cout << "ny = " << ny << ";" << endl;
  cout << "eps = " << eps << ";" << endl;
  cout << "np = " << np << ";" << endl;
  */
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
    *(*g+col_num)=HI_TEMP;
  }
  //initiate bottom rows to LO_TEMP
  for(int row_num=1;row_num<ny;row_num++){
    for(int col_num=0;col_num<nx;col_num++){
      *(*(h+row_num)+col_num)=LO_TEMP;
      *(*(g+row_num)+col_num)=LO_TEMP;
    }
  }

  bool converged=false;

  while(converged==false){
    converged=true;
    #pragma omp parallel for
    for(int row_num=1;row_num<(ny-1);row_num++){//leaving top and bottom rows
      for(int col_num=1;col_num<(nx-1);col_num++){
	int r = row_num;
	int c = col_num;
	double left=*(*(h+r)+(c-1));
        double right=*(*(h+r)+(c+1));
	double up=*(*(h+(r-1))+c);
	double down=*(*(h+(r+1))+c);
	double twin=*(*(h+r)+c);
        #pragma critical
	{
          printf("hello from %i\n",omp_get_thread_num());
	  *(*(g+r)+c)=0.25*(left+right+up+down);
	}
        double self=*(*(g+r)+c);
        if((self*self - twin*twin) > eps){
	  converged=false;
	}
	/*
	printf("r is %i\n",r);
	printf("c is %i\n",c);
	printf("left is %f\n",left);
        printf("right is %f\n",right);
        printf("up is %f\n",up);
        printf("down is %f\n",down);
	printf("twin is %f\n",twin);
	*/
      }
    }
  printf("--------------before--------------------------------------\n");
  printf("g\n");
  output(g,nx,ny);
  printf("h\n");
  output(h,nx,ny);
    double** temp=g;
    g=h;
    h=temp;
  printf("--------------after-------------------------------------\n");
  printf("g\n");
  output(g,nx,ny);
  printf("h\n");
  output(h,nx,ny);
    
  }


  //omp_set_num_threads(4);
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

