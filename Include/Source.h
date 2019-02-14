#ifndef SOURCE_H
#define SOURCE_H

#include<vector>

using namespace std;

class PointSource
{
  double Power;
  vector<double> position;

public:
  PointSource(double, double, double);
  vector<double> GetPosition(){return position;};
  double GetPower(){return Power;};
};

PointSource::PointSource(double x, double y, double power)
{
  position.resize(2);
  position[0] = x;
  position[1] = y;
  Power = power;
};

#endif
