//
// Created by joachim on 12/17/22.
//

#ifndef SCOP_BASICTRANSFORMATION_HPP
#define SCOP_BASICTRANSFORMATION_HPP
#include "Transformation.hpp"

class BasicTransformation : public Transformation {
private:
	const Matrix<float>	value;
protected:
public:
	Matrix<float> compute() const override;
	explicit BasicTransformation(Matrix<float> val);
};


#endif //SCOP_BASICTRANSFORMATION_HPP
