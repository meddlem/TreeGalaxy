#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

// structure for the particle, holds coords + mass
struct part {
 double x;
 double y;
 double mass;
};

class node{
  private:
    part *particle_present;
    part CM; // center of mass position + mass
    vector<double> origin; // origin of the node
    double halfDim; // half of the box length 
    double theta; //cutoff parameter 
    node *quadrant[4]; // pointers to quadrants of the node 

  public:
    node(const double halfDim, const vector<double> origin, const double theta)
      :origin(origin), halfDim(halfDim), theta(theta), particle_present(NULL){
      for (int i=0; i<4; i++){
        quadrant[i] = NULL;
      }
    }

    ~node(void){
      for (int i=0; i<4; i++)
      {
        delete quadrant[i];
      }
    }

    //prototypes
    bool isexternalnode(void) const;

    int get_quadrant(const part* particle) const;
    
    void insert_particle(part* particle);

    void calcforce(part* particle, vector<vector<double>*> &forces);
    
    void getpoints(vector<part*> &results);
};

bool node::isexternalnode(void) const{
  return quadrant[0]==NULL;
}

int node::get_quadrant(const part* particle) const{
  int quad = 0;
  
  if((particle->x > origin[0]) && (particle->y < origin[1])){quad = 1;}
  else if((particle->x < origin[0]) && (particle->y < origin[1])){quad = 2;}
  else if((particle->x < origin[0]) && (particle->y > origin[1])){quad = 3;}

  return quad; 
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
      // update total mass
      CM.mass = Mnew;

      //now make the quadrant nodes
      double halfDim_new = halfDim/2;
      vector<double> origin_new(2);

      for(int i=0; i<4; i++){
        origin_new[0] = origin[0] + ((i<2)?1:-1)*halfDim/sqrt(2);
        origin_new[1] = origin[1] + ((i==0||i==3)?1:-1)*halfDim/sqrt(2);
        quadrant[i] = new node(halfDim_new, origin_new, theta);
      }
      
      // find quadrant where the points are and insert them in child nodes
      int qold = get_quadrant(particle_old);
      int qnew = get_quadrant(particle);

      quadrant[qold] -> insert_particle(particle_old);
      quadrant[qnew] -> insert_particle(particle);
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
    // update total mass
    CM.mass = Mnew;
    
    // and then insert particle in one of the quadrants
    int q = get_quadrant(particle);
    quadrant[q] -> insert_particle(particle); 
  }
}

// calculates forces particles/ CMs on a given particle
void node::calcforce(part* particle, vector<vector<double>*> &forces){
  if(isexternalnode()){
    if((particle_present != NULL) && (particle_present !=particle)){ 
      // dit laatste kan problemen geven, check met sarwan
      // weet niet of dit kan met pointers
      // calculate force
      double dx = particle->x - particle_present->x;
      double dy = particle->y - particle_present->y;
      double d = sqrt(dx*dx + dy*dy);    
      double m1 = particle_present->mass;
      double m2 = particle->mass;
      
      vector<double> force(2);
      force[0]= -dx*m1*m2/(d*d); 
      force[1]= -dy*m1*m2/(d*d); 
      
      // push force vector into list containing all force vectors..
      forces.push_back(&force);
    }
  }
  else{
    // calculate s/d<theta
    double s = 2*halfDim; 
    double dx = particle->x - CM.x;
    double dy = particle->y - CM.y;
    double m1 = CM.mass;
    double m2 = particle->mass;
    // calculate distance of particle to CM of node
    double d = sqrt(dx*dx + dy*dy);

    if ((s/d) < theta){
      // treat node as a single particle, and calc force on particle based on
      // this
      vector<double> force(2);
      force[0]= -dx*m1*m2/(d*d); // should be a vector..
      force[1]= -dy*m1*m2/(d*d); // should be a vector..

      // push force vector into list containing all force vectors..
      forces.push_back(&force);
    }
    else
    {
      // continue recursively into child nodes
      for(int i=0; i<4; i++){
        quadrant[i] -> calcforce(particle, forces);
      }
    }
  }
}

//traverse the tree and collect points
void node::getpoints(vector<part*> &results){
  if(isexternalnode()){
    if(particle_present != NULL){
      results.push_back(particle_present);
    }
  }
  else{
    for(int i=0; i<4; i++){
      quadrant[i] -> getpoints(results);
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
  vector<double> origin = {0,0};
  node* root = new node(2,origin, 0.5);
  
  vector<part> p;
  int n = 1000;

  // make random particles (coords + masses)
  for(int i = 0; i<n; i++){
    p.push_back (part());
    p[i].x = randU(); //(i+1)/double(n);
    p[i].y = randU(); // (i+1)/double(n);
    p[i].mass = 2 + randU();
  }
  
  // insert particles into tree..
  for(int i = 0; i<n; i++){
    root -> insert_particle(&p[i]);
  }

  // return list of results
  vector<part*> results;
  root -> getpoints(results);

  // print 1 of them as a check 
  cout << results[1]->x << "\n";
  cout << results[1]->y << "\n";
  cout << results[1]->mass << "\n";
  
  // calc forces on 1st particle?
  vector<vector<double>*> forces;
  root -> calcforce(&p[0],forces);

  double test = (*forces[0])[1];
  cout << test << "\n";
  return 0;
}
