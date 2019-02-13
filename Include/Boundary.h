#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "Particle.h"

class Boundary
{
  double length, height;

public:
  Boundary(double l, double h){length=l; height=h;};
  int CheckInside(Particle);
};

int Boundary::CheckInside(Particle p)
{
  vector<double> pos = p.GetPosition();
  if(pos[0] < 0 || pos[1] > length || pos[1] < 0. || pos[1] > height)
    return 0.;
  return 1.;
}

#endif
