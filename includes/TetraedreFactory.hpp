#ifndef SCOP_TETRAEDREFACTORY_HPP
# define SCOP_TETRAEDREFACTORY_HPP
# include "ObjectFactory.hpp"

typedef struct {
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
}	vertex;


class TetraedreFactory : public ObjectFactory {
	void build();
public:
	TetraedreFactory();
};

#endif //SCOP_TETRAEDREFACTORY_HPP
