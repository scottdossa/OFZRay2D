#ifndef DRIVER_H
#define DRIVER_H

#include <vector>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "Particle.h"
#include "Boundary.h"
#include "Mirror.h"

using namespace std;

class Driver
{
int NumParticles;
vector<Boundary> Boundaries;
vector<PointSource> Sources;
vector<Mirror> Mirrors;

void Ray_Trace(Particle);
void Write_CSV();

public:
  Driver(double, int);
  void Run_Program(double, double);

  void Add_Mirror(double x, double y, double a, double e, int xy, int lrtb)
    {Mirror m(x,y,a,e,xy,lrtb);Mirrors.push_back(m);};
  void Add_Source(double x, double y, double p)
    {PointSource ps(x,y,p); Sources.push_back(ps);};
  void Add_Boundary(vector< vector<double> > Nodes)
    {Boundary b(Nodes);Boundaries.push_back(b);}

  Mirror GetMirror(int i){return Mirrors[i];};
};

Driver::Driver(double size, int Num)
{
  NumParticles = Num;
  vector< vector<double> > nodes = {{-size, -size},{size, -size},{size,size},{-size,size}};
  Boundary b(nodes);
  Boundaries.push_back(b);
};

void Driver::Ray_Trace(Particle p)
{
  int side=-1;
  int closest_mirror=-1;
  int closest_boundary=-1;
  int closest_side=-1;
  double distance=-1;
  double distance_min = pow(10.,120);
  for(int i = 0; i < Boundaries.size(); i++)
  {
    side=Boundaries[i].ClosestSide(p);
    distance=Boundaries[i].FindDistance(side, p);
    if(side>-1 && distance<distance_min)
    {
      closest_boundary = i;
      closest_side = side;
      distance_min=distance;
    }
  }
  for(int i=0; i<Mirrors.size(); i++)
  {
    distance=Mirrors[i].FindDistance(p);
    if(distance > pow(10,-50) && distance<distance_min)
    {
      distance_min=distance;
      closest_side=-1;
      closest_mirror=i;
    }
  }
  if(closest_mirror==-1 && closest_boundary>-1)
  {
    //if(closest_boundary==1)
    //  printf("hit\n");
    Boundaries[closest_boundary].CountCollision(closest_side);
    return;
  }
  else if(closest_mirror>-1)
  {
    //printf("Reflected at Mirror %d\n",closest_mirror);
    vector<double> vel = p.GetVelocity();
    p.SetPosition(p.GetPosition()[0]+distance_min*vel[0],p.GetPosition()[1]+distance_min*vel[1]);
    vel = Mirrors[closest_mirror].ReflectedVelocity(p);
    p.SetVelocity(vel[0],vel[1]);
    Ray_Trace(p);
    return;
  }
  else
  {
    printf("Error: Particle lost\n");
    return;
  }
};

void Driver::Run_Program(double theta_min, double theta_max)
{
  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int NumSources = Sources.size();
  if(NumSources==0)
    return;
  int NumParticles_Per_Source = NumParticles/NumSources;
  double initial_angle;
  PointSource ps = Sources[0];
  Particle p(ps);
  for(int source = 0; source < NumSources; source++)
  {
    ps = Sources[source];
    for(int i = NumParticles_Per_Source/world_size*world_rank; i<NumParticles_Per_Source/world_size*(world_rank+1); i++)
    {
      initial_angle = theta_min+(theta_max-theta_min)*double(i)/double(NumParticles_Per_Source);
      //printf("---------------------\nAngle = (%f, %f)\n",cos(initial_angle),sin(initial_angle));
      p.SetPosition(ps.GetPosition()[0],ps.GetPosition()[1]);
      p.SetVelocity(cos(initial_angle),sin(initial_angle));
      Ray_Trace(p);
    }
  }
  Write_CSV();
};

void Driver::Write_CSV()
{
  ofstream csv_file;
  csv_file.open("rt.csv");
  for(int i=0; i<Boundaries[1].GetNumNodes(); i++)
    csv_file << Boundaries[1].GetNode(i)[1]<<","<<Boundaries[1].GetCounts()[i]<<"\n";
  csv_file.close();
};

#endif
