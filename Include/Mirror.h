#ifndef MIRROR_H
#define MIRROR_H

#include <cmath>
#include <vector>
#include "Particle.h"

class Mirror
{
  double x0, y0, a, e;
  bool xy; //0: Major axis along x, 1: Major axis along y
  int lrtb; // 0 = left side exists, 1 = right side exists, 2 = top exists, 3 = bottom exists

  public:
    Mirror(double x, double y, double major, double eccen, bool axis, int side){x0=x;y0=y;a=major;e=eccen;xy=axis;lrtb=side;};
    double FindDistance(Particle);
    double FindDistanceHighSlope(Particle);
    bool OnMirror(double, double);
    vector<double> ReflectedVelocity(Particle);
    vector<double> GetInnerFocalPoint();
    vector<double> GetOuterFocalPoint();
};

double Mirror::FindDistance(Particle p)
{
  double ex,ey,mp, bp;
  bool pos_exists=0;
  bool neg_exists=0;
  vector<double> vel = p.GetVelocity();
  vector<double> pos = p.GetPosition();
  mp = vel[1]/vel[0];
  if(abs(mp)>50)
    return this->FindDistanceHighSlope(p);
  bp = -mp*pos[0]+pos[1];
  //Get Eccentricity Value for Ellipse Equation
  if(xy==0)
  {
    ex=1.;
    ey=1./(1.-e*e);
  }
  if(xy==1)
  {
    ex=1./(1.-e*e);
    ey=1.;
  }
  //Solve Quadratic Equatio
  //Check Distance calculation
  double A, B, C;
  A = ex+ey*mp*mp;
  B = -2*x0*ex-2*ey*mp*(y0-bp);
  C = ex*x0*x0+ey*(y0-bp)*(y0-bp)-a*a;
  if(B*B-4*A*C<0)
    return -1;
  double x_neg, x_pos, y_pos, y_neg, dist, x, y,dist_neg,dist_pos;
  x_pos = (-B+sqrt(B*B-4*A*C))/(2*A);
  x_neg = (-B-sqrt(B*B-4*A*C))/(2*A);
  y_neg = mp*x_neg+bp;
  y_pos = mp*x_pos+bp;

  dist_pos = sqrt(pow(pos[0]-x_pos,2.)+pow(pos[1]-y_pos,2.));
  dist_neg = sqrt(pow(pos[0]-x_neg,2.)+pow(pos[1]-y_neg,2.));


  //printf("Distances to El = %f, %f\n",dist_pos, dist_neg);
  if(dist_pos>pow(10,-8) && OnMirror(pos[0]+dist_pos*vel[0],pos[1]+dist_pos*vel[1]))
    pos_exists=1;
  if(dist_neg>pow(10,-8) && OnMirror(pos[0]+dist_neg*vel[0],pos[1]+dist_neg*vel[1]))
    neg_exists=1;

  if(pos_exists && neg_exists)
    {
      if(dist_pos<dist_neg)
        return dist_pos;
      else
        return dist_neg;
    }
  else if(pos_exists && !neg_exists)
    return dist_pos;
  else if(!pos_exists && neg_exists)
    return dist_neg;
  else
    return -1;
};

