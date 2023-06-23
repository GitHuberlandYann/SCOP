#ifndef UTILS_H
# define UTILS_H

std::string	trim_spaces(std::string str);
void display_special_characters(std::string str);
t_vertex parse_vertex( std::string line, size_t index, bool texture );
int mlx_exit( int ignore, void *ignore_too );
int event_key_down( int kcode, void *param );
int event_key_released( int kcode, void *param );
int event_mlx_draw( void *param );
void set_vertex( t_vertex & vertex, double x, double y, double z );
void set_rgb( unsigned int & color, int r, int g, int b );

#endif
