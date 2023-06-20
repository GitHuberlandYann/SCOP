#ifndef SCOP_HPP
# define SCOP_HPP

# include "scop.h"

class Scop {
	private:
		std::vector<t_vertex> _vertices;

		void add_vertex( std::string line );

	public:
		Scop( void );
		~Scop( void );

		void parse( std::string file );
		void display_content( void );
};


#endif
