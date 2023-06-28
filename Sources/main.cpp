#include "scop.h"

static std::string get_file( int ac, char **av )
{
	if (ac == 1) {
		return ("Resources/planet.obj");
	} else if (ac == 2) {
		std::string res(av[1]);

		if (res.size() >= 4 && (!res.compare(res.size() - 4, 4, ".obj") || !res.compare(res.size() - 4, 4, ".OBJ"))) {
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
	Mlx *mlx = new Mlx(scop);
	try {
		scop->parse(file);
		scop->center_object();
		scop->display_content();
		mlx->setup();
	} catch (std::exception & e) {
		std::cout << e.what() << std::endl;
		delete mlx;
		delete scop;
		std::string strwhat = e.what();
		return (strwhat.compare(0, 22, "[QuitProgramException]"));
	}
	delete scop;
	return (0);
}
