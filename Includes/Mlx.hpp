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

# define EXTREMUM 50.0
# define DEPTH -1000.0
# define TEXT_RIGHT (WIN_SIZE_X - 300)

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
	KEY_ASTERISK = 65450,
	KEY_PLUS_PAD = 65451,
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
	KEY_ASTERISK = 67,
	KEY_PLUS_PAD = 69,
	KEY_MINUS_PAD = 78,
	KEY_ENTER_PAD = 76
};

# endif

enum color_mode {
	DEFAULT,
	MATERIAL,
	TEXTURE,
	GRAY,
	GRADIENT,
	LAST
};

enum shades {
	S_WHITE,
	S_RED,
	S_GREEN,
	S_BLUE,
	S_LAST
};

enum normals {
	N_FALSE,
	N_POINTS,
	N_FACES,
	N_LAST
};

typedef struct s_img {
	void	*img_ptr;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}				t_img;

class Mlx {
	private:
		t_vertex _angles, _cos, _sin;
		void *_mlx_ptr, *_win_ptr, *_img_ptr;
		char *_img_addr;
		int _img_bits_per_pixel;
		int _img_line_length;
		int _img_endian;
		int _key_rot_x, _key_rot_y, _key_rot_z, _key_horizontal, _key_vertical, _key_zoom,
			_key_color, _key_fill, _key_normal, _key_show_normals, _key_plane_enable,
			_key_plane, _key_plane_side, _key_perpective_enable, _key_reset,
			_key_shade, _key_depth_enable, _key_show_box;
		bool _show_box;
		Scop *_scop;
		std::array<double, WIN_SIZE_X * WIN_SIZE_Y> _depth;
		int _text_y;

		void clear_img( void );
		int check_duplicate_xpm( std::vector<Material *>::iterator it, std::vector<Material *>::iterator ite );

	public:
		Mlx( Scop *scop );
		~Mlx( void );

		int _size, _offset_x, _offset_y, _color_mode, _shade, _show_normals;
		bool _fill, _use_normal, _plane_enable, _plane_side, _perspective_enable, _depth_enable;
		double _plane;
		t_vertex _dir;
		std::vector<t_img *> _xpms;

		void setup( void );
		void put_pixel( int x, int y, unsigned int color, double depth );
		void put_text( std::string str );
		unsigned int get_texture( size_t texture_index, int x, int y );
		t_vertex set_textvert( t_vertex base_vertex, size_t texture_index );
		double rotation_x( t_vertex vertex );
		double rotation_y( t_vertex vertex );
		double rotation_z( t_vertex vertex );
		double add_perspective( double z_value );
		void key_down( int kcode );
		void key_released( int kcode );
		void draw( void );
};

#endif
