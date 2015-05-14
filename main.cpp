#include "tree.h"
#include "Rendering.h"
using namespace std;

double dt = 0.05;		// discrete time step
int Run = 1000; 		// Number of simulations rounds
const int N = 81920; // number of particles in sim
const double eps = 0.025; //softening parameter
const double theta = 0.9; //opening angle

float* out[3];
std::vector<part> pos_mass; 
std::vector<part_vel> vel; 
double force[N][3];


void allocate()
{

  for (int i =0 ; i < 3 ;i++)
        out[i]= new float[N];
}

void get_pos(){
  for (int i = 0; i < N; i++)
  {
    out[0][i] = pos_mass[i].x;
    out[1][i] = pos_mass[i].y;
    out[2][i] = pos_mass[i].z;
  }
}

double rootsize(){
  // returns needed size of root
  double max = 0;

  for(int i=0; i<N; i++){
    if(abs(pos_mass[i].x) > max){max = abs(pos_mass[i].x);}
    if(abs(pos_mass[i].y) > max){max = abs(pos_mass[i].y);}
    if(abs(pos_mass[i].z) > max){max = abs(pos_mass[i].z);}
  }
  return max;
}

void update_position(){
  double m; 
  for (int i = 0; i < N; i++){
    m = pos_mass[i].mass;

    pos_mass[i].x += vel[i].vx*dt + 0.5*force[i][0]*dt*dt/m;	
    pos_mass[i].y += vel[i].vy*dt + 0.5*force[i][1]*dt*dt/m;	
    pos_mass[i].z += vel[i].vz*dt + 0.5*force[i][2]*dt*dt/m;	
  }
}

void update_force(){

  // Initialize force
  for (int i = 0; i < N; ++i){
    for (int j = 0; j<3; j++){
    force[i][j] = 0.;
    }
  }

  // start a new tree
  vector<double> origin (3,0);
  node* root = new node(rootsize(), origin, theta, eps);

  // insert particles into tree
  for(int i = 0; i<N; i++){
    root -> insert_particle(&pos_mass[i]);
  }
  
  //calculate force on each particle
  for(int i = 0; i < N; i++){
    root -> calcforce(&pos_mass[i],force[i]);
  }
  delete root;
}

void update_velocity(){
  double m;
  for (int i = 0; i < N; i++){
    m = pos_mass[i].mass;
//    cout << force[i][0] << "\n";

    vel[i].vx += 0.5*force[i][0]*dt/m;
    vel[i].vy += 0.5*force[i][1]*dt/m;
    vel[i].vz += 0.5*force[i][2]*dt/m;
  }
}

int main(){
  pos_mass.resize(N);
	vel.resize(N);
  allocate();

  ifstream input("dubinski.tab");

  for(int n; n < N; n++)
  { 
    input >> pos_mass[n].mass >> pos_mass[n].x >> pos_mass[n].y >> 
      pos_mass[n].z >> vel[n].vx >> vel[n].vy >> vel[n].vz;
    n++;
  }

  Pre_Render();
  get_pos();
  Render(out,N);
  update_force();

  for (int i = 0; i<Run; i++){		
    update_position();	// Update the positio
    update_velocity();	// Update the velocity
    update_force();
    update_velocity();	// Update the velocity
    get_pos();
    Render(out,N);
  }

  Post_Render();
  return 0;	
}
