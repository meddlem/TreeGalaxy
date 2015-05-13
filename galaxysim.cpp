#include "tree.h"

using namespace std;

/* 
NOTE:
Life is hell
*/

double dt = 0.001;		// discrete time step
double density = 0.1;		// The denisty of the box
const int N = 100000;			// Number of particles
int Run = 1000; 		// Number of simulations rounds

vector<part> pos;	// particle array X,Y,Z, mass
double vel[N][3];	// Velocity array Vx,Vy,Vz
double force[N][3];	// Force array Fx,Fy,Fz

// declare Functions

void Initiate_Position();
void Initiate_Velocity();	
void Initiate_Force();		
void Update_velocity_Plus();
void Update_force();	
void Update_position();
double rootsize();

// set up rng
default_random_engine generator;
normal_distribution<double> maxwell(0.0,0.1);


// define functions
void Initiate_Position(){
  for(int i=0;i<N;i++){
    pos.push_back (part());
    pos[i].x = 2*rand()/double(RAND_MAX) - 1;
    pos[i].y = 2*rand()/double(RAND_MAX) - 1;
    pos[i].z = 2*rand()/double(RAND_MAX) - 1;
    pos[i].mass = 1;
  }
}

void Initiate_Velocity(){
  for (int n=0; n<N; ++n){
  vel[n][0] = maxwell(generator);
  vel[n][1] = maxwell(generator);
  vel[n][2] = maxwell(generator);
  }
}

void Initiate_Force(){
  for (int n=0; n<N; ++n){
  force[n][0] = 0;
  force[n][1] = 0;
  force[n][2] = 0;
  }
}

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
  for (int n = 0; n < N; ++n){
    pos[n].x += vel[n][0] *dt;	// x(i) = x(i-1) + v(i)*dt
    pos[n].y += vel[n][1] *dt;	// x(i) = x(i-1) + v(i)*dt
    pos[n].z += vel[n][2] *dt;	// x(i) = x(i-1) + v(i)*dt
  }
}

void Update_force(){
  // start a new tree
  vector<double> origin = {0,0,0};
  node* root = new node(rootsize(), origin, 0.5);

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
  for (int n = 0; n < N; ++n){
    for (int d = 0; d < 3; ++d){		
      vel[n][d] = vel[n][d] + force[n][d]*dt ;
    }
  }
}

int main(int argc, char*argv[]){
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
	universe uni(N_gal1 + N_gal2);

	// Add galaxies
	uni.generate_gal(N_gal1,gal1,gal1_vel);
	uni.generate_gal(N_gal2,gal2,gal2_vel);

	// These methods return positions/mass and velocities
	uni.get_pos_mass();
	uni.get_vel();

  // Initiate positions
  Initiate_Position();		
  // Initiate velocity
  Initiate_Velocity(); 			
  // this should use the force calculated from initial conditions
  Initiate_Force();			

  for (int i = 0; i<Run; i++){		
    //--- Update system ---
    cout << i << "\n";
    Update_force();
    Update_velocity_Plus();			// Update the velocity
    Update_position();	// Update the position
  }
}
