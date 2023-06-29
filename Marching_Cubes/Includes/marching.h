#ifndef MARCHING_H
# define MARCHING_H

# include <iostream>
# include <random>
# include <fstream>
# include <sstream>

typedef struct {
	double x;
	double y;
	double z;
}		t_vertex;

# include "MarchingCube.hpp"
# include "utils.hpp"
# include "PerlinNoise.hpp"

#endif
