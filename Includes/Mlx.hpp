#ifndef MLX_HPP
# define MLX_HPP

# include "scop.h"
# include <math.h>
extern "C" {
# include "mlx.h"
}

# if __linux__
#  define WIN_SIZE_X 1800
#  define WIN_SIZE_Y 900
# else
#  define WIN_SIZE_X 2560
#  define WIN_SIZE_Y 1400
# endif

enum {	//events supported on mac (only a fraction of what can be found on x11)
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEDOWN = 4,
	ON_MOUSEUP = 5,
	ON_MOUSEMOVE = 6,
	ON_EXPOSE = 12,
	ON_DESTROY = 17
};

enum { //mouse buttons
	MOUSE_LEFT = 1,
	MOUSE_RIGHT = 2,
	MOUSE_WHEEL_DOWN = 4,
	MOUSE_WHEEL_UP = 5,
	MOUSE_WHEEL_RIGHT = 6,
	MOUSE_WHEEL_LEFT = 7
};

# if __linux__

enum {
	KEY_A = 113,
	KEY_B = 98,
	KEY_C = 99,
	KEY_D = 100,
	KEY_E = 101,
	KEY_F = 102,
	KEY_G = 103,
	KEY_H = 104,
	KEY_I = 105,
	KEY_J = 106,
	KEY_K = 107,
	KEY_L = 108,
	KEY_M = 109,
	KEY_N = 110,
	KEY_O = 111,
	KEY_P = 112,
	KEY_Q = 97,
	KEY_R = 114,
	KEY_S = 115,
	KEY_T = 116,
	KEY_U = 117,
	KEY_V = 118,
	KEY_W = 122,
	KEY_X = 120,
	KEY_Y = 121,
	KEY_Z = 119,
	KEY_ESC = 65307,
	KEY_PLUS = 61,
	KEY_MINUS = 45,
	KEY_UP = 65362,
	KEY_RIGHT = 65363,
	KEY_DOWN = 65364,
	KEY_LEFT = 65361,
	KEY_0 = 65438,
	KEY_1 = 65436,
	KEY_2 = 65433,
	KEY_3 = 65435,
	KEY_4 = 65430,
	KEY_5 = 65437,
	KEY_6 = 65432,
	KEY_7 = 65429,
	KEY_8 = 65431,
	KEY_9 = 65434,
	KEY_MINUS_PAD = 65453,
	KEY_ENTER_PAD = 65421
};

# else

enum {
	KEY_A = 0,
	KEY_B = 11,
	KEY_C = 8,
	KEY_D = 2,
	KEY_E = 14,
	KEY_F = 3,
	KEY_G = 5,
	KEY_H = 4,
	KEY_I = 34,
	KEY_J = 38,
	KEY_K = 40,
	KEY_L = 37,
	KEY_M = 46,
	KEY_N = 45,
	KEY_O = 31,
	KEY_P = 35,
	KEY_Q = 12,
	KEY_R = 15,
	KEY_S = 1,
	KEY_T = 17,
	KEY_U = 32,
	KEY_V = 9,
	KEY_W = 13,
	KEY_X = 7,
	KEY_Y = 16,
	KEY_Z = 6,
	KEY_ESC = 53,
	KEY_PLUS = 24,
	KEY_MINUS = 27,
	KEY_UP = 126,
	KEY_RIGHT = 124,
	KEY_DOWN = 125,
	KEY_LEFT = 123,
	KEY_0 = 82,
	KEY_1 = 83,
	KEY_2 = 84,
	KEY_3 = 85,
	KEY_4 = 86,
	KEY_5 = 87,
	KEY_6 = 88,
	KEY_7 = 89,
	KEY_8 = 91,
	KEY_9 = 92,
	KEY_PLUS_PAD = 69,
	KEY_MINUS_PAD = 78
};

# endif

enum {
	DEFAULT,
	MATERIAL,
	LAST
};

class Mlx {
	private:
		t_vertex _angles, _cos, _sin;
		void *_mlx_ptr, *_win_ptr, *_img_ptr;
		char *_img_addr;
		int _img_bits_per_pixel;
		int _img_line_length;
		int _img_endian;
		int _key_rot_x, _key_rot_y, _key_rot_z, _key_horizontal, _key_vertical, _key_zoom, _key_color, _key_fill, _key_normal;
		Scop *_scop;

		void clear_img( void );

	public:
		Mlx( Scop *scop );
		~Mlx( void );

		int _size, _offset_x, _offset_y, _color_mode;
		bool _fill, _use_normal;

		void put_pixel( int x, int y, unsigned int color );
		double rotation_x( t_vertex vertex );
		double rotation_y( t_vertex vertex );
		void key_down( int kcode );
		void key_released( int kcode );
		void draw( void );
};

#endif
