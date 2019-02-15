#ifndef DRIVER_H
#define DRIVER_H

class Driver
{
int NumParticles;
vector<Boundary> Boundaries;
vector<PointSource> Sources;
//Mirrors

void Ray_Trace(double,PointSource);


public:
  Driver();
  void Run_Program();
};

Driver::Driver()
{
  NumParticles = 8;
  vector< vector< double > > b1 = {{0,0},{1,0},{1,1},{0,1}};
  Boundaries.push_back(b1);
  vector< vector< double> > b2 = {
                  {0.5,0.5 },/*{0.55,0.5},{0.6,0.5},{0.65,0.5},*/
                  {0.7,0.5},/*{0.7,0.55},{0.7,0.6},{0.7,0.65},*/
                  {0.7,0.7},/*{0.65,0.7},{0.6,0.7},{0.55,0.7},*/
                  {0.5,0.7}/*,{0.5,0.65},{0.5,0.6},{0.5,0.55}*/
                };
  Boundaries.push_back(b2);
  PointSource ps(0.6, 0.2, 1.);
  Sources.push_back(ps);
};

void Driver::Ray_Trace(double angle, PointSource ps)
{
  int side=-1;
  int closest_boundary=-1;
  int closest_side=-1;
  double distance=-1;
  double distance_min = pow(10.,120);
  Particle p(ps);
  p.SetVelocity(cos(angle),sin(angle));
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
  if(closest_boundary>-1 && closest_side>-1)
  {
    printf("%d, %d\n",closest_boundary,closest_side);
    Boundaries[closest_boundary].CountCollision(closest_side);
  }
};

void Driver::Run_Program()
{
  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int NumSources = Sources.size();
  int NumParticles_Per_Source = NumParticles/NumSources;
  double initial_angle;
  for(int source = 0; source < NumSources; source++)
  {
    for(int i = NumParticles_Per_Source/world_size*world_rank; i<NumParticles_Per_Source/world_size*(world_rank+1); i++)
    {
      initial_angle = 2*M_PI*double(i)/double(NumParticles_Per_Source);
      printf("%f, ",initial_angle);
      Ray_Trace(initial_angle, Sources[source]);
    }
  }
};

#endif
