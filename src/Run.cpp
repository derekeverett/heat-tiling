
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#ifdef _OPENMP
#include <omp.h>
#endif

int main(void)
{
  printf("####################################################################\n");
  printf("heat-tiling : a project to explore tiling optimizations of 2D\n");
  printf("and 3D update algorithms which couple spatially neighboring points \n");
  printf("####################################################################\n");

  //declare a large array
  int steps = 100; //number of time steps
  int dimx = 256;
  int dimy = 256;
  int dimz = 128;
  float k = 1.0; //heat diffusion strength
  int dim = dimx * dimy * dimz;
  float *temp_old, *temp_new;
  temp_old = (float*)calloc(dim, sizeof(float)); //previous temperature grid
  temp_new = (float*)calloc(dim, sizeof(float)); //updated temperature grid

  //the strides in x,y,z given is = (dimz * dimy) * ix + (dimz) * iy + iz;
  int xstride = dimy * dimz;
  int ystride = dimz;
  int zstride = 1;

  //set the initial conditions, all cells at zero temp except a single heater cell at center
  temp_old[ dim / 2 ] = 1.0;

  for (int power = 0; power < 24; power++)
  {
    int num_tiles = pow(2, power);
    //printf("num_tiles : %d \n", num_tiles);
    //clock
    double sec = 0.0;
    #ifdef _OPENMP
    sec = omp_get_wtime();
    #endif

    //get the tilesize
    int tilesize = dim / num_tiles;

    //iterate for n time steps
    for (int n = 0; n < steps; n++)
    {
      //iterate over each tile
      for (int itile = 0; itile < num_tiles; itile++)
      {
        //printf("itile : %d \n", itile);
        //perform local blending kernel on each tile , is_loc is local index within tile
        for (int is_loc = 0; is_loc < tilesize; is_loc++)
        {
          //printf("is_loc : %d \n", is_loc);
          int is_glb = is_loc + itile * tilesize; //the global index across all tiles
          int xm = is_glb - xstride;
          int xp = is_glb + xstride;
          int ym = is_glb - ystride;
          int yp = is_glb + ystride;
          int zm = is_glb - zstride;
          int zp = is_glb + zstride;

          if (xm < 0) xm += xstride;
          if (xp > dimx - 1) xp -= xstride;
          if (ym < 0) ym += ystride;
          if (yp > dimy - 1) yp -= ystride;
          if (zm < 0) zm += zstride;
          if (zp > dimz - 1) zp -= zstride;
          //blending step
          temp_new[is_glb] = temp_old[is_glb] + k * ( temp_old[xm] + temp_old[xp] + temp_old[ym] + temp_old[yp] + temp_old[zm] + temp_old[zp] - 6.0 * temp_old[is_glb] );
        }
      }

      //reset the boundary conditions/heater cells
      //heater cell in middle of grid
      temp_new[ dim / 2 ] = 1.0;

      //heat sinks at boundary
      for (int is = 0; is < dim; is++)
      {
        //int is = (dimy) * ix + iy;
        int ix = is / (dimy * dimz);
        int iy = ( is - (dimy * dimz * ix) )/ dimz;
        int iz = is - (dimy * dimz * ix) - dimz * iy;
        if (ix == 0 || iy == 0 || ix == dimx-1 || iy == dimy-1 || iz == 0 || iz == dimz-1) temp_new[is] = 0.0;
      }

      //print results?

      //swap the updated temperature into temp_old
      temp_old = temp_new;

    }
    //clock
    #ifdef _OPENMP
    sec = omp_get_wtime() - sec;
    #endif
    printf("heat finished in %f seconds with num_tiles = %d \n", sec, num_tiles);
  }
  //free(temp_old);
  //free(temp_new);
  return 0;

}
