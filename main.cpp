#include <mpi.h>
#include <stdio.h>
#include "Include/Driver.h"


int main (int argc, char *argv[])
{
  MPI_Init(NULL, NULL);

  Driver d;
  d.Run_Program();

  MPI_Finalize();
};
