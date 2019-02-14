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
  //double CheckDistance(Particle);
};

Absorber::Absorber(vector< vector<double> > Nodes)
{
  nodes = Nodes;
  vector<double> count(Nodes.size(),0.);
};

#endif
