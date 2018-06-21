
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

int main(void)
{
  printf("heat-tiling : a project to explore tiling optimizations of 2D and 3D update \n");
  printf("algorithms which couple spatially neighboring points \n");

  //declare a large array
  int dimx = 100;
  int dimy = 100;
  int dim = dimx * dimy;
  float *temp;
  temp = (float*)calloc(dim, sizeof(float));
  
  //iterate for n time steps 
  for (int n = 0; n < steps; n++)
    {

      for (int itile = 0; itile < num_tiles; itile++)
	{
	  
	  //perform local blending kernel
	  
	}

      //reset the boundary conditions/heater cells
      for (int is = 0; is < dim; is++)
	{
	  
	}
    }

  free(temp);

  return 0;

}
