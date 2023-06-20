#ifndef FACE_HPP
# define FACE_HPP

# include "scop.h"

class Face {
	private:
		int _size;
		std::vector<t_vertex> _vertices;
		std::vector<t_vertex> _vertices_textures;
		std::vector<t_vertex> _vertices_normals;

	public:
		Face( void );
		~Face( void );

		void add_vertex( t_vertex *vertex, t_vertex *texture, t_vertex *normal );
		size_t get_size( void );
};

#endif
