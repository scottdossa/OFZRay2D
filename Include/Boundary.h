#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <vector>
#include "Particle.h"

using namespace std;

class Boundary
{
  vector<int> collision_count;
  vector< vector<double> > nodes;

public:
  Boundary(vector< vector<double> >);
  vector<int> GetCounts(){return collision_count;};
  vector<double> GetNode(int i){return nodes[i];};
  double FindDistance(int, Particle);
  int ClosestSide(Particle);
  void CountCollision(int i){collision_count[i]++;};
};

Boundary::Boundary(vector< vector<double> > Nodes)
{
  nodes = Nodes;
  vector<int> counts(Nodes.size(),0.);
  collision_count = counts;
};

int Boundary::ClosestSide(Particle p)
{
  double closest=-1;  //Index of closest surface
  double dist_min=pow(10.,120); //Very large number to start
  double dist;
  int N = nodes.size();
  for(int i=0; i<N; i++)
  {
    dist = FindDistance(i,p);
    if(dist > 0 && dist<dist_min)
      {
        dist_min = dist;
        closest = i;
      }
  }
  return closest;
};

double Boundary::FindDistance(int i, Particle p)
{
  if(i==-1)
    return -1;
  double dist=-1.;
  int N = nodes.size();
  double mp; //Slope of particle's path
  double bp; //Y-intercept of Particle
  double ma; //Slope of Boundary element;
  double ba; //Y-intercept of Boundary element;
  double x,y;//Intersection Points
  vector<double> pos = p.GetPosition();
  vector<double> vel = p.GetVelocity();
  //Calculate slopes and intercepts
  mp = vel[1]/vel[0];
  ma = (nodes[(i+1)%N][1]-nodes[i][1])/(nodes[(i+1)%N][0]-nodes[i][0]);
  bp = -mp*pos[0]+pos[1];
  ba = -ma*nodes[i][0] + nodes[i][1];

  //Check if both lines are vertical or parallel
  if(abs(ma) > pow(10,6) && abs(mp) > pow(10,6))
    return -1;
  else if(ma==mp)
    return -1;

  //If neither line is vertical
  else if( abs(ma) < pow(10,6) && abs(mp) < pow(10,6))
  {
    x = (bp - ba)/(ma - mp);
    y = ma*x+ba;
    dist = sqrt(pow(x-pos[0],2) + pow(y-pos[1],2));
  }

  //If one line is vertical
  else
  {
    if(abs(mp) > pow(10,6) && abs(ma) < pow(10,6))
      {
        x = pos[0];
        y = ma*x+ba;
      }
    else
    {
      x = nodes[i][0];
      y = mp*x+bp;
    }
    dist = sqrt(pow(x-pos[0],2) + pow(y-pos[1],2));
  }

  //Check that intersection lies on element
  if((x > nodes[i][0] && x > nodes[(i+1)%N][0]) || (x < nodes[i][0] && x < nodes[(i+1)%N][0]))
    return -1;
  if((y > nodes[i][1] && y > nodes[(i+1)%N][1]) || (y < nodes[i][1] && y < nodes[(i+1)%N][1]))
    return -1;

  //If intersection point is along the positive velocity direction
  if(dist != -1 && abs(pos[0]+vel[0]*dist-x)<pow(10,-8)  && abs(pos[1]+vel[1]*dist-y)<pow(10,-8))
    return dist;
  else
    return -1;
}

#endif
