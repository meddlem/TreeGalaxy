#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

// structure for the particle (coords + mass)
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
    node *quadrant[4]; // pointers to quadrants of the node 

  public:
    node(const double halfDim, const vector<double> origin)
      :origin(origin), halfDim(halfDim), particle_present(NULL){
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
    bool isleafnode(void) const;

    int get_quadrant(const part* particle) const;
    
    void insert_particle(part* particle);
    
    void getpoints(vector<part*> &results);
};

bool node::isleafnode(void) const{
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
  if(isleafnode()){
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
        quadrant[i] = new node(halfDim_new, origin_new);
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

double randU(){ // Random number between 1, -1
  double r = 2*rand()/double(RAND_MAX) - 1;
  return r; 
}

//traverse the tree and collect points
void node::getpoints(vector<part*> &results){
  if(isleafnode()){
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

//double defbox(vector<pos> p){ 
//}

int main(void){
  vector<double> origin = {0,0};
  node* root = new node(2,origin);
  
  vector<part> p;
  int n = 100;

  // insert particles into tree..
  for(int i = 0; i<n; i++){
    p.push_back (part());
    p[i].x = randU(); //(i+1)/double(n);
    p[i].y = randU(); // (i+1)/double(n);
  }
  
  for(int i = 0; i<n; i++){
    root -> insert_particle(&p[i]);
  }

  // return list of results
  vector<part*> results;
  root -> getpoints(results);
  cout << results[1]->x << "\n";
  cout << results[1]->y << "\n";
  return 0;
}
