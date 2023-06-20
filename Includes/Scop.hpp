#ifndef SCOP_HPP
# define SCOP_HPP

# include "scop.h"

class Scop {
	private:
		std::vector<t_vertex> _vertices;
		std::vector<t_vertex> _vertices_textures;
		std::vector<t_vertex> _vertices_normals;
		std::vector<Face *> _faces;

	public:
		Scop( void );
		~Scop( void );

		void parse( std::string file );
		void display_content( void );
};


#endif
