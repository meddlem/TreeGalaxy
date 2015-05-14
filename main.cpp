#include "tree.h"
using namespace std;

int main(){

	double dt = 0.05;		// discrete time step
	int Run = 1000; 		// Number of simulations rounds

	// Create first galaxy
	// Number of stars
	int N_gal1 = 1000;

	// position of the galaxy
	part gal1;
		gal1.x = 0;
		gal1.y = 0;
		gal1.z = 0;

	// mass of galaxy
		gal1.mass = 1;

	// Global velocity of galaxy
	part_vel gal1_vel;
		gal1_vel.vx = 0;
		gal1_vel.vy = 0;
		gal1_vel.vz = 0;

	// Same for galaxy n°2
	int N_gal2 = 1000;
	part gal2;
		gal2.x = 40;
		gal2.y = 0;
		gal2.z = 0;
		gal2.mass = 5;
	part_vel gal2_vel;
		gal2_vel.vx = 0;
		gal2_vel.vy = 0;
		gal2_vel.vz = 0;

	// Create a universe with N_gal1 + N_gal2 stars
	int N = N_gal1 + N_gal2;
	universe uni(N,dt);

	// Add galaxies
	uni.generate_gal(N_gal1,gal1,gal1_vel);
	uni.generate_gal(N_gal2,gal2,gal2_vel);
  
  for (int i = 0; i<Run; i++){		
    uni.update_force();
    uni.update_velocity_Plus();			// Update the velocity
    uni.update_position();	// Update the position
  }

  return 0;
}