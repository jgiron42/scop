//
// Created by joachim on 8/24/22.
//

#ifndef MATRIX_VECTOR_HPP
#define MATRIX_VECTOR_HPP
#include <vector>
#include <iostream>
#include "Matrix.hpp"
#include "math.h"
#include "complex"
using namespace std;

class incompatible_exception;

template <typename T>
class Matrix;

template <typename T = float>
class	Vector {
public:
	typedef int		size_type;
	std::vector<T>	data;

	explicit Vector(initializer_list<T> init) : data(init) {}
	explicit Vector(vector<T> init) : data(init) {}
	explicit Vector(size_type n) : data(n, 0) {}

	void	assign(initializer_list<T> init)
	{
		data.assign(init);
	}
	void	resize(int size)
	{
		this->data.resize(size);
	}

	[[nodiscard]] size_type	size() const {return this->data.size();}
	void	print(bool newline = true) const
	{
		cout << "{";
		bool first = true;
		for (auto item : data)
		{
			if (first)
				first = false;
			else
				cout << ", ";
			cout << item;
		}
		cout << "}";
		if (newline)
			cout << endl;
	}

	template <typename U>
	Vector 	&scl(U scalar)
	{
		return this->map([&scalar](T val, int i){(void)i;return scalar * val;});
	}
	Vector 	&add(Vector r)
	{
		return this->map([&r](T val, int i){return val + r.data[i];});
	}
	Vector 	&sub(Vector r)
	{
		return this->map([&r](T val, int i){return val - r.data[i];});
	}
	Vector	&cross_product(Vector other)
	{
		if (other.size() != 3 || this->size() != 3) throw incompatible_exception();
		this->assign({this->data[1] * other.data[2] - this->data[2] * other.data[1],
					  this->data[2] * other.data[0] - this->data[0] * other.data[2],
					  this->data[0] * other.data[1] - this->data[1] * other.data[0],});
		return *this;
	}

	T		dot(Vector r)
	{
		return (Matrix<T>(1, this->data.size(), {this->data}) * Matrix<T>(r.data.size(), 1, r.data)).data[0][0];
	}

	float		norm(double n) {
		if (n == INFINITY)
		{
			float max = 0;
			for (T val : this->data)
				if (std::norm(val) > max)
					max = pow(std::norm(val), 0.5);
			return max;
		}
		else
		{
			float ret = 0;
			for (T val : this->data) {
				ret += pow(std::norm(val), n/2);
			}
			return (float)pow(ret, 1/n);
		}
	}

	float	norm() {return this->norm(2);}
	float	norm_1() {return this->norm(1);}
	float	norm_inf() {return this->norm(INFINITY);}

	Vector &operator+=(Vector const &other) {return *this = *this + other;}
	Vector &operator-=(Vector const &other) {return *this = *this - other;}
	[[nodiscard]] T		norm() const
	{
		T tmp;
		for (T val : this->data)
			tmp += val * val;
		return sqrt(tmp);
	}
	template<class C>
	Vector	&map(C f)
	{
		for (size_t i = 0; i < this->data.size(); i++)
			this->data[i] = f(this->data[i], i);
		return *this;
	}
};

namespace std
{
	template<typename T>
	float abs(Vector<T> v)
	{
		return v.norm();
	}
}

template<typename T = float>
bool operator==(Vector<T> const &l, Vector<T> const &r)
{
	return l.data == r.data;
}

template<typename T = float>
bool operator!=(Vector<T> const &l, Vector<T> const &r)
{
	return l.data != r.data;
}

template<typename T = float>
Vector<T> operator+(Vector<T> const &l, Vector<T> const &r)
{
	return Vector<>(l).add(r);
}

template<typename T = float>
Vector<T> operator-(Vector<T> const &l, Vector<T> const &r)
{
	return Vector<>(l).sub(r);
}

template<typename T = float>
Vector<T> operator*(Vector<T> const &l, T const &r)
{
	return Vector<>(l).scl(r);
}

template<typename T = float>
Vector<T> operator/(Vector<T> const &l, T const &r)
{
	return Vector<>(l).scl(1/r);
}

template <typename T = float>
ostream &operator<<(ostream &out, const Vector<T> &v)
{
	v.print(false);
	return out;
}

template <typename T = float>
float	angle_cos(Vector<T> const &l, Vector<T> const &r)
{
	return Vector<T>(l / l.norm()).dot(r / r.norm());
}

template <typename T = float>
Vector<T>	cross_product(const Vector<T> &l, const Vector<T> &r)
{
	return Vector<T>(l).cross_product(r);
}


#endif //MATRIX_VECTOR_HPP
