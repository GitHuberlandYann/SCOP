#include "scop.h"

Face::Face( Material *mat ) : _size(0), _color(0xffffff), _texture_index(NULL)
{
	if (mat) {
		_color = mat->get_color();
		_texture_index = mat->get_texture_index();
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

void Face::fill_triangle( Mlx *mlx, t_vertex a, t_vertex b, t_vertex c, bool texture, t_vertex ta, t_vertex tb, t_vertex tc )
{
	t_vertex starta, startc, delta, deltc, deltta, delttc;

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

	if (texture) {
		deltta = {(tb.x - ta.x) / len, (tb.y - ta.y) / len, 0};
		delttc = {(tb.x - tc.x) / len, (tb.y - tc.y) / len, 0};
	}

	for (; len > 0; --len) {
		draw_line(mlx, a, c, texture, ta, tc);
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
		if (texture) {
			ta.x += deltta.x;
			ta.y += deltta.y;
			tc.x += delttc.x;
			tc.y += delttc.y;
		}
	}
}

void Face::fill_faces( Mlx *mlx )
{
	double z0 = 0, z1 = 0, z2 = 0;

	if (mlx->_plane_enable) {
		z0 = mlx->rotation_z(_vertices[0]);
		if (z0 * (1 - 2 * mlx->_plane_side) < mlx->_plane * (1 - 2 * mlx->_plane_side)) {
			return ;
		}
	} else if (mlx->_perspective_enable) {
		z0 =  mlx->rotation_z(_vertices[0]);
	}
	
	t_vertex f0, f1, f2, t0, t1, t2;
	bool texture = (mlx->_color_mode == TEXTURE && !_vertices_textures.empty() && _texture_index && *_texture_index != std::string::npos);
	f0.x = mlx->rotation_x(_vertices[0]) * mlx->_size * mlx->add_perspective(z0) + mlx->_offset_x;
	f0.y = mlx->rotation_y(_vertices[0]) * mlx->_size * mlx->add_perspective(z0) + mlx->_offset_y;

	if (texture) {
		t0 = mlx->set_textvert(_vertices_textures[0], *_texture_index);
	}
	for (int index = 1; index < _size - 1; ++index) {
		if (mlx->_perspective_enable) {
			z1 = mlx->rotation_z(_vertices[index]);
			z2 = mlx->rotation_z(_vertices[index + 1]);
		}
		f1.x = mlx->rotation_x(_vertices[index]) * mlx->_size * mlx->add_perspective(z1) + mlx->_offset_x;
		f1.y = mlx->rotation_y(_vertices[index]) * mlx->_size * mlx->add_perspective(z1) + mlx->_offset_y;
		f2.x = mlx->rotation_x(_vertices[index + 1]) * mlx->_size * mlx->add_perspective(z2) + mlx->_offset_x;
		f2.y = mlx->rotation_y(_vertices[index + 1]) * mlx->_size * mlx->add_perspective(z2) + mlx->_offset_y;
		if (texture) {
			t1 = mlx->set_textvert(_vertices_textures[index], *_texture_index);
			t2 = mlx->set_textvert(_vertices_textures[index + 1], *_texture_index);
		}
		fill_triangle(mlx, f0, f1, f2, texture, t0, t1, t2);
	}
}

void Face::draw_line( Mlx *mlx, t_vertex & a, t_vertex & b, bool texture, t_vertex ta, t_vertex tb )
{
	t_vertex delta, pixel, deltt;
	double len;

	delta.x = b.x - a.x;
	delta.y = b.y - a.y;
	delta.z = delta.y / delta.x;
	len = sqrt(delta.x * delta.x + delta.y * delta.y);
	delta.x /= len;
	delta.y /= len;
	pixel.x = a.x;
	pixel.y = a.y;

	if (texture) {
		deltt = {(tb.x - ta.x) / len, (tb.y - ta.y) / len, 0};
	}

	while (len > 0)
	{
		if (mlx->_color_mode == DEFAULT) {
			mlx->put_pixel(pixel.x, pixel.y, 0xffffff);
		} else if (mlx->_color_mode == MATERIAL) {
			mlx->put_pixel(pixel.x, pixel.y, _color);
		} else if (texture) {
			mlx->put_pixel(pixel.x, pixel.y, mlx->get_pixel(*_texture_index, ta.x, ta.y));
			ta.x += deltt.x;
			ta.y += deltt.y;
		} else {
			mlx->put_pixel(pixel.x, pixel.y, 0x950321);
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
	double za = 0, zb = 0;

	if (mlx->_plane_enable) {
		za = mlx->rotation_z(_vertices[a]);
		if (za * (1 - 2 * mlx->_plane_side) < mlx->_plane * (1 - 2 * mlx->_plane_side)) {
			return ;
		}
		zb = mlx->rotation_z(_vertices[b]);
		if (zb * (1 - 2 * mlx->_plane_side) < mlx->_plane * (1 - 2 * mlx->_plane_side)) {
			return ;
		}
	} else if (mlx->_perspective_enable) {
		za = mlx->rotation_z(_vertices[a]);
		zb = mlx->rotation_z(_vertices[b]);
	}

	t_vertex s, e;
	s.x = mlx->rotation_x(_vertices[a]) * mlx->_size * mlx->add_perspective(za) + mlx->_offset_x;
	s.y = mlx->rotation_y(_vertices[a]) * mlx->_size * mlx->add_perspective(za) + mlx->_offset_y;
	e.x = mlx->rotation_x(_vertices[b]) * mlx->_size * mlx->add_perspective(zb) + mlx->_offset_x;
	e.y = mlx->rotation_y(_vertices[b]) * mlx->_size * mlx->add_perspective(zb) + mlx->_offset_y;
	draw_line(mlx, s, e, false, s, e);
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
	draw_line(mlx, s, e, false, s, e);
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
		link_vertices(mlx, index, index + 1); //FYI possible work with 2 vertex and remember 1 between 0-1 and 1-2
		link_normal(mlx, index);
	}
	link_vertices(mlx, 0, _size - 1);
	link_normal(mlx, _size - 1);

	// display_dir(mlx);
}

void Face::center_object( t_vertex central_axis )
{
	std::vector<t_vertex>::iterator it = _vertices.begin();
	std::vector<t_vertex>::iterator ite = _vertices.end();

	for (; it != ite; it++) {
		(*it).x -= central_axis.x;
		(*it).y -= central_axis.y;
		(*it).z -= central_axis.z;
	}
}
