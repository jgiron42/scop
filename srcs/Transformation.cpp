
#include "Transformation.hpp"
#include "BasicTransformation.hpp"

Transformation::Transformation() : buffer() {}

// identity transformation
Matrix<float> Transformation::compute() const {
	return identity_mat(4);
}

const GLfloat *Transformation::getBuf() {
	Matrix<float> mat = this->compute();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->buffer[i * 4 + j] = mat.data[i][j];
	return this->buffer.data();
}

Transformation::operator const float *() {
	return this->getBuf();
}

BasicTransformation operator*(const Transformation &l, const Transformation &r)
{
	return BasicTransformation{l.compute() * r.compute()};
}
