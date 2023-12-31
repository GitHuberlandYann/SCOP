#include "scop.h"

Face::Face( Material *mat, int generated_color ) : _size(0), _color(0xffffff), _texture_index(NULL)
{
	if (mat) {
		_color = mat->get_color();
		_texture_index = mat->get_texture_index();
	}

	set_rgb(_grays[0], generated_color, generated_color, generated_color);
	set_rgb(_grays[1], generated_color + 50, generated_color - 50, generated_color - 50); //!!this works because gen_col [50:200]
	set_rgb(_grays[2], generated_color - 50, generated_color + 50, generated_color - 50);
	set_rgb(_grays[3], generated_color - 50, generated_color - 50, generated_color + 50);
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

void Face::fill_triangle( Mlx *mlx, t_vertex a, t_vertex b, t_vertex c, bool texture, t_vertex ta, t_vertex tb, t_vertex tc, t_vertex a3d, t_vertex b3d, t_vertex c3d, double za, double zb, double zc )
{
	t_vertex deltta = {0, 0, 0}, delttc = {0, 0, 0}, delt3da = {0, 0, 0}, delt3dc = {0, 0, 0};

	t_vertex starta = {a.x, a.y, 0};
	t_vertex startc = {c.x, c.y, 0};

	t_vertex delta = {fsubstract(b.x, a.x), fsubstract(b.y, a.y), 0};
	delta.z = delta.y / delta.x;
	t_vertex deltc = {fsubstract(b.x, c.x), fsubstract(b.y, c.y), 0};
	deltc.z = deltc.y / deltc.x;
	
	double len, deltza = 0, deltzc = 0;
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
		deltta.x = fsubstract(tb.x, ta.x) / len;
		deltta.y = fsubstract(tb.y, ta.y) / len;
		delttc.x = fsubstract(tb.x, tc.x) / len;
		delttc.y = fsubstract(tb.y, tc.y) / len;
	} else if (mlx->_color_mode == GRADIENT) {
		delt3da.x = fsubstract(b3d.x, a3d.x) / len;
		delt3da.y = fsubstract(b3d.y, a3d.y) / len;
		delt3da.z = fsubstract(b3d.z, a3d.z) / len;
		delt3dc.x = fsubstract(b3d.x, c3d.x) / len;
		delt3dc.y = fsubstract(b3d.y, c3d.y) / len;
		delt3dc.z = fsubstract(b3d.z, c3d.z) / len;
	}
	if (mlx->_depth_enable) {
		deltza = fsubstract(zb, za) / len;
		deltzc = fsubstract(zb, zc) / len;
	}

	for (; len > 0; --len) {
		draw_line(mlx, a, c, texture, ta, tc, a3d, c3d, za, zc);
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
		} else if (mlx->_color_mode == GRADIENT) {
			a3d.x += delt3da.x;
			a3d.y += delt3da.y;
			a3d.z += delt3da.z;
			c3d.x += delt3dc.x;
			c3d.y += delt3dc.y;
			c3d.z += delt3dc.z;
		}
		if (mlx->_depth_enable) {
			za += deltza;
			zc += deltzc;
		}
	}
}

void Face::fill_faces( Mlx *mlx )
{
	double z0 = 0, z1 = 0, z2 = 0;
	t_vertex v0 = _vertices[0], v1, v2;

	if (mlx->_plane_enable) {
		z0 = mlx->rotation_z(v0);
		if (z0 * (1 - 2 * mlx->_plane_side) < mlx->_plane * (1 - 2 * mlx->_plane_side)) {
			return ;
		}
	} else if (mlx->_perspective_enable || mlx->_depth_enable) {
		z0 =  mlx->rotation_z(v0);
	}
	
	t_vertex f0, f1, f2, t0, t1, t2;
	bool texture = (mlx->_color_mode == TEXTURE && !_vertices_textures.empty() && _texture_index && *_texture_index != std::string::npos);
	f0.x = mlx->rotation_x(v0) * mlx->_size * mlx->add_perspective(z0) + mlx->_offset_x;
	f0.y = mlx->rotation_y(v0) * mlx->_size * mlx->add_perspective(z0) + mlx->_offset_y;

	if (texture) {
		t0 = mlx->set_textvert(_vertices_textures[0], *_texture_index);
	}
	for (int index = 1; index < _size - 1; ++index) {
		v1 = _vertices[index];
		v2 = _vertices[index + 1];
		if (mlx->_perspective_enable || mlx->_depth_enable) {
			z1 = mlx->rotation_z(v1);
			z2 = mlx->rotation_z(v2);
		}
		f1.x = mlx->rotation_x(v1) * mlx->_size * mlx->add_perspective(z1) + mlx->_offset_x;
		f1.y = mlx->rotation_y(v1) * mlx->_size * mlx->add_perspective(z1) + mlx->_offset_y;
		f2.x = mlx->rotation_x(v2) * mlx->_size * mlx->add_perspective(z2) + mlx->_offset_x;
		f2.y = mlx->rotation_y(v2) * mlx->_size * mlx->add_perspective(z2) + mlx->_offset_y;
		if (texture) {
			t1 = mlx->set_textvert(_vertices_textures[index], *_texture_index);
			t2 = mlx->set_textvert(_vertices_textures[index + 1], *_texture_index);
		}
		fill_triangle(mlx, f0, f1, f2, texture, t0, t1, t2, v0, v1, v2, z0, z1, z2);
	}
}

