#ifndef MATERIAL_HPP
# define MATERIAL_HPP

class Material {
	private:
		std::string _name;
		int _color[3]; //kd
		size_t _texture_index; //map_kd

		void set_rgb( std::string line );

	public:
		Material( std::string name, std::ifstream & indata );
		~Material( void );

};

#endif
