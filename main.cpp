#include "tree.h"
#include "Rendering.h"
using namespace std;

double dt = 0.05;		// discrete time step
int Run = 1000; 		// Number of simulations rounds
const int N = 81920;

float* out[3];
std::vector<part> pos_mass; 
std::vector<part_vel> vel; 
std::vector<double*> force;
//float poss[3][N];

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
  for (int i = 0; i < N; i++){
    pos_mass[i].x += vel[i].vx*dt;	// x(i) = x(i-1) + v(i)*dt
    pos_mass[i].y += vel[i].vy*dt;	// x(i) = x(i-1) + v(i)*dt
    pos_mass[i].z += vel[i].vz*dt;	// x(i) = x(i-1) + v(i)*dt
  }
}

void update_force(){

  // Initialize force
  for (int i = 0; i < N; ++i)
  {
    double f[3];
    f[0] = 0.;
    f[1] = 0.;
    f[2] = 0.;
    force[i] = f;
  }

  // start a new tree
  vector<double> origin (3,0);
  node* root = new node(rootsize(), origin, 0.5, 0.025);

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
  for (int i = 0; i < N; i++){
    vel[i].vx = vel[i].vx + force[i][0]*dt;
    vel[i].vy = vel[i].vy + force[i][1]*dt;
    vel[i].vz = vel[i].vz + force[i][2]*dt;
  }
}

int main(){
  pos_mass.resize(N);
	vel.resize(N);
	force.resize(N);
  allocate();

  ifstream input("dubinski.tab");

  int n = 0;
  while (n < N)
  {
    input >> pos_mass[n].mass >> pos_mass[n].z >> pos_mass[n].y >> 
      pos_mass[n].x >> vel[n].vz >> vel[n].vy >> vel[n].vx;
    n++;
  }
  Pre_Render();
  get_pos();
  Render(out,N);

  for (int i = 0; i<Run; i++){		
    update_force();
    update_velocity();	// Update the velocity
    update_position();	// Update the positio
    get_pos();
    Render(out,N);
  }

  Post_Render();
  return 0;	
}
