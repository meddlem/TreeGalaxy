#include "headers.h"
using namespace std;

int main(int argc, char* argv[]){

  double dt = 0.02;	 // Time step
  int tsteps = 1000; // Number of iterations
  int N = 8096;     // Number of particles in sim
  bool gen;
  
  for(int i = 1; i < argc; i++)
    if(std::string(argv[i]) == "--gen") gen = true;
  
  if(!gen) N = 81920;

  // initialize the universe!
  universe uni(N,dt);

  if(gen){
    // calculate initial positions, masses, velocities from plummer dist
    uni.generate_galaxy();
  }
  else{
    // or read them from a data file
    uni.read_galaxy_data();
  }

  // initialize rendering
  Pre_Render();
  Render(uni.get_pos(),N);
  // initialize force
  uni.update_force();

  for (int i = 0; i < tsteps; i++){		
    // time integration using leapfrog (Velocity Verlet) algorithm
    uni.update_position();	// Update particle positions
    uni.update_velocity();	// Update particle velocities 1/2
    uni.update_force();     // Calculate forces
    uni.update_velocity();	// Update particle velocites, 2/2
    Render(uni.get_pos(),N);
  }

  Post_Render();
  return 0;	
}
