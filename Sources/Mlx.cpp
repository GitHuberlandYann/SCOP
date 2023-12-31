#include "scop.h"

Mlx::Mlx( Scop *scop ) : _mlx_ptr(NULL), _win_ptr(NULL), _img_ptr(NULL),
		_key_rot_x(0), _key_rot_y(0), _key_rot_z(0), _key_horizontal(0), _key_vertical(0),
		_key_zoom(0), _key_color(0), _key_fill(0), _key_normal(0), _key_show_normals(0),
		_key_plane_enable(0), _key_plane(0), _key_plane_side(0),
		_key_perpective_enable(0), _key_reset(0), _key_shade(0), _key_depth_enable(0),
		_key_show_box(0), _show_box(false),
		_scop(scop), _size(5), _offset_x(WIN_SIZE_X / 2), _offset_y(WIN_SIZE_Y / 2),
		_color_mode(DEFAULT), _shade(S_WHITE), _show_normals(N_FALSE), _fill(false), _use_normal(false),
		_plane_enable(false), _plane_side(false), _perspective_enable(false), _depth_enable(false),
		_plane(0)
{
	std::cout << "Constructor of Mlx called" << std::endl;
	set_vertex(_angles, 0, 0, M_PI);
	set_vertex(_cos, 1, 1, -1);
	set_vertex(_sin, 0, 0, 0);
	set_vertex(_dir, 0, 0, -1);
}

// int	mlx_destroy_window(void *mlx_ptr, void *win_ptr);
// int	mlx_destroy_image(void *mlx_ptr, void *img_ptr);
// int	mlx_destroy_display(void *mlx_ptr);
Mlx::~Mlx( void )
{
	std::cout << "Destructor of Mlx called" << std::endl;
	if (_img_ptr) {
		mlx_destroy_image(_mlx_ptr, _img_ptr);
	}

	std::vector<t_img *>::iterator it = _xpms.begin();
	std::vector<t_img *>::iterator ite = _xpms.end();
	
	for (; it != ite; it++) {
		mlx_destroy_image(_mlx_ptr, (*it)->img_ptr);
		delete *it;
	}
	_xpms.clear();

	if (_win_ptr) {
		mlx_destroy_window(_mlx_ptr, _win_ptr);
	}
	if (_mlx_ptr) {
		free(_mlx_ptr);
	}
}

// ************************************************************************** //
//                                  Private                                   //
// ************************************************************************** //

void Mlx::clear_img( void )
{
	bool saved_depth_enable = _depth_enable;
	_depth_enable = false;
	for (int x = 0; x < WIN_SIZE_X; x++) {
		for (int y = 0; y < WIN_SIZE_Y; y++) {
			put_pixel(x, y, 0x606060, 0);
		}
	}
	_depth_enable = saved_depth_enable;
}

int Mlx::check_duplicate_xpm( std::vector<Material *>::iterator it, std::vector<Material *>::iterator ite )
{
	std::string current_xpm = (*ite)->get_xpm();
	for (; it != ite; it++) {
		if (!current_xpm.compare((*it)->get_xpm())) {
			(*ite)->set_texture_index(*(*it)->get_texture_index());
			return (0);
		}
	}
	return (1);
}

// ************************************************************************** //
//                                  Public                                    //
// ************************************************************************** //

void Mlx::setup( void ) {

	_mlx_ptr = mlx_init();
	if (!_mlx_ptr) {
		throw MlxCallException();
	}
	std::string title = "scop";
	char *title_str = &title[0];
	_win_ptr =  mlx_new_window(_mlx_ptr, WIN_SIZE_X, WIN_SIZE_Y, title_str);
	if (!_win_ptr) {
		throw MlxCallException();
	}
	_img_ptr = mlx_new_image(_mlx_ptr, WIN_SIZE_X, WIN_SIZE_Y);
	if (!_img_ptr) {
		throw MlxCallException();
	}
	_img_addr = mlx_get_data_addr(_img_ptr, &_img_bits_per_pixel,
			&_img_line_length, &_img_endian);
	
	std::vector<Material *>::iterator it = _scop->_materials.begin();
	std::vector<Material *>::iterator ite = _scop->_materials.end();

	size_t cnt = 0, dup_cnt = 0;;
	for (; it != ite; it++) {
		if ((*it)->get_xpm().empty()) {
			continue ;
		} else if (!check_duplicate_xpm(_scop->_materials.begin(), it)) {
			++dup_cnt;
			continue ;
		}
		t_img *nimg = new t_img;
		_xpms.push_back(nimg);
		title = _scop->_root + (*it)->get_xpm();
		title_str = &title[0];
		nimg->img_ptr = mlx_xpm_file_to_image(_mlx_ptr,
			title_str, &nimg->width, &nimg->height);
		if (!nimg->img_ptr) {
			std::cout << "xpm file: " << title << std::endl;
			throw MlxCallException();
		}
		nimg->addr = mlx_get_data_addr(nimg->img_ptr, &nimg->bits_per_pixel,
				&nimg->line_length, &nimg->endian);
		(*it)->set_texture_index(cnt);
		++cnt;
	}
	(!dup_cnt)
		? std::cout << " ---- number of xpms: " << cnt << " ----" << std::endl << std::endl
		: std::cout << " ---- number of xpms: " << cnt + dup_cnt << " -> " << cnt << " ----" << std::endl << std::endl;

	_scop->map_img(this);
	mlx_put_image_to_window(_mlx_ptr, _win_ptr, _img_ptr, 0, 0);
	mlx_hook(_win_ptr, ON_KEYDOWN, 1 << 0, event_key_down, this);
	mlx_hook(_win_ptr, ON_KEYUP, 1l << 1, event_key_released, this);
	mlx_hook(_win_ptr, ON_DESTROY, 0, mlx_exit, NULL);
	Mlx *test = this;
	mlx_loop_hook(_mlx_ptr, event_mlx_draw, test);
	mlx_loop(_mlx_ptr);
}

