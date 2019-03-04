#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>
#include "Source.h"

using namespace std;

class Particle
{
  vector<double> position;
  vector<double> unit_velocity;

  public:
    Particle(PointSource);
    vector<double> GetPosition(){return position;};
    vector<double> GetVelocity(){return unit_velocity;};
    void SetPosition(double x, double y){position[0]=x, position[1]=y;};
    void SetVelocity(double x, double y){unit_velocity[0]=x/sqrt(x*x+y*y); unit_velocity[1]=y/sqrt(x*x+y*y);};
};

Particle::Particle(PointSource ps)
{
  position = ps.GetPosition();
  unit_velocity.resize(2);
};

#endif
