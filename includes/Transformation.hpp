//
// Created by joachim on 12/17/22.
//

#ifndef SCOP_TRANSFORMATION_HPP
#define SCOP_TRANSFORMATION_HPP

# include "Matrix.hpp"
# include <array>

class BasicTransformation;
class Transformation {
protected:
	std::array<float, 16>	buffer;
public:
	virtual Matrix<float> 	compute() const;
	friend BasicTransformation operator*(const Transformation &l, const Transformation &r);
	Transformation();
	const float *getBuf(void);
	operator const float*();
};


#endif //SCOP_TRANSFORMATION_HPP
