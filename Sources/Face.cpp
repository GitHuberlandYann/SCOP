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

void Face::fill_triangle( Mlx *mlx, t_vertex a, t_vertex b, t_vertex c )
{
	t_vertex starta, startc, delta, deltc;

	starta = {a.x, a.y, 0};
	startc = {c.x, c.y, 0};

	delta = {b.x - a.x, b.y - a.y, 0};
	delta.z = delta.y / delta.x;
	deltc = {b.x - c.x, b.y - c.y, 0};
	deltc.z = deltc.y / deltc.x;
	
	double len;
	double lena = sqrt(delta.x * delta.x + delta.y * delta.y);
	double lenc = sqrt(deltc.x * deltc.x + deltc.y * deltc.y);
	if (lena > lenc) {
		len = lena;
	} else {
		len = lenc;
	}

	delta.x /= len;
	delta.y /= len;
	deltc.x /= len;
	deltc.y /= len;
	//set cols here TODO

	for (; len > 0; --len) {
		draw_line(mlx, a, c);
		a.x += delta.x;
		if (delta.x)
			a.y = delta.z * (a.x - starta.x) + starta.y;
		else
			a.y += delta.y;
		c.x += deltc.x;
		if (deltc.x)
			c.y = deltc.z * (c.x - startc.x) + startc.y;
		else
			c.y += deltc.y;
	}
}

void Face::fill_faces( Mlx *mlx )
{
	t_vertex f0, f1, f2;
	for (int index = 1; index < _size - 1; ++index) {
		f0.x = mlx->rotation_x(_vertices[0]) * mlx->_size + mlx->_offset_x;
		f0.y = mlx->rotation_y(_vertices[0]) * mlx->_size + mlx->_offset_y;
		f1.x = mlx->rotation_x(_vertices[index]) * mlx->_size + mlx->_offset_x;
		f1.y = mlx->rotation_y(_vertices[index]) * mlx->_size + mlx->_offset_y;
		f2.x = mlx->rotation_x(_vertices[index + 1]) * mlx->_size + mlx->_offset_x;
		f2.y = mlx->rotation_y(_vertices[index + 1]) * mlx->_size + mlx->_offset_y;
		fill_triangle(mlx, f0, f1, f2);
	}
}

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

// void Face::display_dir( Mlx *mlx )
// {
// 	t_vertex s, e, o;
// 	o = {0, 0, 0};
// 	s.x = mlx->rotation_x(o) * mlx->_size + mlx->_offset_x;
// 	s.y = mlx->rotation_y(o) * mlx->_size + mlx->_offset_y;
// 	e.x = mlx->rotation_x(mlx->_dir) * mlx->_size + mlx->_offset_x;
// 	e.y = mlx->rotation_y(mlx->_dir) * mlx->_size + mlx->_offset_y;
// 	draw_line(mlx, s, e);
// }

void Face::link_normal( Mlx *mlx, int index )
{
	if (!mlx->_show_normals || _vertices_normals.empty()) {
		return ;
	}

	t_vertex s, e, n;
	n = {_vertices[index].x + _vertices_normals[index].x, _vertices[index].y + _vertices_normals[index].y, _vertices[index].z + _vertices_normals[index].z};
	s.x = mlx->rotation_x(_vertices[index]) * mlx->_size + mlx->_offset_x;
	s.y = mlx->rotation_y(_vertices[index]) * mlx->_size + mlx->_offset_y;
	e.x = mlx->rotation_x(n) * mlx->_size + mlx->_offset_x;
	e.y = mlx->rotation_y(n) * mlx->_size + mlx->_offset_y;
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
	double scalar;
	t_vertex norm;
	if (mlx->_use_normal && !_vertices_normals.empty()) {
		for (int index = 0; index < _size; index++) {
			norm = {mlx->rotation_x(_vertices_normals[index]), mlx->rotation_y(_vertices_normals[index]), mlx->rotation_z(_vertices_normals[index])};
			scalar = mlx->_dir.x * norm.x + mlx->_dir.y * norm.y + mlx->_dir.z * norm.z;
			if (scalar < 0) {
				return ;
			}
		}
	}

	if (mlx->_fill) {
		return (fill_faces(mlx));
	}
	for (int index = 0; index < _size - 1; index++) {
		link_vertices(mlx, index, index + 1);
		link_normal(mlx, index);
	}
	link_vertices(mlx, 0, _size - 1);
	link_normal(mlx, _size - 1);

	// display_dir(mlx);
}
