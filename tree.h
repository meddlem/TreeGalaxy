#ifndef tree_H
#define tree_H
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <time.h>

const double PI = 3.14159265358979323846;

struct part {
	double x;
	double y;
	double z;
	double mass;
};

struct part_vel {
	double vx;
	double vy;
	double vz;
};

class universe{
private: 
	int N;
  double dt;
	std::vector<part> pos_mass; 
	std::vector<part_vel> vel; 
  std::vector<part_vel> force;
	
public:
	universe(int N, double dt);
	~universe() {};

	void generate_gal(int N_stars, part galaxy_position_mass, part_vel galaxy_velocity);
  void read_galaxy_data();
  void update_velocity();
  void update_force();
  void update_position();
  double rootsize();
  std::vector<part> get_pos(){return pos_mass;}
};

class node{
private:
    part *particle_present; //contains particle locaction if node is external
    part CM; // center of mass position + mass
    std::vector<double> origin; // origin of the node
    double halfDim; // half of the box length 
    double theta; //cutoff parameter 
    double eps; //cutoff parameter 
    node *octant[8]; // pointers to quadrants of the node 

public:
    node(const double halfDim, const std::vector<double> origin, 
        const double theta, const double eps):origin(origin), 
        halfDim(halfDim), theta(theta), eps(eps), particle_present(NULL){
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
    void calcforce(part* particle, part_vel* force);
};

#endif
