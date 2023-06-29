#include "marching.h"

std::string	trim_spaces( std::string str )
{
	int index = 0;
	std::string new_string;
	while (str[index] == ' ' || str[index] == '\t')
		++index;
	while (str[index])
	{
		if (str[index] == '\t')
			new_string += ' ';
		else
			new_string += str[index];
		if (str[index] == ' ' || str[index] == '\t')
		{
			while (str[index] == ' ' || str[index] == '\t')
				++index;
			--index;
		}
		++index;
	}
	if (new_string[new_string.size() - 1] == ' ')
		new_string = new_string.substr(0, new_string.size() - 1);
	return (new_string);
}

void custom_swap( double arr[8], int a, int b )
{
	double tmp = arr[a];
	arr[a] = arr[b];
	arr[b] = tmp;
}
