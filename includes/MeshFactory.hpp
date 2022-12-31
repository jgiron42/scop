//
// Created by joachim on 12/14/22.
//

#ifndef SCOP_MESHFACTORY_HPP
#define SCOP_MESHFACTORY_HPP
#include "ObjectFactory.hpp"
#include "MeshData.hpp"

class MeshFactory : public ObjectFactory
{
	void build();
	public:
	MeshFactory(MeshData const &);
};


#endif //SCOP_MESHFACTORY_HPP
