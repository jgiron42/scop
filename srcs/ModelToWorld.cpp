//
// Created by joachim on 12/14/22.
//

#include "ModelToWorld.hpp"

ModelToWorld::ModelToWorld() : rot(4, 4), trans(4, 4), scale(4, 4) {
	this->reset();
}

ModelToWorld & ModelToWorld::rotate(float xrot, float yrot, float zrot) {
	this->rot =
			Matrix<float> (
					{
							{1 ,0			, 0				, 0},
							{0 ,cosf(xrot)	, -sinf(xrot)	, 0},
							{0 ,sinf(xrot)	, cosf(xrot)	, 0},
							{0 ,0			, 0				, 1}
					}) *
			Matrix<float> (
					{
							{cosf(yrot)		, 0	, sinf(yrot)	, 0},
							{0				, 1	, 0				, 0},
							{-sinf(yrot)	, 0	, cosf(yrot)	, 0},
							{0				, 0	, 0				, 1}
					}) *
			Matrix<float> (
					{
							{cosf(zrot)		, -sinf(zrot)	, 0	, 0},
							{sinf(zrot)		, cosf(zrot)	, 0	, 0},
							{0				, 0				, 1	, 0},
							{0				, 0				, 0	, 1}
					}) * rot;
	return *this;
}

ModelToWorld &ModelToWorld::translate(const Vector<float> &trans) {
	this->trans.data[0][3] += trans.data[0];
	this->trans.data[1][3] += trans.data[1];
	this->trans.data[2][3] += trans.data[2];
	return *this;
}

ModelToWorld &ModelToWorld::resize(float size_factor) {
	this->scale *= Matrix<float>(
			{
				{size_factor, 0, 0, 0},
				{0, size_factor, 0, 0},
				{0, 0, size_factor, 0},
				{0, 0, 0, 1}
			}
			);
	this->scale *= size_factor;
	return *this;
}

ModelToWorld &ModelToWorld::reset() {
	this->rot = identity_mat(4);
	this->trans = identity_mat(4);
	this->scale = identity_mat(4);
	return *this;
}

ModelToWorld &ModelToWorld::setAngle(float xrot, float yrot, float zrot) {
	this->rot = identity_mat(4);
	return this->rotate(xrot, yrot, zrot);
}

ModelToWorld &ModelToWorld::setPos(const Vector<float> &newTrans) {
	this->trans.data[0][3] = newTrans.data[0];
	this->trans.data[1][3] = newTrans.data[1];
	this->trans.data[2][3] = newTrans.data[2];
	return *this;
}

ModelToWorld &ModelToWorld::setSize(float size_factor) {

	this->scale = Matrix<float>(
			{
					{size_factor, 0, 0, 0},
					{0, size_factor, 0, 0},
					{0, 0, size_factor, 0},
					{0, 0, 0, 1}
			}
	);
	return *this;
}

ModelToWorld operator*(const ModelToWorld &l, const ModelToWorld &r)
{
	ModelToWorld ret;
	ret.rot = l.rot * r.rot;
	ret.trans = l.trans * r.trans;
	ret.scale = l.scale * r.scale;
	return ret;
}

Matrix<float> ModelToWorld::compute() const {
	return this->trans * this->rot * this->scale;
}