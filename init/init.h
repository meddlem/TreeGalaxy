#ifndef INIT_H
#define INIT_H

#include "header.h"

class Npart{
private: 
	int N;
	std::vector<part> pos_mass; 
	std::vector<part_vel> vel; 
	

public:
	Npart(int N_total);
	~Npart () {};

	void generate_gal(int N_stars, part galaxy_position_mass, part_vel galaxy_velocity);
	void print2file();
};

#endif