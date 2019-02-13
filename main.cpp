#include <mpi.h>
#include <stdio.h>
#include "Particle/Particle.h"

int main (int argc, char *argv[]) {

  MPI_Init(NULL, NULL);

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  printf("Ray Tracing on %d processors\n", world_size);

  PointSource ps(0,0,1);
  Particle p(ps);

  printf("%f, %f\n", p.GetPosition()[0], p.GetPosition()[1]);

  MPI_Finalize();
}
