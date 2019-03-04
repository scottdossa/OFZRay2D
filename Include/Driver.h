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
  Driver();
  void Run_Program();
};

Driver::Driver()
{
  NumParticles = pow(10,5);
  vector< vector< double > > b1 = {{-5000,-5000},{5000,-5000},{5000,5000},{-5000,5000}};
  Boundaries.push_back(b1);
  double x_min = -100;
  double length = 2*abs(x_min);
  int NumElem = 50;
  vector< vector<double> > b2 = {{x_min, 0}};
  for(int i=1; i<NumElem+1; i++)
    b2.push_back({x_min+length*double(i)/double(NumElem),0});
  b2.push_back({0,5});
  Boundaries.push_back(b2);
  Mirror m1(0, -100, 150, 0.6666666666667, 1, 3);

  Mirrors.push_back(m1);

  PointSource p(0, -200,1 );
  Sources.push_back(p);
  /*double s1 = 74, p1 = 396, s2 = 151, p2 = 504;
  //printf("Edge of system = %f\nFocal Point = %f\n",s1+2*p1+2*p2+s2, s1+2*p1+2*p2);
  vector< vector< double > > b1 = {{-5000,-5000},{5000,-5000},{5000,5000},{-5000,5000}};
  Boundaries.push_back(b1);
  double f2 = s1+2*p1+2*p2;
  double length = 40.;
  double radius = 5.;
  double NumElem = 40;
  vector< vector< double> > b2 = {{-radius,f2-length/2.}};
  for(int i=0; i<NumElem; i++)
    b2.push_back({radius, f2-length/2.+length*double(i)/double(NumElem)});
  b2.push_back({radius,f2+length/2.});
  //for(int i=0; i<NumElem; i++)
  //  b2.push_back({-radius, f2+length/2.-length*double(i)/double(NumElem)});
  b2.push_back({-radius,f2+length/2.});
  Boundaries.push_back(b2);
  PointSource ps(0,s1,1.);
  Sources.push_back(ps);
  Mirror m1(0,s1+p1,s1+p1,0.842553,1,3);
  Mirrors.push_back(m1);
  Mirror m2(0,s1+2*p1+p2,s2+p2,0.769466,1,2);
  Mirrors.push_back(m2);*/
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
  //printf("Min Boundary distance = %f\n",distance_min);
  for(int i=0; i<Mirrors.size(); i++)
  {
    distance=Mirrors[i].FindDistance(p);
    if(distance > pow(10,-8) && distance<distance_min)
    {
      distance_min=distance;
      closest_side=-1;
      closest_mirror=i;
    }
  }
  if(closest_boundary>-1 && closest_side>-1)
  {
    //if(closest_boundary>0 && closest_side!=0)
    //printf("Hit at = (%d, %d)\n",closest_boundary,closest_side);
    Boundaries[closest_boundary].CountCollision(closest_side);
    return;
  }
  if(closest_mirror>-1)
  {
    //printf("Intersection Point = (%f, %f)\n",p.GetPosition()[0]+p.GetVelocity()[0]*distance,p.GetPosition()[1]+p.GetVelocity()[1]*distance);
    //printf("Reflection at (%d) mirror\n",closest_mirror);
    vector<double> vel = p.GetVelocity();
    p.SetPosition(p.GetPosition()[0]+distance_min*vel[0],p.GetPosition()[1]+distance_min*vel[1]);
    vel = Mirrors[closest_mirror].ReflectedVelocity(p);
    //printf("velocity reflected = (%f,%f)\n",vel[0],vel[1]);
    //printf("Angle between old and new = %f\n",180./M_PI* acos(vel[0]*p.GetVelocity()[0]+vel[1]*p.GetVelocity()[1]));
    p.SetVelocity(vel[0],vel[1]);
    //Ray_Trace(p);
    return;
  }
  return;
};

void Driver::Run_Program()
{
  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int NumSources = Sources.size();
  int NumParticles_Per_Source = NumParticles/NumSources;
  double initial_angle;
  PointSource ps = Sources[0];
  Particle p(ps);
  for(int source = 0; source < NumSources; source++)
  {
    ps = Sources[source];
    for(int i = NumParticles_Per_Source/world_size*world_rank; i<NumParticles_Per_Source/world_size*(world_rank+1); i++)
    {
      initial_angle = 2*M_PI*double(i)/double(NumParticles_Per_Source);
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
  int NumElem = 50;
  for(int i=0; i<NumElem; i++)
    csv_file << Boundaries[1].GetNode(i)[0]<<","<<Boundaries[1].GetCounts()[i]<<"\n";
  csv_file.close();
};

#endif
