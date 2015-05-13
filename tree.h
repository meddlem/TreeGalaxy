#ifndef tree_H
#define tree_H
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include<string.h>
#include<random>
#include<unistd.h>
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
	std::vector<part> pos_mass; 
	std::vector<part_vel> vel; 
	

public:
	universe(int N_total);
	~universe() {};

	void generate_gal(int N_stars, part galaxy_position_mass, part_vel galaxy_velocity);
	void print2file();
	std::vector<part> get_pos_mass(){
		return pos_mass;
	} 
	std::vector<part_vel> get_vel(){
		return vel;
	}
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
