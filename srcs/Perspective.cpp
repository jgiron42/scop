#include "../includes/Perspective.hpp"
#include <cmath>

Perspective::Perspective(float new_near, float new_far, float new_fov, float new_aspect_ratio) : fov(new_fov), aspect_ratio(new_aspect_ratio), near(new_near), far(new_far) {}

Matrix<float> Perspective::compute() const {
	float h = 1.f/(tanf(this->fov / 2));
	float w = h * this->aspect_ratio;
	return Matrix<float>(
			{
					{w, 0, 0, 0},
					{0, h, 0, 0},
					{0, 0, this->far / (this->near - this->far), ((this->near * this->far) / (this->near - this->far))},
					{0, 0, -1, 0}
			}
		);
}

void Perspective::setFov(float new_fov) {
	this->fov = new_fov;
}

void Perspective::setAspectRatio(float new_aspect_ratio) {
	this->aspect_ratio = new_aspect_ratio;
}