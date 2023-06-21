#ifndef MATERIAL_HPP
# define MATERIAL_HPP

class Material {
	private:
		std::string _name;
		unsigned int _color; //kd
		size_t _texture_index; //map_kd

		void set_rgb( std::string line );

	public:
		Material( std::string name, std::ifstream & indata );
		~Material( void );

		unsigned int get_color( void );

};

#endif
