#ifndef SCOP_PERSPECTIVE_HPP
#define SCOP_PERSPECTIVE_HPP
#include "Transformation.hpp"

class Perspective : public Transformation {
private:
	float	fov;
	float	aspect_ratio;
	float	near;
	float	far;
protected:
public:
	Matrix<float> compute() const override;
	Perspective(float new_near, float new_far, float new_fov = M_PI_4, float new_aspect_ratio = 1);
	void	setFov(float new_fov);
	void	setAspectRatio(float new_aspect_ratio);
};


#endif //SCOP_PERSPECTIVE_HPP