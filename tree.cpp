#include "headers.h"

using namespace std;

//methods implementing the octree

bool node::externalnode(void) const{
  return octant[0]==NULL;
}

int node::get_octant(const part* particle) const{
  // determine in which octant of the node a particle is located
  int oct = 0;
  
  if((particle->x > origin[0]) && (particle->y < origin[1]) 
      && (particle->z > origin[2])){oct = 1;}
  else if((particle->x < origin[0]) && (particle->y < origin[1]) 
      && (particle->z > origin[2])){oct = 2;}
  else if((particle->x < origin[0]) && (particle->y > origin[1]) 
      && (particle->z > origin[2])){oct = 3;}
  else if((particle->x > origin[0]) && (particle->y > origin[1]) 
      && (particle->z < origin[2])){oct = 4;}
  else if((particle->x > origin[0]) && (particle->y < origin[1]) 
      && (particle->z < origin[2])){oct = 5;}
  else if((particle->x < origin[0]) && (particle->y < origin[1]) 
      && (particle->z < origin[2])){oct = 6;}
  else if((particle->x < origin[0]) && (particle->y > origin[1]) 
      && (particle->z < origin[2])){oct = 7;}

  return oct; 
}

void node::insert_particle(part* particle){
  if(externalnode()){
    if(particle_present == NULL){
      // if there pare no particles then we can put particle here
      particle_present = particle; 
      CM = *particle;
      return;
    }
    else{
      // if there are other paticles we need to split up the node
      // we take the old + new points and put them in octants of the node
      // but first we update the mass and center of mass of the node
      part* particle_old = particle_present;
      particle_present = NULL;
      
      // add particle mass to total mass in the node
      double Mold = CM.mass;
      double Madd = particle->mass;
      double Mnew = Mold + Madd;
      // update center of mass position
      CM.x = (Mold*CM.x + Madd*particle->x)/Mnew;
      CM.y = (Mold*CM.y + Madd*particle->y)/Mnew;
      CM.z = (Mold*CM.z + Madd*particle->z)/Mnew;
      // update total mass
      CM.mass = Mnew;

      //now make the quadrant nodes
      double halfDim_new = halfDim/2;
      vector<double> origin_new(3);

      for(int i=0; i<8; i++){
        origin_new[0] = origin[0] + 
          ((i==0||i==1||i==4||i==5)?1:-1)*halfDim/sqrt(3);
        origin_new[1] = origin[1] + 
          ((i==2||i==3||i==6||i==7)?1:-1)*halfDim/sqrt(3);
        origin_new[2] = origin[2] + ((i<4)?1:-1)*halfDim/sqrt(3);
        octant[i] = new node(halfDim_new, origin_new, theta, eps);
      }
      
      // find quadrant where the points are and insert them in child nodes
      int Oold = get_octant(particle_old);
      int Onew = get_octant(particle);

      octant[Oold] -> insert_particle(particle_old);
      octant[Onew] -> insert_particle(particle);
    }
  }
  else{
    // add particle mass to total mass in the node
    double Mold = CM.mass;
    double Madd = particle->mass;
    double Mnew = Mold + Madd;
    // update center of mass position
    CM.x = (Mold*CM.x + Madd*particle->x)/Mnew;
    CM.y = (Mold*CM.y + Madd*particle->y)/Mnew;
    CM.z = (Mold*CM.z + Madd*particle->z)/Mnew;
    // update total mass
    CM.mass = Mnew;
    
    // and then insert particle in one of the octants
    int q = get_octant(particle);
    octant[q] -> insert_particle(particle); 
  }
}

// calculates forces particles/ CMs on a given particle
void node::calcforce(part* particle, part_vel* force){
  double G = 1.;
  if(externalnode()){
    if((particle_present != NULL) && (particle_present != particle)){ 
      // calculate force
      double dx = particle->x - particle_present->x;
      double dy = particle->y - particle_present->y;
      double dz = particle->z - particle_present->z;
      double d = sqrt(dx*dx + dy*dy + dz*dz);    
      double m1 = particle_present->mass;
      double m2 = particle->mass;
      if (d<0.0001) cout << "error\n";
      
      // update total force
      force->vx = force->vx - G*dx*m1*m2/pow(d*d + eps*eps,1.5); 
      force->vy = force->vy - G*dy*m1*m2/pow(d*d + eps*eps,1.5); 
      force->vz = force->vz - G*dz*m1*m2/pow(d*d + eps*eps,1.5); 
    }
  }
  else{
    // calculate distance of particle to CM of node
    double dx = particle->x - CM.x;
    double dy = particle->y - CM.y;
    double dz = particle->z - CM.z;
    double d = sqrt(dx*dx + dy*dy + dz*dz);

    // check if s/d<theta
    if ((2*halfDim/d) < theta){
      // in this case treat node as a single particle, and calc force
      double m1 = CM.mass;
      double m2 = particle->mass;

      // update total force
      force->vx = force->vx - G*dx*m1*m2/pow(d*d + eps*eps,1.5);
      force->vy = force->vy - G*dy*m1*m2/pow(d*d + eps*eps,1.5); 
      force->vz = force->vz - G*dz*m1*m2/pow(d*d + eps*eps,1.5); 
    }
    else
    {
      // continue recursively into the child nodes
      for(int i=0; i<8; i++){
        octant[i] -> calcforce(particle, force);
      }
    }
  }
}
