#ifndef SCOP_COMMON_HPP
# define SCOP_COMMON_HPP
#include <iostream>
#include <string>

template <typename T>
class Vector;
template <typename T>
class Matrix;

class incompatible_exception : public std::exception {
	[[nodiscard]] const char * what() const noexcept override
	{
		return "Incompatible elements";
	}
};

#endif