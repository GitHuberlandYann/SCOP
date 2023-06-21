#ifndef SCOP_HPP
# define SCOP_HPP

# include "scop.h"
class Face;
class Material;
class Mlx;

enum {
	UNSET,
	ONLY_V,
	ONLY_VT,
	ONLY_VN,
	VTN
};

class Scop {
	private:
		std::string _root;
		int _face_mode;
		std::vector<t_vertex> _vertices;
		std::vector<t_vertex> _vertices_textures;
		std::vector<t_vertex> _vertices_normals;
		std::vector<Face *> _faces;
		std::vector<Material *> _materials;
		Material *_current_used_material;

		void get_face( std::string line );
		void add_vertex_face( Face *face, std::string line, size_t & index );
		t_vertex *get_vertex( int num );
		t_vertex *get_texture( int num );
		t_vertex *get_normal( int num );
		void add_materials( std::string file );

	public:
		Scop( std::string root );
		~Scop( void );

		void parse( std::string file );
		void display_content( void );
		void map_img(Mlx *mlx);
};


#endif