void Mlx::put_pixel( int x, int y, unsigned int color, double depth )
{
	char *dst;

	if (y < 0 || y >= WIN_SIZE_Y || x < 0 || x >= WIN_SIZE_X) {
		return ;
	}
	if (_depth_enable) {
		if (_depth[y * WIN_SIZE_X + x] > depth)
			return ;
		_depth[y * WIN_SIZE_X + x] = depth;
	}
	dst = _img_addr + (y * _img_line_length + x * (_img_bits_per_pixel / 8));
	*(unsigned int *) dst = color;
}

void Mlx::put_text( std::string str )
{
	if (_text_y > WIN_SIZE_Y - 16) {
		return ;
	}
	char *string_to_put = &str[0];
	mlx_string_put(_mlx_ptr, _win_ptr, TEXT_RIGHT, _text_y, 0xffffff, string_to_put);
	_text_y += 16;
}

unsigned int Mlx::get_texture( size_t texture_index, int x, int y )
{
	char	*dst;
	int		limit_x;
	int		limit_y;

	t_img *img = _xpms[texture_index];
	limit_x = img->width;
	limit_y = img->height;
	if (y < 0 || y >= limit_y || x < 0 || x >= limit_x)
		return (0x0);
	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	return (*(unsigned int *) dst);
}

t_vertex Mlx::set_textvert( t_vertex base_vertex, size_t texture_index )
{
	base_vertex.x *= _xpms[texture_index]->width;
	base_vertex.y *= _xpms[texture_index]->height;
	return (base_vertex);
}

double Mlx::rotation_x( t_vertex vertex )
{
	double	res;

	res = _cos.y * _cos.z * vertex.x;
	res += (_sin.x * _sin.y * _cos.z - _cos.x * _sin.z) * vertex.y;
	res += (_cos.x * _sin.y * _cos.z + _sin.x * _sin.z) * vertex.z;
	return (res);
}

double Mlx::rotation_y( t_vertex vertex )
{
	double	res;

	res = _cos.y * _sin.z * vertex.x;
	res += (_sin.x * _sin.y * _sin.z + _cos.x * _cos.z) * vertex.y;
	res += (_cos.x * _sin.y * _sin.z - _sin.x * _cos.z) * vertex.z;
	return (res);
}

double Mlx::rotation_z( t_vertex vertex )
{
	double res;
	
	res = -_sin.y * vertex.x;
	res += _sin.x * _cos.y * vertex.y;
	res += _cos.x * _cos.y * vertex.z;
	return (res);
}

double Mlx::add_perspective( double z_value )
{
	if (!_perspective_enable) {
		return (1);
	}
	return (1 + z_value / EXTREMUM / 1.5);
}

