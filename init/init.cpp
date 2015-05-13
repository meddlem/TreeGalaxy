#include "init.h"
#include "header.h"

using namespace std;

Npart::Npart(int N_total):
N(0)
{
	pos_mass.resize(N_total);
	vel.resize(N_total);
}

double frand(double low, double high){
	return low + (double)rand() * (high - low) / (double)RAND_MAX;
}

void Npart::generate_gal(int N_stars, part gal_pos_mass, part_vel gal_vel)
{

	int N_old = N;
	N += N_stars;

	srand(time(NULL)); 

	double radius;
	double theta;
	double phi;

	double x = 0.0;
	double y = 0.1;
	double velocity;

	double star_mass = gal_pos_mass.mass / N;
	double radius0 = 3 * PI * gal_pos_mass.mass * gal_pos_mass.mass /64;
	double velocity0 = 64 * sqrt (gal_pos_mass.mass) / 3 / PI;

	// Plummer
	
	for (int i = N_old; i < N; ++i)
	{
		// Generate position
		radius = radius0 / sqrt( pow(frand(0,1),(-2.0/3.0)) - 1.0);
		theta = acos(frand(-1, 1));
		phi = frand(0, 2*PI);
		pos_mass[i].x = gal_pos_mass.x + radius * sin( theta ) * cos( phi );
		pos_mass[i].y = gal_pos_mass.y + radius * sin( theta ) * sin( phi );
		pos_mass[i].z = gal_pos_mass.z + radius * cos( theta );

		// Generate velocity
		while (y > x*x*pow((1.0-x*x),3.5)){
			x = frand(0,1);
			y = frand(0,0.1);
		}
	
		velocity = x * sqrt(2.0) * pow(( 1.0 + radius*radius),(-0.25));
		theta = acos(frand(-1, 1));
		phi = frand(0, 2*PI);

		vel[i].vx = gal_vel.vx + velocity * sin( theta ) * cos( phi );
		vel[i].vy = gal_vel.vy + velocity * sin( theta ) * sin( phi );
		vel[i].vz = gal_vel.vz + velocity * cos( theta );

		// Determine mass
		pos_mass[i].mass = star_mass;

	}

}

void Npart::print2file(){
	ofstream file;
		file.open ("galaxy.txt");
		for (int i = 0; i < N; ++i)
		{
			file << pos_mass[i].x << ' ' << pos_mass[i].y<< ' ' << vel[i].vx<< ' ' << vel[i].vy << endl;
		}
		file.close();
}