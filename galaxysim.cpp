#include "tree.h"
using namespace std;

double dt = 0.05;		// discrete time step
int Run = 1000; 		// Number of simulations rounds

const int N_gal1 = 1000;
const int N_gal2 = 1000;
const int N = N_gal1 + N_gal2;

std::vector<part> pos;
std::vector<part_vel> vel;
double force[N][3];	// Force array Fx,Fy,Fz

// declare Functions

void Update_velocity_Plus();
void Update_force();	
void Update_position();
double rootsize();

double rootsize(){
  // returns needed size of root
  double max = 0;

  for(int i=0; i<N; i++){
    if(abs(pos[i].x) > max){max = abs(pos[i].x);}
    if(abs(pos[i].y) > max){max = abs(pos[i].y);}
    if(abs(pos[i].z) > max){max = abs(pos[i].z);}
  }
  return max;
}

void Update_position(){
  for (int i = 0; i < N; i++){
    pos[i].x += vel[i].vx*dt;	// x(i) = x(i-1) + v(i)*dt
    pos[i].y += vel[i].vy*dt;	// x(i) = x(i-1) + v(i)*dt
    pos[i].z += vel[i].vz*dt;	// x(i) = x(i-1) + v(i)*dt
  }
}

void Update_force(){
  // start a new tree
  vector<double> origin = {0,0,0};
  double rootsz = rootsize();
  cout << rootsz << "\n";
  node* root = new node(rootsz, origin, 0.5);

  // insert particles into tree
  for(int i = 0; i<N; i++){
    root -> insert_particle(&pos[i]);
  }
  
  //calculate force on each particle
  for(int i = 0; i < N; i++){
    root -> calcforce(&pos[i],force[i]);
  }
  delete root;
}

void Update_velocity_Plus(){
  for (int i = 0; i < N; i++){
    vel[i].vx = vel[i].vx + force[i][0]*dt;
    vel[i].vy = vel[i].vy + force[i][1]*dt;
    vel[i].vz = vel[i].vz + force[i][2]*dt;
  }
}

int main(void){
	// Create first galaxy
	// Number of stars

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
	universe uni(N_gal1 + N_gal2);

	// Add galaxies
	uni.generate_gal(N_gal1,gal1,gal1_vel);
	uni.generate_gal(N_gal2,gal2,gal2_vel);

	// These methods return positions/mass and velocities
	pos = uni.get_pos_mass();
	vel = uni.get_vel();
  
  for (int i = 0; i<Run; i++){		
    Update_force();
    Update_velocity_Plus();			// Update the velocity
    Update_position();	// Update the position
  }

  return 0;
}
