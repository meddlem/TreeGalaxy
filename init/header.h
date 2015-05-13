#ifndef HEADER_H
#define HEADER_H

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

#endif
