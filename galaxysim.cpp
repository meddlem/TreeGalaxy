#include "headers.h"
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

    pos_mass[i].x = pos_mass[i].x + vel[i].vx*dt + 0.5*force[i].vx*dt*dt/m;	
    pos_mass[i].y = pos_mass[i].y + vel[i].vy*dt + 0.5*force[i].vy*dt*dt/m;	
    pos_mass[i].z = pos_mass[i].z + vel[i].vz*dt + 0.5*force[i].vz*dt*dt/m;	
  }
}

void universe::update_force(){
  // Initialize force
  for (int i = 0; i < N; ++i){
    force[i].vx = 0.;
    force[i].vy = 0.;
    force[i].vz = 0.;
  }
  
  // start a new tree
  vector<double> origin (3,0);
  node* root = new node(rootsize(), origin, 0.25, 0.025);

  // insert particles into tree
  for(int i = 0; i<N; i++){
    root -> insert_particle(&pos_mass[i]);
  }
  
  //calculate force on each particle
  for(int i = 0; i < N; i++){
    root -> calcforce(&pos_mass[i],&force[i]);
  }
  delete root;
  //direct calc
  
  /* 
  double eps = 0.025;
  
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      if(i!=j){
      
      double dx = pos_mass[i].x - pos_mass[j].x;
      double dy = pos_mass[i].y - pos_mass[j].y;
      double dz = pos_mass[i].z - pos_mass[j].z;
      double d = sqrt(dx*dx + dy*dy + dz*dz);    
      double m1 = pos_mass[i].mass;
      double m2 = pos_mass[j].mass;
      
      // update total force
      force[i].vx += -dx*m1*m2/pow(d*d + eps*eps,1.5); 
      force[i].vy += -dy*m1*m2/pow(d*d + eps*eps,1.5); 
      force[i].vz += -dz*m1*m2/pow(d*d + eps*eps,1.5); 
      }
    } 
  }
  */
}

void universe::update_velocity(){
  double m;
  for (int i = 0; i < N; i++){
    m = pos_mass[i].mass;

    vel[i].vx = vel[i].vx + 0.5*force[i].vx*dt/m;
    vel[i].vy = vel[i].vy + 0.5*force[i].vy*dt/m;
    vel[i].vz = vel[i].vz + 0.5*force[i].vz*dt/m;
  }
}
