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

int main(int argc, char *argv[])
{
  //defaults
  int nx    =330;		// number of points, x dimension
  int ny    =70;		// number of points, y dimension
  double eps=0.05;		// epsilon (stopping condition)
  int fx    =30;			// fin width (x dimension)
  int fy    =55;		// fin length (y dimension)
  int fg    =30;			// gap between adjacent fins

  //parsing args
  for(int i=0;i<argc;i++){
    if(0==strcmp(argv[i],"-nx")){
      nx=atoi(argv[i+1]);
    }
    else if(0==strcmp(argv[i],"-ny")){
      ny=atoi(argv[i+1]);
    }
    else if(0==strcmp(argv[i],"-eps")){
      eps=(double) atof(argv[i+1]);
    }
    else if(0==strcmp(argv[i],"-fx")){
      fx=atoi(argv[i+1]);
    }
    else if(0==strcmp(argv[i],"-fy")){
      fy=atoi(argv[i+1]);
    }
    else if(0==strcmp(argv[i],"-fg")){
      fg=atoi(argv[i+1]);
    }
  }

  int fx_total   = fx+fg;
  int base_height= ny-fy;
  int num_fins   = nx/fx_total;

  if(num_fins % 2 != 0){
    num_fins++; //place another fin on the base ... if it will fit
  }

  /*
  printf("nx is %i\n",nx);
  printf("ny is %i\n",ny);
  printf("eps is %f\n",eps);
  printf("fx is %i\n",fx);
  printf("fy is %i\n",fy);
  printf("fg is %i\n",fg);
  printf("fx_total is %i\n",fx_total);
  printf("num_fins is %i\n",num_fins);
  printf("base_height is %i\n",base_height);
  */

  double **g,**h; //we will read from h and operate on g
  g = (double **) malloc(ny*sizeof(double*));
  h = (double **) malloc(ny*sizeof(double*));
  for(int row_num=0;row_num<ny;row_num++){
    *(g+row_num)= (double*) malloc(nx*sizeof(double));
    *(h+row_num)= (double*) malloc(nx*sizeof(double));
  }
  //initiate base to HI_TEMP
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
    //calculating base
    #pragma omp parallel for
      for(int row_num=1;row_num<base_height;row_num++){
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
	    *(*(g+r)+c)=0.25*(left+right+up+down);
	  }
          double self=*(*(g+r)+c);
          if((self*self - twin*twin) > eps){
	    #pragma critical
	    {
	      converged=false;
	    }
	  }
        }
      }
    //calculating fins
    for(int fin=0;fin<num_fins;fin++){
      #pragma omp parallel for
      for(int row_num=base_height;row_num<(ny-1);row_num++){
        for(int col_num=fx_total*fin+1;col_num<(fx_total*fin+fx-1);col_num++){
	  int r = row_num;
	  int c = col_num;
	  double left=*(*(h+r)+(c-1));
          double right=*(*(h+r)+(c+1));
	  double up=*(*(h+(r-1))+c);
	  double down=*(*(h+(r+1))+c);
	  double twin=*(*(h+r)+c);
          #pragma critical
	  {
	    *(*(g+r)+c)=0.25*(left+right+up+down);
	  }
          double self=*(*(g+r)+c);
          if((self*self - twin*twin) > eps){
	    #pragma critical
            {
	      converged=false;
	    }
	  }
        }
      }
    }
    double** temp=g;
    g=h;
    h=temp;
  }
  
  output(g,nx,ny);
  
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
      cout << row_num << "," << col_num << "," << *(*(array+row_num)+col_num) << endl; //" ";
    }
    //cout << endl;
  }
}

