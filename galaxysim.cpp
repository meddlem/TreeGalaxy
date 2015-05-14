#include "tree.h"
using namespace std;

double universe::rootsize(){
  // returns needed size of root
  double max = 0;

  for(int i=0; i<N; i++){
    if(abs(pos_mass[i].x) > max){max = abs(pos_mass[i].x);}
    if(abs(pos_mass[i].y) > max){max = abs(pos_mass[i].y);}
    if(abs(pos_mass[i].z) > max){max = abs(pos_mass[i].z);}
  }
  return max;
}

void universe::update_position(){
  for (int i = 0; i < N; i++){
    pos_mass[i].x += vel[i].vx*dt;	// x(i) = x(i-1) + v(i)*dt
    pos_mass[i].y += vel[i].vy*dt;	// x(i) = x(i-1) + v(i)*dt
    pos_mass[i].z += vel[i].vz*dt;	// x(i) = x(i-1) + v(i)*dt
  }
}

void universe::update_force(){

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
  double rootsz = rootsize();
  cout << rootsz << "\n";
  node* root = new node(rootsz, origin, 0.5, 0.025);

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

void universe::update_velocity_Plus(){
  for (int i = 0; i < N; i++){
    vel[i].vx = vel[i].vx + force[i][0]*dt;
    vel[i].vy = vel[i].vy + force[i][1]*dt;
    vel[i].vz = vel[i].vz + force[i][2]*dt;
  }
}

float ** universe::get_pos(){
  
  float** out = 0;
  out = new float*[N];

  for (int i = 0; i < N; i++)
  {
    out[i] = new float[3];
    out[i][0] = pos_mass[i].x;
    out[i][1] = pos_mass[i].y;
    out[i][2] = pos_mass[i].z;
  }

  return out;
}