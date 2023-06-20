#ifndef SCOP_HPP
# define SCOP_HPP

# include "scop.h"
class Face;

enum {
	UNSET,
	ONLY_V,
	ONLY_VT,
	ONLY_VN,
	VTN
};

class Scop {
	private:
		int _face_mode;
		std::vector<t_vertex> _vertices;
		std::vector<t_vertex> _vertices_textures;
		std::vector<t_vertex> _vertices_normals;
		std::vector<Face *> _faces;

		void get_face( std::string line );
		void add_vertex_face( Face *face, std::string line, size_t & index );
		t_vertex *get_vertex( int num );
		t_vertex *get_texture( int num );
		t_vertex *get_normal( int num );

	public:
		Scop( void );
		~Scop( void );

		void parse( std::string file );
		void display_content( void );
};


#endif
