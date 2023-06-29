#ifndef MARCHINGCUBE_HPP
# define MARCHINGCUBE_HPP

# include "marching.h"

class MarchingCube {
	private:
		size_t _size[3];
		int *_array;
		int *(_carrays[2]);
		int _block;
		int _SurfaceValue;
		int _nb_vertex;

		void vMarchCube(int fX, int fY, int fZ, std::ofstream & outdata);

	public:
		MarchingCube( void );
		~MarchingCube( void );

		void set_size( size_t x, size_t y, size_t z );
		void gen_random( void );
		void gen_small_sphere( void );
		void gen_Perlin( void );
		void set_cross_arrays( std::ifstream & indata );
		void gen_cross_array( void );
		void display( void );
		void vMarchingCubes( std::ofstream & outdata );

};

#endif