void Face::draw_line( Mlx *mlx, t_vertex a, t_vertex b, bool texture, t_vertex ta, t_vertex tb, t_vertex a3d, t_vertex b3d, double za, double zb )
{
	t_vertex delta, pixel, deltt = {0, 0, 0}, delt3d = {0, 0, 0};
	double len, deltz = 0;

	delta.x = fsubstract(b.x, a.x);
	delta.y = fsubstract(b.y, a.y);
	delta.z = delta.y / delta.x;
	len = sqrt(delta.x * delta.x + delta.y * delta.y);
	delta.x /= len;
	delta.y /= len;
	pixel.x = a.x;
	pixel.y = a.y;

	if (texture) {
		deltt.x = fsubstract(tb.x, ta.x) / len;
		deltt.y = fsubstract(tb.y, ta.y) / len;
	} else if (mlx->_color_mode == GRADIENT) {
		delt3d.x = fsubstract(b3d.x, a3d.x) / len;
		delt3d.y = fsubstract(b3d.y, a3d.y) / len;
		delt3d.z = fsubstract(b3d.z, a3d.z) / len;
	}
	if (mlx->_depth_enable) {
		deltz = fsubstract(zb, za) / len;
	}

	while (len > 0)
	{
		if (mlx->_color_mode == DEFAULT) {
			mlx->put_pixel(pixel.x, pixel.y, 0xffffff, za);
		} else if (mlx->_color_mode == MATERIAL) {
			mlx->put_pixel(pixel.x, pixel.y, _color, za);
		} else if (texture) {
			mlx->put_pixel(pixel.x, pixel.y, mlx->get_texture(*_texture_index, ta.x, ta.y), za);
			ta.x += deltt.x;
			ta.y += deltt.y;
		} else if (mlx->_color_mode == GRAY) {
			mlx->put_pixel(pixel.x, pixel.y, _grays[mlx->_shade], za);
		} else if (mlx->_color_mode == GRADIENT) {
			mlx->put_pixel(pixel.x, pixel.y, get_gradient(a3d), za);
			a3d.x += delt3d.x;
			a3d.y += delt3d.y;
			a3d.z += delt3d.z;
		} else {
			mlx->put_pixel(pixel.x, pixel.y, 0x950321, za);
		}
		pixel.x += delta.x;
		if (delta.x)
			pixel.y = delta.z * (pixel.x - a.x) + a.y;
		else
			pixel.y += delta.y;
		if (mlx->_depth_enable) {
			za += deltz;
		}
		--len;
	}
}

