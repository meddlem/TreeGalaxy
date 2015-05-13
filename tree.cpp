#include <vector>
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;

// structure for the particle, holds coords + mass
struct part {
 double x;
 double y;
 double z;
 double mass;
};

class node{
  private:
    part *particle_present; //contains particle locaction if node is external
    part CM; // center of mass position + mass
    vector<double> origin; // origin of the node
    double halfDim; // half of the box length 
    double theta; //cutoff parameter 
    node *octant[8]; // pointers to quadrants of the node 

  public:
    node(const double halfDim, const vector<double> origin, const double theta)
      :origin(origin), halfDim(halfDim), theta(theta), particle_present(NULL){
      for (int i=0; i<8; i++){
        octant[i] = NULL;
      }
    }

    ~node(void){
      for (int i=0; i<8; i++)
      {
        delete octant[i];
      }
    }

    //prototypes
    bool isexternalnode(void) const;
    int get_octant(const part* particle) const;
    void insert_particle(part* particle);
    void calcforce(part* particle, double* force);
};

//methods

bool node::isexternalnode(void) const{
  return octant[0]==NULL;
}

int node::get_octant(const part* particle) const{
  // determine in which octant of the node a particle is located, the dumb way
  int oct = 0;
  
  if((particle->x > origin[0]) && (particle->y < origin[1]) && (particle->z > origin[2])){oct = 1;}
  else if((particle->x < origin[0]) && (particle->y < origin[1]) && (particle->z > origin[2])){oct = 2;}
  else if((particle->x < origin[0]) && (particle->y > origin[1]) && (particle->z > origin[2])){oct = 3;}
  else if((particle->x > origin[0]) && (particle->y > origin[1]) && (particle->z < origin[2])){oct = 4;}
  else if((particle->x > origin[0]) && (particle->y < origin[1]) && (particle->z < origin[2])){oct = 5;}
  else if((particle->x < origin[0]) && (particle->y < origin[1]) && (particle->z < origin[2])){oct = 6;}
  else if((particle->x < origin[0]) && (particle->y > origin[1]) && (particle->z < origin[2])){oct = 7;}

  return oct; 
}

void node::insert_particle(part* particle){
  if(isexternalnode()){
    if(particle_present == NULL){
      // if there pare no particles then we can put particle here
      particle_present = particle; 
      CM = *particle;
      return;
    }
    else{
      // if there are other paticles we need to split up the node
      // we take the old + new points and put them in quadrants of the node
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
        octant[i] = new node(halfDim_new, origin_new, theta);
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
void node::calcforce(part* particle, double *force){
  if(isexternalnode()){
    if((particle_present != NULL) && (particle_present != particle)){ 
      // dit laatste kan problemen geven, check met sarwan
      // weet niet of dit kan met pointers
      // calculate force
      double dx = particle->x - particle_present->x;
      double dy = particle->y - particle_present->y;
      double dz = particle->z - particle_present->z;
      double d = sqrt(dx*dx + dy*dy + dz*dz);    
      double m1 = particle_present->mass;
      double m2 = particle->mass;
      
      // update total force
      force[0] = force[0] - dx*m1*m2/(d*d*d); 
      force[1] = force[1] - dy*m1*m2/(d*d*d); 
      force[2] = force[2] - dz*m1*m2/(d*d*d); 
    }
  }
  else{
    // calculate s/d<theta
    double dx = particle->x - CM.x;
    double dy = particle->y - CM.y;
    double dz = particle->z - CM.z;
    // calculate distance of particle to CM of node
    double d = sqrt(dx*dx + dy*dy + dz*dz);

    if ((halfDim/d) < theta){
      // treat node as a single particle, and calc force
      double m1 = CM.mass;
      double m2 = particle->mass;

      // update total force
      force[0] = force[0] - dx*m1*m2/(d*d*d);
      force[1] = force[1] - dy*m1*m2/(d*d*d); 
      force[2] = force[2] - dz*m1*m2/(d*d*d); 
    }
    else
    {
      // continue recursively into child nodes
      for(int i=0; i<8; i++){
        octant[i] -> calcforce(particle, force);
      }
    }
  }
}

double randU(){ // Returns a random number between 1, -1
  double r = 2*rand()/double(RAND_MAX) - 1;
  return r; 
}

// need routine that returns length, origin so that all particles are 
// contained in it
//double defbox(vector<pos> p){ 
//}

int main(void){
  vector<double> origin = {0,0,0};
  node* root = new node(2,origin, 0.5);
  
  vector<part> p;
  int n = 10000;

  // make random particles (coords + masses)
  for(int i = 0; i<n; i++){
    p.push_back (part());
    p[i].x = randU(); //(i+1)/double(n);
    p[i].y = randU(); // (i+1)/double(n);
    p[i].z = randU(); // (i+1)/double(n);
    p[i].mass = 1;// + randU();
  }
  
  // insert particles into tree..
  for(int i = 0; i<n; i++){
    root -> insert_particle(&p[i]);
  }

  clock_t t1 = clock();
  // calc total force on 1 particle, by all others, make this a vector again?
  double force[n][3];
  for(int i = 0; i < n; i++){
    root -> calcforce(&p[i],force[i]);
  }
  clock_t t2 = clock();
  cout << double((t2-t1))/CLOCKS_PER_SEC << "\n";

  cout << "(" << force[0][0] << " , "<< force[1][0] << " , " << force[2][0] << ")" << "\n";
  return 0;
}
