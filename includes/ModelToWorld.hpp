#ifndef SCOP_MODEL_TO_WORLD_HPP
#define SCOP_MODEL_TO_WORLD_HPP
# include "Vector.hpp"
# include "Transformation.hpp"
# include "Matrix.hpp"
# include <array>
class ModelToWorld : public Transformation {
private:
protected:
	Matrix<float> compute() const override;
public:
	Matrix<float> rot;
	Matrix<float> trans;
	Matrix<float> scale;
	ModelToWorld();

	ModelToWorld & rotate(float xrot, float yrot, float zrot);
	ModelToWorld & translate(const Vector<float> &trans);
	ModelToWorld & resize(float size_factor);

	ModelToWorld & reset(void);

	ModelToWorld & setAngle(float xrot, float yrot, float zrot);
	ModelToWorld & setPos(const Vector<float> &trans);
	ModelToWorld & setSize(float size_factor);
};


#endif