void Face::link_vertices( Mlx *mlx, int a, int b )
{
	double za = 0, zb = 0;
	t_vertex va = _vertices[a], vb = _vertices[b];

	if (mlx->_plane_enable) {
		za = mlx->rotation_z(va);
		if (za * (1 - 2 * mlx->_plane_side) < mlx->_plane * (1 - 2 * mlx->_plane_side)) {
			return ;
		}
		zb = mlx->rotation_z(vb);
		if (zb * (1 - 2 * mlx->_plane_side) < mlx->_plane * (1 - 2 * mlx->_plane_side)) {
			return ;
		}
	} else if (mlx->_perspective_enable || mlx->_depth_enable) {
		za = mlx->rotation_z(va);
		zb = mlx->rotation_z(vb);
	}

	t_vertex s, e;
	s.x = mlx->rotation_x(va) * mlx->_size * mlx->add_perspective(za) + mlx->_offset_x;
	s.y = mlx->rotation_y(va) * mlx->_size * mlx->add_perspective(za) + mlx->_offset_y;
	e.x = mlx->rotation_x(vb) * mlx->_size * mlx->add_perspective(zb) + mlx->_offset_x;
	e.y = mlx->rotation_y(vb) * mlx->_size * mlx->add_perspective(zb) + mlx->_offset_y;
	draw_line(mlx, s, e, false, s, e, va, vb, za, zb);
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

void Face::link_face_normal( Mlx *mlx )
{
	t_vertex s, e, norm;
	
	set_vertex(norm, _face_center.x + _face_normal.x, _face_center.y + _face_normal.y, _face_center.z + _face_normal.z);

	double zs = 0, ze = 0;
	if (mlx->_plane_enable) {
		zs = mlx->rotation_z(_face_center);
		if (zs * (1 - 2 * mlx->_plane_side) < mlx->_plane * (1 - 2 * mlx->_plane_side)) {
			return ;
		}
		ze = mlx->rotation_z(norm);
		if (ze * (1 - 2 * mlx->_plane_side) < mlx->_plane * (1 - 2 * mlx->_plane_side)) {
			return ;
		}
	} else if (mlx->_depth_enable) {
		zs = mlx->rotation_z(_face_center);
		ze = mlx->rotation_z(norm);
	}
	
	s.x = mlx->rotation_x(_face_center) * mlx->_size + mlx->_offset_x;
	s.y = mlx->rotation_y(_face_center) * mlx->_size + mlx->_offset_y;
	e.x = mlx->rotation_x(norm) * mlx->_size + mlx->_offset_x;
	e.y = mlx->rotation_y(norm) * mlx->_size + mlx->_offset_y;
	draw_line(mlx, s, e, false, s, e, _face_center, norm, zs, ze);
}

void Face::link_normal( Mlx *mlx, int index )
{
	if (mlx->_show_normals != N_POINTS || mlx->_perspective_enable || _vertices_normals.empty()) {
		return ;
	}

	t_vertex s, e;
	t_vertex vi = _vertices[index];
	t_vertex n = {vi.x + _vertices_normals[index].x * 5,
					vi.y + _vertices_normals[index].y * 5,
					vi.z + _vertices_normals[index].z * 5};
	
	double zs = 0, ze = 0;
	if (mlx->_plane_enable) {
		zs = mlx->rotation_z(vi);
		if (zs * (1 - 2 * mlx->_plane_side) < mlx->_plane * (1 - 2 * mlx->_plane_side)) {
			return ;
		}
		ze = mlx->rotation_z(n);
		if (ze * (1 - 2 * mlx->_plane_side) < mlx->_plane * (1 - 2 * mlx->_plane_side)) {
			return ;
		}
	} else if (mlx->_depth_enable) {
		zs = mlx->rotation_z(vi);
		ze = mlx->rotation_z(n);
	}
	
	s.x = mlx->rotation_x(vi) * mlx->_size + mlx->_offset_x;
	s.y = mlx->rotation_y(vi) * mlx->_size + mlx->_offset_y;
	e.x = mlx->rotation_x(n) * mlx->_size + mlx->_offset_x;
	e.y = mlx->rotation_y(n) * mlx->_size + mlx->_offset_y;
	draw_line(mlx, s, e, false, s, e, vi, n, zs, ze);
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

/* use vn (if given) of points of face to compute one normal to the face */
void Face::set_face_normal( void )
{
	set_vertex(_face_normal, 0, 0, 0);
	set_vertex(_face_center, 0, 0, 0);
	if (_vertices_normals.empty()) {
		return ;
	}
	for (int index = 0; index < _size; index++) {
		_face_normal.x += _vertices_normals[index].x * 5;
		_face_normal.y += _vertices_normals[index].y * 5;
		_face_normal.z += _vertices_normals[index].z * 5;
		_face_center.x += _vertices[index].x;
		_face_center.y += _vertices[index].y;
		_face_center.z += _vertices[index].z;
	}
	_face_normal.x /= _size;
	_face_normal.y /= _size;
	_face_normal.z /= _size;
	_face_center.x /= _size;
	_face_center.y /= _size;
	_face_center.z /= _size;
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
		norm.x = mlx->rotation_x(_face_normal);
		norm.y = mlx->rotation_y(_face_normal);
		norm.z = mlx->rotation_z(_face_normal);
		scalar = mlx->_dir.x * norm.x + mlx->_dir.y * norm.y + mlx->_dir.z * norm.z;
		if (scalar > 0) {
			return ;
		}
	}

	if (mlx->_fill) {
		return (fill_faces(mlx));
	}
	for (int index = 0; index < _size - 1; index++) {
		link_vertices(mlx, index, index + 1); //FYI possible to work with 2 vertices and remember 1 between 0-1 and 1-2
		link_normal(mlx, index);
	}
	link_vertices(mlx, 0, _size - 1);
	link_normal(mlx, _size - 1);

	if (mlx->_show_normals == N_FACES && !mlx->_perspective_enable && !_vertices_normals.empty()) {
		link_face_normal(mlx);
	}

	// display_dir(mlx);
}

void Face::center_object( t_vertex central_axis, double normalizer )
{
	std::vector<t_vertex>::iterator it = _vertices.begin();
	std::vector<t_vertex>::iterator ite = _vertices.end();

	for (; it != ite; it++) {
		set_vertex(*it, (it->x - central_axis.x) * normalizer,
						(it->y - central_axis.y) * normalizer,
						(it->z - central_axis.z) * normalizer);
	}
}
