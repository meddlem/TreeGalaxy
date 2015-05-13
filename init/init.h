#ifndef INIT_H
#define INIT_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
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

#endif