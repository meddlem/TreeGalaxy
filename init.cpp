#include "headers.h"

using namespace std;

universe::universe(int N, double dt):
N(N), dt(dt)
{
  pos_mass.resize(N);
  vel.resize(N);
  force.resize(N); 
}

void universe::read_galaxy_data(){
  ifstream input("tab1024");

  for(int n = 0; n<N; n++){ 
    input >> pos_mass[n].mass >> pos_mass[n].x >> pos_mass[n].y >> 
      pos_mass[n].z >> vel[n].vx >> vel[n].vy >> vel[n].vz;
  // take into account that we are omitting masses
  // pos_mass[n].mass = 4.*pos_mass[n].mass; 
  }
}

double frand(double low, double high){
	return low + (double)rand() * (high - low) / (double)RAND_MAX;
}

void universe::generate_galaxy()
{

	srand(time(NULL)); 

	double radius;
	double theta;
	double phi;

	double x = 0.0;
	double y = 0.1;
	double velocity;
	double galaxy_mass = 2.;

	double star_mass = galaxy_mass / N;
	double radius0 = 3 * PI * galaxy_mass * galaxy_mass /64;
	double velocity0 = 64 * sqrt (galaxy_mass) / 3 / PI;

	// Plummer
	
	for (int i = 0; i < N; ++i)
	{
    // Generate position
		radius = radius0 / sqrt( pow(frand(0,1),(-2.0/3.0)) - 1.0);
		theta = acos(frand(-1, 1));
		phi = frand(0, 2*PI);
		pos_mass[i].x = radius * sin( theta ) * cos( phi );
		pos_mass[i].y = radius * sin( theta ) * sin( phi );
		pos_mass[i].z = radius * cos( theta );
    
		// Generate velocity
		while (y > x*x*pow((1.0-x*x),3.5)){
			x = frand(0,1);
			y = frand(0,0.1);
		}
	
		velocity = x * sqrt(2.0) * pow(( 1.0 + radius*radius),(-0.25));
		theta = acos(frand(-1, 1));
		phi = frand(0, 2*PI);

		vel[i].vx = velocity * sin( theta ) * cos( phi );
		vel[i].vy = velocity * sin( theta ) * sin( phi );
		vel[i].vz = velocity * cos( theta );

		// Determine mass
		pos_mass[i].mass = star_mass;
	}
}
