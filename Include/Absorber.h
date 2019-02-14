#ifndef ABSORBER_H
#define ABSORBER_H

#include <vector>
#include "Particle.h"

using namespace std;

class Absorber
{
  vector<int> collision_count;
  vector< vector<double> > nodes;

public:
  Absorber(vector< vector<double> >);
  vector<int> GetCounts(){return collision_count;};
  vector<double> GetNode(int i){return nodes[i];};
  int ClosestSide(Particle);
};

Absorber::Absorber(vector< vector<double> > Nodes)
{
  nodes = Nodes;
  vector<double> count(Nodes.size(),0.);
};

int Absorber::ClosestSide(Particle p)
{
  double mp; //Slope of particle's path
  double bp; //Y-intercept of Particle
  double ma; //Slope of absorber element;
  double ba; //Y-intercept of absorber element;
  double x,y;//Intersection Points
  double closest=-1;  //Index of closest surface
  double dist=pow(10.,120); //Very large number to start
  int N = nodes.size()-1;
  vector<double> pos = p.GetPosition();
  vector<double> vel = p.GetVelocity();
  for(int i=0; i<N; i++)
  {
    mp = vel[1]/vel[0];
    bp = pos[1]-mp*pos[0];
    ma = (nodes[i+1][1]-nodes[i][1])/(nodes[i+1][0]-nodes[i][0]);
    ma = nodes[i][1]-ma*nodes[i][0];
    x = (bp-ba)/(ma-mp);
    y = mp*x+bp;
    if(nodes[i][0] < x && x < nodes[i+1][0] && sqrt(pow(x-pos[0],2)+pow(y-pos[1],2))<dist)
    {
      dist = sqrt(pow(x-pos[0],2)+pow(y-pos[1],2));
      closest = i;
    }
  }
  return closest;
};

#endif
