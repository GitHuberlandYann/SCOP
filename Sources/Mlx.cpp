#include "scop.h"

Mlx::Mlx( Scop *scop ) : _angles{0, M_PI, 0}, _cos{1, -1, 1}, _sin{0, 0, 0}, _size(5),
		_offset_x(WIN_SIZE_X / 2), _offset_y(WIN_SIZE_Y / 2), _color_mode(DEFAULT), _fill(false)
{
	std::cout << "Constructor of Mlx called" << std::endl;
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

	scop->map_img(this);
	mlx_put_image_to_window(_mlx_ptr, _win_ptr, _img_ptr, 0, 0);
	// mlx_hook(_win_ptr, ON_KEYDOWN, 1 << 0, key_down, scop);
	// mlx_hook(_win_ptr, ON_KEYUP, 1l << 1, key_released, scop);
	// mlx_hook(_win_ptr, ON_DESTROY, 0, mlx_exit, NULL);
	// mlx_loop_hook(_mlx_ptr, mlx_draw, scop);
	mlx_loop(_mlx_ptr);
}

Mlx::~Mlx( void )
{
	std::cout << "Destructor of Mlx called" << std::endl;
}

// ************************************************************************** //
//                                  Public                                    //
// ************************************************************************** //

void Mlx::clear_img( void )
{
	for (int x = 0; x < WIN_SIZE_X; x++) {
		for (int y = 0; y < WIN_SIZE_Y; y++) {
			put_pixel(x, y, 0x0);
		}
	}
}

void Mlx::put_pixel( int x, int y, unsigned int color )
{
	char *dst;

	if (y < 0 || y >= WIN_SIZE_Y || x < 0 || x >= WIN_SIZE_X) {
		return ;
	}
	dst = _img_addr + (y * _img_line_length + x * (_img_bits_per_pixel / 8));
	*(unsigned int *) dst = color;
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
