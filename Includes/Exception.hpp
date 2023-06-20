#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include "scop.h"

class InvalidFileException : public std::exception
{
	public:
		const char *what() const throw();
};

class InvalidVertexException : public std::exception
{
	public:
		const char *what() const throw();
};

#endif