double Mirror::FindDistanceHighSlope(Particle p)
{
  double ex,ey,mp, bp;
  bool pos_exists=0;
  bool neg_exists=0;
  vector<double> vel = p.GetVelocity();
  vector<double> pos = p.GetPosition();
  mp = vel[0]/vel[1];
  bp = -mp*pos[1]+pos[0];
  //Get Eccentricity Value for Ellipse Equation
  if(xy==0)
  {
    ex=1.;
    ey=1./(1.-e*e);
  }
  if(xy==1)
  {
    ex=1./(1.-e*e);
    ey=1.;
  }
  //Solve Quadratic Equatio
  //Check Distance calculation
  double A, B, C;
  A = ey+ex*mp*mp;
  B = -2*y0*ey-2*ex*mp*(x0-bp);
  C = ey*y0*y0+ex*(x0-bp)*(x0-bp)-a*a;
  if(B*B-4*A*C<0)
    return -1;
  double x_neg, x_pos, y_pos, y_neg, dist, x, y,dist_neg,dist_pos;
  y_pos = (-B+sqrt(B*B-4*A*C))/(2*A);
  y_neg = (-B-sqrt(B*B-4*A*C))/(2*A);
  x_neg = mp*y_neg+bp;
  x_pos = mp*y_pos+bp;

  dist_pos = sqrt(pow(pos[0]-x_pos,2.)+pow(pos[1]-y_pos,2.));
  dist_neg = sqrt(pow(pos[0]-x_neg,2.)+pow(pos[1]-y_neg,2.));


  //printf("Distances to El = %f, %f\n",dist_pos, dist_neg);
  if(dist_pos>pow(10,-8) && OnMirror(pos[0]+dist_pos*vel[0],pos[1]+dist_pos*vel[1]))
    pos_exists=1;
  if(dist_neg>pow(10,-8) && OnMirror(pos[0]+dist_neg*vel[0],pos[1]+dist_neg*vel[1]))
    neg_exists=1;

  if(pos_exists && neg_exists)
    {
      if(dist_pos<dist_neg)
        return dist_pos;
      else
        return dist_neg;
    }
  else if(pos_exists && !neg_exists)
    return dist_pos;
  else if(!pos_exists && neg_exists)
    return dist_neg;
  else
    return -1;
};

bool Mirror::OnMirror(double x, double y)
{
  double ex,ey;
  if(xy==0)
  {
    ex=1.;
    ey=1./(1.-e*e);
  }
  if(xy==1)
  {
    ex=1./(1.-e*e);
    ey=1.;
  }
  //Check if point lies on ellipse
  if(abs(ex*pow(x-x0,2)+ey*pow(y-y0,2)-a*a) < pow(10,-8))
  {
    if(lrtb==0)
    {
      if(x<x0-a/sqrt(ex) || x0<x)
        return 0;
      else
        return 1;
    }
    else if(lrtb==1)
    {
      if(x>x0+a/sqrt(ex)|| x0>x)
        return 0;
      else
        return 1;
    }
    else if(lrtb==2)
    {
      if(y>y0+a/sqrt(ey) || y0>y)
        return 0;
      else
        return 1;
    }
    else
    {
      if(y<y0-a/sqrt(ey) || y0<y)
        return 0;
      else
        return 1;
    }
  }
  return 0;
};

vector<double> Mirror::ReflectedVelocity(Particle p)
{
  vector<double> new_vel = {0.,0.};
  vector<double> norm = {0., 0.};
  vector<double> vel = p.GetVelocity();
  vector<double> pos = p.GetPosition();
  double ex,ey, mag;
  if(xy==0)
  {
    ex=1.;
    ey=1./(1.-e*e);
  }
  if(xy==1)
  {
    ex=1./(1.-e*e);
    ey=1.;
  }
  if(!this->OnMirror(pos[0],pos[1]))
    return vel;

  mag = sqrt(pow(ex*(pos[0]-x0),2)+pow(ey*(pos[1]-y0),2.));
  norm = {-ex*(pos[0]-x0)/mag,-ey*(pos[1]-y0)/mag};

  new_vel[0] = vel[0]-2*(vel[0]*norm[0]+vel[1]*norm[1])*norm[0];
  new_vel[1] = vel[1]-2*(vel[0]*norm[0]+vel[1]*norm[1])*norm[1];
  return new_vel;

};

vector<double> Mirror::GetInnerFocalPoint()
{
  vector<double> FP = {x0,y0};
  if(xy==0)
  {
    if(lrtb%2==0)
      FP = {x0-e*a, y0};
    else
      FP = {x0+e*a, y0};
  }
  else
  {
    if(lrtb%2==1)
      FP = {x0, y0-e*a};
    else
      FP = {x0, y0+e*a};
  }
  return FP;
};
vector<double> Mirror::GetOuterFocalPoint()
{
  vector<double> FP = {x0,y0};
  if(xy==0)
  {
    if(lrtb%2==1)
      FP = {x0-e*a, y0};
    else
      FP = {x0+e*a, y0};
  }
  else
  {
    if(lrtb%2==0)
      FP = {x0, y0-e*a};
    else
      FP = {x0, y0+e*a};
  }
  return FP;
};

#endif
