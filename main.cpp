#include <mpi.h>
#include <stdio.h>
#include "Include/Driver.h"

void BehrTest();
void NoMirrorTest();
void SingleMirrorTest();

int main (int argc, char *argv[])
{
  MPI_Init(NULL, NULL);

  NoMirrorTest();

  MPI_Finalize();
};

void BehrTest()
{

};

void NoMirrorTest()
{
  Driver d(100, pow(10,2));
  d.Add_Source(0,0.5,1);
  vector< vector< double> > nodes = {{-1,-1}};
  nodes.push_back({1,-1});nodes.push_back({1,1});nodes.push_back({-1,1});
  d.Add_Boundary(nodes);
  d.Run_Program();
};

void SingleMirrorTest()
{
  int NumElem = 100;

  Driver d(1000, pow(10,3));
  d.Add_Source(0,0,1);
  vector< vector<double> > nodes;
  for(int i=0; i<NumElem; i++)
    nodes.push_back({-length+double(i)/double(NumElem-1), 0})
  nodes.push_back({0, 1});

};
