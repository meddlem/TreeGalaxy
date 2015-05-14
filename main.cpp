#include "tree.h"
#include "Rendering.h"
using namespace std;

double dt = 0.05;		// discrete time step
int Run = 1000; 		// Number of simulations rounds
const int N = 30000;

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

int main(){
  pos_mass.resize(N);
	vel.resize(N);
	force.resize(N);
  allocate();

  ifstream input("dubinski.tab");

  int n = 0;
  while (n < N)
  {
    input >> pos_mass[n].mass >> pos_mass[n].z >> pos_mass[n].y >> pos_mass[n].x >> vel[n].vz >> vel[n].vy >> vel[n].vx;
    n++;
  }
  Pre_Render();
  get_pos();
  Render(out,N);

  for (int i = 0; i<Run; i++){		
   // uni.update_force();
   // uni.update_velocity_Plus();	// Update the velocity
   // uni.update_position();	// Update the positio
   Render(out,N);
  }

  Post_Render();
  return 0;	
}
