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
		int _face_mode;
		t_vertex _max_box;
		t_vertex _min_box;
		std::vector<t_vertex> _vertices;
		std::vector<t_vertex> _vertices_textures;
		std::vector<t_vertex> _vertices_normals;
		std::vector<Face *> _faces;
		std::vector<Face *> _box;
		Material *_current_used_material;

		void push_face( std::string line, int generated_color );
		void add_vertex_face( Face *face, std::string line, size_t & index );
		void push_vertex( t_vertex vertex );
		t_vertex *get_vertex( int num );
		t_vertex *get_texture( int num );
		t_vertex *get_normal( int num );
		void add_materials( std::string file );
		void set_material( std::string name );
		
		double get_extremum( void );

	public:
		Scop( std::string root );
		~Scop( void );

		std::string _root;
		std::vector<Material *> _materials;

		void parse( std::string file );
		void display_content( void );
		void center_object( void );
		void map_img( Mlx *mlx );
		void map_box( Mlx *mlx );
};


#endif
