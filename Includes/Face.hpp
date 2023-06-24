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
		unsigned int _grays[4];
		size_t *_texture_index;

		void fill_triangle( Mlx *mlx, t_vertex a, t_vertex b, t_vertex c, bool texture, t_vertex ta, t_vertex tb, t_vertex tc, t_vertex a3d, t_vertex b3d, t_vertex c3d );
		void fill_faces( Mlx *mlx );
		void draw_line( Mlx *mlx, t_vertex & a, t_vertex & b, bool texture, t_vertex ta, t_vertex tb, t_vertex a3d, t_vertex b3d );
		void link_vertices( Mlx *mlx, int a, int b );
		// void display_dir( Mlx *mlx );
		void link_normal( Mlx *mlx, int index );

	public:
		Face( Material *mat, int generated_color );
		~Face( void );

		void add_vertex( t_vertex *vertex, t_vertex *texture, t_vertex *normal );
		size_t get_size( void );
		void draw_face( Mlx *mlx );
		void center_object( t_vertex central_axis, double normalizer );
};

#endif
