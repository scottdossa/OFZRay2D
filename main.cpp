#include <mpi.h>
#include <stdio.h>
#include "Include/Particle.h"
#include "Include/Boundary.h"
#include "Include/Absorber.h"

int main (int argc, char *argv[]) {

  MPI_Init(NULL, NULL);

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  if(world_rank==0)
    printf("Ray Tracing on %d processors\n", world_size);

  PointSource ps(0.5,0.5,1);
  Particle p(ps);
  Boundary Bounds(1,1);

  vector< vector<double> > Nodes = { {0.7,0.6}, {0.7,0.7}, {0.5,0.7}, {0.5,0.6} };
  Absorber A(Nodes);

  MPI_Finalize();
}
