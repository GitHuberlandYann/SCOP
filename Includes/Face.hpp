#ifndef FACE_HPP
# define FACE_HPP

# include "scop.h"

class Material;
class Mlx;

class Face {
	private:
		int _size;
		std::vector<t_vertex> _vertices;
		std::vector<t_vertex> _vertices_textures;
		std::vector<t_vertex> _vertices_normals;
		unsigned int _color;

		void draw_line( Mlx *mlx, t_vertex & s, t_vertex & e );
		void link_vertices( Mlx *mlx, int a, int b );

	public:
		Face( Material *mat );
		~Face( void );

		void add_vertex( t_vertex *vertex, t_vertex *texture, t_vertex *normal );
		size_t get_size( void );
		void draw_face( Mlx *mlx );
};

#endif
