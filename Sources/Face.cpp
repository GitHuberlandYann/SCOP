#include "scop.h"

Face::Face( Material *mat ) : _size(0), _color(0xffffff)
{
	if (mat) {
		_color = mat->get_color();
	}
}

Face::~Face( void )
{
	_vertices.clear();
	_vertices_textures.clear();
	_vertices_normals.clear();
}

// ************************************************************************** //
//                                  Private                                   //
// ************************************************************************** //

void Face::draw_line( Mlx *mlx, t_vertex & a, t_vertex & b )
{
	t_vertex delta, pixel;
	double len;

	delta.x = b.x - a.x;
	delta.y = b.y - a.y;
	delta.z = delta.y / delta.x;
	len = sqrt(delta.x * delta.x + delta.y * delta.y);
	delta.x /= len;
	delta.y /= len;
	pixel.x = a.x;
	pixel.y = a.y;
	while (len > 0)
	{
		if (mlx->_color_mode == DEFAULT) {
			mlx->put_pixel(pixel.x, pixel.y, 0xffffff);
		} else if (mlx->_color_mode == MATERIAL) {
			mlx->put_pixel(pixel.x, pixel.y, _color);
		}
		pixel.x += delta.x;
		if (delta.x)
			pixel.y = delta.z * (pixel.x - a.x) + a.y;
		else
			pixel.y += delta.y;
		--len;
	}
}

void Face::link_vertices( Mlx *mlx, int a, int b )
{
	t_vertex s, e;
	s.x = mlx->rotation_x(_vertices[a]) * mlx->_size + mlx->_offset_x;
	s.y = mlx->rotation_y(_vertices[a]) * mlx->_size + mlx->_offset_y;
	e.x = mlx->rotation_x(_vertices[b]) * mlx->_size + mlx->_offset_x;
	e.y = mlx->rotation_y(_vertices[b]) * mlx->_size + mlx->_offset_y;
	draw_line(mlx, s, e);
}

// ************************************************************************** //
//                                  Public                                    //
// ************************************************************************** //

void Face::add_vertex( t_vertex *vertex, t_vertex *texture, t_vertex *normal )
{
	if (vertex) {
		_vertices.push_back(*vertex);
	}
	if (texture) {
		_vertices_textures.push_back(*texture);
	}
	if (normal) {
		_vertices_normals.push_back(*normal);
	}
	++_size;
}

size_t Face::get_size( void )
{
	return (_size);
}

void Face::draw_face( Mlx *mlx )
{
	for (int index = 0; index < _size - 1; index++) {
		link_vertices(mlx, index, index + 1);
	}
	link_vertices(mlx, 0, _size - 1);
}
