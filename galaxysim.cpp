#include<iostream>
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<random>
#include<unistd.h>
#include <tuple>
#include <algorithm>

#include "tree.h"

using namespace std;

/* 
NOTES:
Life is hell
struct part {
 double x;
 double y;
 double z;
 double mass;
};
*/

char Save_Dir_Energy[] = "Energy.txt";
char Save_Dir_Quantities[] = "Quantities.txt";
double Temp_Goal;
double cutoff = 3;

double dt = 0.001;		// discrete time step
double density = 0.1;		// The denisty of the box
double a;	   	// Length of unit FCC unit cell

const int N = 864;			// Number of particles
int cub_num;		// Number of unit FCC boxes
int Run = 1000; 		// Number of simulations rounds
int run;		// Round number
int runtemp = -1;	// Used for the display function 
const int dim = 3;	// Number of dimension
const double boxlength = pow((N/density),0.3333); 	// Lengths of the box

vector<double> origin = {0,0,0};

vector<part> pos;	// Position array X,Y,Z
double vel[N][3];	// Velocity array Vx,Vy,Vz
double force[N][3];	// Force array Fx,Fy,Fz
/*
_________________________________________________________

 -------------- Function initialization ---------------- 
_________________________________________________________

*/

// Initiation Functions
void Make_array();					// In use
void Initiate_Position_FCC();				// In use
void Initiate_Velocity();				// In use
void Initiate_Force();					// In use

// Updating functions
void Update_velocity_Plus();				// In use 		 
void Update_force();				// In use 	
void Update_position(); // in use

// Extra functions
void Clean_up();					// In use

default_random_engine generator;
normal_distribution<double> maxwell(0.0,0.1);

  // ________ Initiate particles position ________ \\

void Initiate_Position(){
  for(int i=0;i<N;i++){
    pos.push_back (part());
    pos[i].x = 2*rand()/double(RAND_MAX) - 1;
    pos[i].y = 2*rand()/double(RAND_MAX) - 1;
    pos[i].z = 2*rand()/double(RAND_MAX) - 1;
    pos[i].mass = 1;
  }
}

// ________ Initiate particles velocity ________ \\

void Initiate_Velocity(){
  for (int n=0; n<N; ++n){
  vel[n][0] = maxwell(generator);
  vel[n][1] = maxwell(generator);
  vel[n][2] = maxwell(generator);
  }
}

// ________ Initiate particles "Previous" force ________ \\

void Initiate_Force(){
  for (int n=0; n<N; ++n){
  force[n][0] = 0;
  force[n][1] = 0;
  force[n][2] = 0;
  }
}

// ________ Update particles position ________ \\

// basic function

void Update_position(){
  for (int n = 0; n < N; ++n){
    pos[n].x += vel[n][0] *dt;	// x(i) = x(i-1) + v(i)*dt
    pos[n].y += vel[n][1] *dt;	// x(i) = x(i-1) + v(i)*dt
    pos[n].z += vel[n][2] *dt;	// x(i) = x(i-1) + v(i)*dt
  }
}

double maxpos(){
  double max = 0;

  for(int i=0; i<N; i++){
    if(abs(pos[i].x) > max){max = abs(pos[i].x);}
    if(abs(pos[i].y) > max){max = abs(pos[i].y);}
    if(abs(pos[i].z) > max){max = abs(pos[i].z);}
  }
  return max;
}

void Update_force(){
  double nodesize;
  nodesize = maxpos();

  // make a new tree
  node* root = new node(nodesize, origin, 0.5);
  // insert particles into tree..
  for(int i = 0; i<N; i++){
    root -> insert_particle(&pos[i]);
  }
  
  //calc force on each particle
  for(int i = 0; i < N; i++){
    root -> calcforce(&pos[i],force[i]);
  }
  delete root;
}

// ________ Update particles velocity ________ \\

void Update_velocity_Plus(){
  for (int n = 0; n < N; ++n){
    for (int d = 0; d < dim; ++d){		
      vel[n][d] = vel[n][d] + force[n][d]*dt ;
    }
  }
}

// ________ Cleaning memory ________ \\

/*
void Clean_up(){
  delete [] pos;
  for (int d=0; d<3; ++d){
    delete [] vel[d];
    delete [] force[d];
  }
}
*/

int main(int argc, char*argv[]){

  //--- Read and make variables ---
  //Parameters(argc, argv);			// Insert parameters
  // Others and calculated values
  cub_num = ceil(pow((N/4),0.333));	// Number of boxes
  a=boxlength/cub_num;			// Length of unity box
  //Make_array(argc, argv);			// preallocate arrays

  //--- Initiate ---
  Initiate_Position();		// Initiate positions
  Initiate_Velocity(); 			// Initiate velocity
  Initiate_Force();			// Initiate (indirectly) the previous  
            // forces by initializing the force as 0
  //--- Calculate values with initial conditions ---
  for (run = 0; run<Run; ++run){		
    //--- Update system ---
    cout << run << "\n";
    Update_force();
    Update_velocity_Plus();			// Update the velocity
    Update_position();	// Update the position
  }

  //--- Clean memory ---
  //Clean_up();
}
