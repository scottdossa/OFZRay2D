#include <mpi.h>
#include <stdio.h>
#include "Include/Driver.h"

void BehrTest();
void NoMirrorTest();
void SingleMirrorTest();

int main (int argc, char *argv[])
{
  MPI_Init(NULL, NULL);

  BehrTest();

  MPI_Finalize();
};

void BehrTest()
{
  int NumP = pow(10,6);
  int NumElem = 320;
  double length = 40.;
  double radius = 5.;

  Driver d(3000, NumP);

  d.Add_Mirror(0,470.,470,396./470.,1,3);
  d.Add_Mirror(0,74+2*396+504,655,504./655.,1,2);

  double f1 = d.GetMirror(0).GetInnerFocalPoint()[1];
  double f2 = d.GetMirror(1).GetInnerFocalPoint()[1];

  vector< vector<double> > nodes={{-radius, f2-length/2.}};
  for(int i=0; i<NumElem;i++)
    nodes.push_back({radius,f2+length*(-1./2.+double(i)/double(NumElem))});
  //nodes.push_back({radius, f2-length/2.});
  nodes.push_back({radius, f2+length/2.});
  nodes.push_back({-radius,f2+length/2.});
  d.Add_Boundary(nodes);

  double h  = d.GetMirror(0).GetOuterFocalPoint()[1];
  double r=1.;
  vector< vector<double> > nodes2={{-3000,h-0.1},{-r,h-0.1},{-r,h+0.1},{-3000,h+0.1}};
  vector< vector<double> > nodes3={{3000,h-0.1},{r,h-0.1},{r,h+0.1},{3000,h+0.1}};
  d.Add_Boundary(nodes2);
  d.Add_Boundary(nodes3);


  d.Add_Source(0, f1, 1);
  d.Run_Program(3./2.*M_PI-109./180.*M_PI,3.*M_PI/2.-49./180.*M_PI);
};

void NoMirrorTest()
{
  Driver d(100, pow(10,2));
  d.Add_Source(0,0.0,1);
  vector< vector< double> > nodes = {{-1,-1}};
  nodes.push_back({1,-1});nodes.push_back({1,1});nodes.push_back({-1,1});
  d.Add_Boundary(nodes);
  d.Run_Program(0,2*M_PI);
};

void SingleMirrorTest()
{
  int NumElem = 50;
  double length = 500;

  Driver d(1000,pow(10,0));
  vector< vector<double> > nodes;
  for(int i=0; i<NumElem; i++)
    nodes.push_back({-length/2.+length*double(i)/double(NumElem-1), 0});
  nodes.push_back({0, 1});
  d.Add_Boundary(nodes);

  d.Add_Mirror(0, -45, 50, 0.8, 0, 2);
  double f1 = d.GetMirror(0).GetInnerFocalPoint()[0];
  double f2 = d.GetMirror(0).GetInnerFocalPoint()[1];
  d.Add_Source(f1,f2,1);

  d.Run_Program(0,2*M_PI);

};
