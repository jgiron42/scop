#include <utility>

#include "../includes/BasicTransformation.hpp"

BasicTransformation::BasicTransformation(Matrix<float> val) : value(std::move(val)) {}

Matrix<float> BasicTransformation::compute() const {return this->value;}