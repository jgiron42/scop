#ifndef SCOP_OBJECT_HPP
#define SCOP_OBJECT_HPP
# include "ObjectFactory.hpp"
# include "Matrix.hpp"
# include "ModelToWorld.hpp"
# include "Transformation.hpp"

class ObjectFactory;

class Object{
private:
	const ObjectFactory		&factory;
	ModelToWorld			position;
public:
	float 					texture_switch;
	Object(const ObjectFactory &f);
	Object & rotate(float xrot, float yrot, float zrot);
	Object & translate(const Vector<float> &trans);
	Object & resize(float size_factor);

	Object & reset(void);

	Object & setAngle(float xrot, float yrot, float zrot);
	Object & setPos(const Vector<float> &trans);
	Object & setSize(float size_factor);

	void draw(const Transformation& t = Transformation());
};
#endif //SCOP_OBJECT_HPP
