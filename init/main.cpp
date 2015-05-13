#include "header.h"
#include "init.h"

int main()
{	

	int N_gal1 = 1000;
	part gal1;
		gal1.x = 0;
		gal1.y = 0;
		gal1.z = 0;
		gal1.mass = 1;
	part_vel gal1_vel;
		gal1_vel.vx = 0;
		gal1_vel.vy = 0;
		gal1_vel.vz = 0;

		int N_gal2 = 1000;
	part gal2;
		gal2.x = 40;
		gal2.y = 0;
		gal2.z = 0;
		gal2.mass = 5;
	part_vel gal2_vel;
		gal2_vel.vx = 0;
		gal2_vel.vy = 0;
		gal2_vel.vz = 0;

	Npart universe(N_gal1 + N_gal2);
	universe.generate_gal(N_gal1,gal1,gal1_vel);
	universe.generate_gal(N_gal2,gal2,gal2_vel);
	universe.print2file();
	return 0;
}