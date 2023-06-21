#include "scop.h"

static std::string get_file( int ac, char **av )
{
	if (ac == 1) {
		return ("Ressources/planet.obj");
	} else if (ac == 2) {
		std::string res(av[1]);

		if (res.size() >= 4 && !res.compare(res.size() - 4, 4, ".obj")) {
			return (res);
		}
	}
	std::cerr << "Usage: ./scop <path-to-file>.obj" << std::endl;
	return ("");
}

static std::string get_root( std::string file )
{
	size_t index = file.rfind('/');
	if (index == std::string::npos)
		return ("");
	return (file.substr(0, index + 1));
}

int main( int ac, char **av )
{
	std::string file = get_file(ac, av);
	if (file.empty())
		return (1);

	Scop *scop = new Scop(get_root(file));
	try {
		scop->parse(file);
		scop->display_content();
		Mlx mlx(scop);
	} catch (std::exception & e) {
		std::cout << e.what() << std::endl;
		delete scop;
		exit(1); //remove this to see leaks when quit
	}
	delete scop;
	return (0);
}
