#include "headers.h"
using namespace std;

int main(void){

  double dt = 0.05;	
  int tsteps = 1000; // Number of iterations
  int N = 81920; // number of particles in sim

  // initialize the universe!
  universe uni(N,dt);
  // read initial positions, masses, velocities from file
  uni.read_galaxy_data();

  Pre_Render();
  Render(uni.get_pos(),N);
  uni.update_force();

  for (int i = 0; i < tsteps; i++){		
    uni.update_position();	// Update the positio
    uni.update_velocity();	// Update the velocity
    uni.update_force();
    uni.update_velocity();	// Update the velocity
    Render(uni.get_pos(),N);
  }

  Post_Render();
  return 0;	
}
