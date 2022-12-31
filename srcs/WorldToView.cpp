#include "../includes/WorldToView.hpp"
#include "Vector.hpp"
WorldToView::WorldToView() : cpos({0, 0, -3}), ctar({0, 0, 1}), up({0, 1, 0}) {}

Matrix<float> WorldToView::compute() const {
	Vector<float> up_normalized = up / up.norm();
	Vector<float> ctar_normalized = ctar / ctar.norm();
	Vector<float> z = (cpos - ctar_normalized);
	z = z / z.norm();
	Vector<float> x = cross_product(z, up_normalized);
	x = x / x.norm();
	Vector<float> y = cross_product(x, z);
	y = y / y.norm();

	return (Matrix<float>(
			{
					{x.data[0], x.data[1], x.data[2], -x.dot(cpos)},
					{y.data[0], y.data[1], y.data[2], -y.dot(cpos)},
					{z.data[0], z.data[1], z.data[2], -z.dot(cpos)},
					{0, 0, 0, 1}
			}
			));
}

void WorldToView::setPos(const Vector<float> &new_pos) {this->cpos = new_pos;}

void WorldToView::setTarget(const Vector<float> &new_tar) {this->ctar = new_tar;}

void WorldToView::setUpVector(const Vector<float> &new_up) {this->up = new_up;}