void Mlx::key_down( int kcode )
{
	// std::cout << "kcode: " << kcode << std::endl;
	if (kcode == KEY_E || kcode == KEY_R)
		_key_rot_z = (kcode == KEY_E) - (kcode == KEY_R);
	else if (kcode == KEY_W || kcode == KEY_S)
		_key_rot_y = (kcode == KEY_W) - (kcode == KEY_S);
	else if (kcode == KEY_A || kcode == KEY_D)
		_key_rot_x = (kcode == KEY_A) - (kcode == KEY_D);
	else if (kcode == KEY_LEFT || kcode == KEY_RIGHT)
		_key_horizontal = (kcode == KEY_RIGHT) - (kcode == KEY_LEFT);
	else if (kcode == KEY_UP || kcode == KEY_DOWN)
		_key_vertical = (kcode == KEY_DOWN) - (kcode == KEY_UP);
	else if (kcode == KEY_PLUS || kcode == KEY_MINUS)
		_key_zoom = (kcode == KEY_PLUS) - (kcode == KEY_MINUS);
	else if (kcode == KEY_C && ++_key_color == 1) {
		++_color_mode;
		if (_color_mode == LAST)
			_color_mode = DEFAULT;
	} else if (kcode == KEY_X && ++_key_shade == 1) {
		++_shade;
		if (_shade == S_LAST)
			_shade = S_WHITE;
	}
	else if (kcode == KEY_F && ++_key_fill == 1)
		_fill = !_fill;
	else if (kcode == KEY_V && ++_key_normal == 1)
		_use_normal = !_use_normal;
	else if (kcode == KEY_B && ++_key_depth_enable == 1)
		_depth_enable = !_depth_enable;
	else if (kcode == KEY_U && ++_key_show_box == 1) {
		_show_box = !_show_box;
	} else if (kcode == KEY_N && ++_key_show_normals == 1) {
		++_show_normals;
		if (_show_normals == N_LAST)
			_show_normals = N_FALSE;
	}
	else if (kcode == KEY_O && ++_key_reset == 1) {
		_size = 5;
		_offset_x = WIN_SIZE_X / 2;
		_offset_y = WIN_SIZE_Y / 2;
		set_vertex(_angles, 0, 0, M_PI);
		set_vertex(_cos, 1, 1, -1);
		set_vertex(_sin, 0, 0, 0);
		_color_mode = DEFAULT;
		_shade = S_WHITE;
		_fill = false;
		_use_normal = false;
		_show_normals = false;
		_plane_enable = false;
		_plane_side = false;
		_perspective_enable = false;
		_show_box = false;
		_depth_enable = false;
		_plane = 0;
	} else if (kcode == KEY_P && ++_key_perpective_enable == 1)
		_perspective_enable = !_perspective_enable;
	else if (kcode == KEY_ASTERISK && ++_key_plane_enable == 1)
		_plane_enable = !_plane_enable;
	else if (kcode == KEY_ENTER_PAD && ++_key_plane_side == 1)
		_plane_side = !_plane_side;
	else if (kcode == KEY_PLUS_PAD || kcode == KEY_MINUS_PAD)
		_key_plane = (kcode == KEY_PLUS_PAD) - (kcode == KEY_MINUS_PAD);
}

void Mlx::key_released( int kcode )
{
	if (kcode == KEY_E || kcode == KEY_R)
		_key_rot_z = 0;
	else if (kcode == KEY_W || kcode == KEY_S)
		_key_rot_y = 0;
	else if (kcode == KEY_A || kcode == KEY_D)
		_key_rot_x = 0;
	else if (kcode == KEY_LEFT || kcode == KEY_RIGHT)
		_key_horizontal = 0;
	else if (kcode == KEY_UP || kcode == KEY_DOWN)
		_key_vertical = 0;
	else if (kcode == KEY_PLUS || kcode == KEY_MINUS)
		_key_zoom = 0;
	else if (kcode == KEY_C)
		_key_color = 0;
	else if (kcode == KEY_X)
		_key_shade = 0;
	else if (kcode == KEY_F)
		_key_fill = 0;
	else if (kcode == KEY_V)
		_key_normal = 0;
	else if (kcode == KEY_U)
		_key_show_box = 0;
	else if (kcode == KEY_N)
		_key_show_normals = 0;
	else if (kcode == KEY_B)
		_key_depth_enable = 0;
	else if (kcode == KEY_O)
		_key_reset = 0;
	else if (kcode == KEY_P)
		_key_perpective_enable = 0;
	else if (kcode == KEY_ASTERISK)
		_key_plane_enable = 0;
	else if (kcode == KEY_ENTER_PAD)
		_key_plane_side = 0;
	else if (kcode == KEY_PLUS_PAD || kcode == KEY_MINUS_PAD)
		_key_plane = 0;
}

void Mlx::draw( void )
{
	if (_key_rot_x)
	{
		_angles.x += 2 * (M_PI / 180) * _key_rot_x;
		_sin.x = sin(_angles.x);
		_cos.x = cos(_angles.x);
	}
	if (_key_rot_y)
	{
		_angles.y += 2 * (M_PI / 180) * _key_rot_y;
		_sin.y = sin(_angles.y);
		_cos.y = cos(_angles.y);
	}
	if (_key_rot_z)
	{
		_angles.z += 2 * (M_PI / 180) * _key_rot_z;
		_sin.z = sin(_angles.z);
		_cos.z = cos(_angles.z);
	}
	_offset_x += 10 * _key_horizontal;
	_offset_y += 10 * _key_vertical;
	if (_size + _key_zoom > 0) {
		_size += _key_zoom;
	}
	_plane += _key_plane * 0.5;
	_text_y = 16;

	clear_img();
	if (_depth_enable) {
		_depth.fill(DEPTH);
	}
	_scop->map_img(this);
	if (_show_box) {
		_scop->map_box(this);
	}
	// put_pixel(WIN_SIZE_X / 2, WIN_SIZE_Y /2, 0xff0000);
	mlx_put_image_to_window(_mlx_ptr, _win_ptr, _img_ptr, 0, 0);
}
