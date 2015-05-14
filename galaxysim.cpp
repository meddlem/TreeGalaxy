#include "tree.h"
using namespace std;

double universe::rootsize(){
  // returns needed size of root
  double max = 0.;

  for(int i=0; i<N; i++){
    if(abs(pos_mass[i].x) > max){max = abs(pos_mass[i].x);}
    if(abs(pos_mass[i].y) > max){max = abs(pos_mass[i].y);}
    if(abs(pos_mass[i].z) > max){max = abs(pos_mass[i].z);}
  }
  return max;
}

void universe::update_position(){
  double m; 
  for (int i = 0; i < N; i++){
    m = pos_mass[i].mass;

    pos_mass[i].x += vel[i].vx*dt + 0.5*force[i][0]*dt*dt/m;	
    pos_mass[i].y += vel[i].vy*dt + 0.5*force[i][1]*dt*dt/m;	
    pos_mass[i].z += vel[i].vz*dt + 0.5*force[i][2]*dt*dt/m;	
  }
}

void universe::update_force(){
  // Initialize force
  for (int i = 0; i < N; ++i){
    for (int j = 0; j<3; j++){
    force[i][j] = 0.;
    }
  }

  // start a new tree
  vector<double> origin (3,0);
  node* root = new node(rootsize(), origin, 0.9, 0.025);

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

void universe::update_velocity(){
  double m;
  for (int i = 0; i < N; i++){
    m = pos_mass[i].mass;

    vel[i].vx += 0.5*force[i][0]*dt/m;
    vel[i].vy += 0.5*force[i][1]*dt/m;
    vel[i].vz += 0.5*force[i][2]*dt/m;
  }
}

float** universe::get_pos(){
  float ** out = 0;

  for (int i = 0; i < 3; i++)
  {
    out[i]    = new float[N];
    out[0][i] = pos_mass[i].x;
    out[1][i] = pos_mass[i].y;
    out[2][i] = pos_mass[i].z;
  }
}
