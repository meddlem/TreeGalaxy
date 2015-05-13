#ifndef tree_H
#define tree_H
#include <vector>
#include <iostream>
#include <math.h>

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
    std::vector<double> origin; // origin of the node
    double halfDim; // half of the box length 
    double theta; //cutoff parameter 
    node *octant[8]; // pointers to quadrants of the node 

  public:
    node(const double halfDim, const std::vector<double> origin, const double theta)
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

#endif
