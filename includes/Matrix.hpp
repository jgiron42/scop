#ifndef MATRIX_MATRIX_HPP
#define MATRIX_MATRIX_HPP
#include <vector>
#include <array>
#include <iostream>
#include <complex>
#include "Vector.hpp"
#include "common.hpp"

using namespace std;

template <typename T>
class Vector;

template<typename T = float>
class	Matrix {
public:
	typedef	pair<int, int>	size_type;
	int	m; // lines
	int	n; // columns
	std::vector<std::vector<T>> data;

	Matrix(int m, int n) {this->resize(m, n);}
	Matrix(size_type s) {this->resize(s);}
	Matrix(int new_m, int new_n, const vector<T> &init) {this->assign(new_m, new_n, init);}
	Matrix(const vector<vector<T>> init) {this->assign(init);}
	Matrix(Matrix &&other) = default;
	Matrix(const Matrix &other) = default;
	Matrix &operator=(const Matrix &other) = default;

	void	assign(int new_m, int new_n, const vector<T> &init)
	{
		auto it = init.begin();
		this->resize(new_m, new_n);
		for (auto &i : data)
			for (auto &j : i)
				if (it != init.end())
				{
					j = *it;
					it++;
				}
				else
					j = T();
	}
	void	assign(const vector<vector<T>> init)
	{
		auto it = init.begin();
		this->data.resize(init.size());
		for (auto &v : this->data)
		{
			v = *it;
			it++;
		}
		this->resize(init.size(), init.begin()->size());
	}
	void	assign(const vector<Vector<T>> init)
	{
		auto it = init.begin();
		this->data.resize(init.size());
		for (auto &v : this->data)
		{
			v = it->data;
			it++;
		}
		this->resize(init.size(), init.begin()->size());
	}

	void	resize(int new_m, int new_n)
	{
		this->m = new_m;
		this->n = new_n;
		this->data.resize(new_m);
		for (auto &row : this->data)
			row.resize(new_n);
	}
	void	resize(size_type size)
	{
		this->resize(size.first, size.second);
	}

	[[nodiscard]] bool				is_square() const {return m == n;}
	[[nodiscard]] size_type	size() const {return make_pair(this->m, this->n);}
	void	print(bool newline = true) const
	{
		bool f1 = true;
		cout << "{";
		for (auto line : this->data)
		{
			if (f1)
				f1 = false;
			else
			{
				cout << ",";
				if (newline)
					cout << endl;
				cout << " ";
			}
			cout << "{";
			bool f2 = true;
			for (auto item : line)
			{
				if (f2)
					f2 = false;
				else
					cout << ", ";
				cout << item;
			}
			cout << "}";
		}
		cout << "}";
		if (newline)
			cout << endl;
	}

	Matrix	&scl(T scalar)
	{
		return this->map([&scalar](T val, int i, int j){(void)i; (void)j;return scalar * val;});
	}
	Matrix	&add(Matrix r)
	{
		return this->map([&r](T val, int i, int j){(void)i; (void)j;return val + r.data[i][j];});
	}
	Matrix	&sub(Matrix r)
	{
		return this->map([&r](T val, int i, int j){(void)i; (void)j;return val - r.data[i][j];});
	}
	[[nodiscard]] T		trace()
	{
		T ret = T();
		if (!this->is_square()) throw incompatible_exception();
		for (int i = 0; i < this->m; i++)
			ret += this->data[i][i];
		return ret;
	}
	[[nodiscard]] Matrix	transpose() const
	{
		Matrix ret(this->n, this->m);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				ret.data[i][j] = this->data[j][i];
		return ret;
	}

	// https://fr.wikipedia.org/wiki/%C3%89limination_de_Gauss-Jordan
	[[nodiscard]] Matrix	row_echelon()
	{
		Matrix ret(*this);
		int r = -1;
		for (int j = 0; j < n; j++)
		{
			int k = r + 1;
			for (int i = r + 1; i < m; i++)
			{
				if (std::norm(ret.data[i][j]) >
				std::norm(ret.data[k][j]))
					k = i;
			}
			if (k < m && k >= 0 && ret.data[k][j] != 0)
			{
				r++;
				T tmp = ret.data[k][j];
				for (int i = 0; i < n; i++)
					ret.data[k][i] /= tmp;
				if (k != r)
						swap(ret.data[k], ret.data[r]);
				for (int i = 0; i < m; i++) {
					tmp = ret.data[i][j];
					if (i != r)
						for (int l = 0; l < n; l++)
							ret.data[i][l] -= ret.data[r][l] * tmp;
				}
			}
		}
		for (auto &i : ret.data)
			for (auto &j : i)
				if (j == -0.)
					j = 0.;
		return ret;
	}

	[[nodiscard]] T determinant(void) const
	{
		if (!this->is_square() || this->data.size() < 1) throw incompatible_exception();
		vector<vector<T>> copy(this->data);
		return det(copy);
	}

	[[nodiscard]] Matrix invert(void) const
	{
		if (!this->determinant())
			throw runtime_error("matrix is not invertible");

		Matrix tmp = *this;
		// append the identity matrix at the right of the matrix
		for (size_t i = 0; i < this->data.size(); i++)
		{
			tmp.data[i].resize(this->data.size() * 2);
			tmp.data[i][this->data.size() + i] = 1;
		}
		tmp.n *= 2;
		tmp = tmp.row_echelon(); // magic!

		// get rid of the left matrix
		for (auto &line : tmp.data)
			line.erase(line.begin(), line.begin() + (line.size() / 2));
		tmp.n /= 2;
		return tmp;
	}

	std::vector<T> get_raw_data()
	{
		std::vector<T> ret;

		for (int i = 0; i < this->m; i++)
			for (int j = 0; j < this->n; j++)
				ret.push_back(this->data[i][j]);
		return ret;
	}

	size_t	rank()
	{
		Matrix<T> tmp = this->row_echelon();
		return find(tmp.data.begin(), tmp.data.end(), vector<T>(this->n)) - tmp.data.begin();
	}

	Matrix &operator+=(Matrix const &other) {return *this = *this + other;}
	Matrix &operator-=(Matrix const &other) {return *this = *this - other;}
	template <class C>
	Matrix &operator*=(C const &other) {return *this = *this * other;}

	template<class C>
	Matrix	&map(C f)
	{
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				this->data[i][j] = f(this->data[i][j], i, j);
		return *this;
	}
private:

	static T	det(vector<vector<T>> &array)
	{
		// trivial case (2x2):
		if (array.size() == 2)
			return (array[0][0] * array[1][1] - array[1][0] * array[0][1]);

		// we're using Laplace method. Pas mal non? c'est francais.

		// row echelon (doing this lower the complexity from O(n!) to O(n^3)):
		size_t refline = 0; // the non-zero line
		for (; refline < array.size() && !array[refline][0]; refline++);
		if (refline >= array.size()) // all lines begin with 0 so the det is 0
			return 0;

		// transform the matrix in order to keep one non-zero value in the first column
		for (size_t i = 0; i < array.size(); i++) {
			if (i != refline && array[i][0]) {
				double f = array[i][0] / array[refline][0];
				for (size_t j = 0; j < array[i].size(); j++)
					array[i][j] -= f * array[refline][j];
			}
		}

		// actual determinant:
		T ret = array[refline][0];
		array.erase(array.begin() + refline);
		for (auto &line : array)
			line.erase(line.begin());
		ret *= (refline % 2 ? -1 : 1) * det(array);
		return ret;
	}
	void	reserve()
	{
		data.reserve(m);
		for (auto &i : this->data)
			i.reserve(n);
	}
};

template<typename T = float>
bool operator==(Matrix<T> const &l, Matrix<T> const &r)
{
	return l.data == r.data;
}

template<typename T = float>
bool operator!=(Matrix<T> const &l, Matrix<T> const &r)
{
	return l.data != r.data;
}

template<typename T = float>
Matrix<T> operator+(Matrix<T> const &l, Matrix<T> const &r)
{
	if (l.size() != r.size()) throw incompatible_exception();
	return Matrix<>(l).add(r);
}

template<typename T = float>
Matrix<T> operator-(Matrix<T> const &l, Matrix<T> const &r)
{
	if (l.size() != r.size()) throw incompatible_exception();
	return Matrix<>(l).sub(r);
}

template<typename T = float>
Matrix<T> operator*(Matrix<T> const &l, Matrix<T> const &r)
{
	if (l.n != r.m) throw incompatible_exception();
	Matrix<T> ret(l.m, r.n);
	for (int i = 0; i < l.m; i++)
		for (int j = 0; j < r.n; j++)
			for (int k = 0; k < l.n; k++)
				ret.data[i][j] += l.data[i][k] * r.data[k][j];
	return ret;
}

template<typename T = float>
Vector<T> operator*(Matrix<T> const &l, Vector<T> const &vr)
{
	if (l.n != vr.size()) throw incompatible_exception();
	Vector<T> ret(l.m);
	for (int i = 0; i < l.m; i++)
		for (int k = 0; k < l.n; k++)
			ret.data[i] += l.data[i][k] * vr.data[k];
	return ret;
}


template<typename T = float>
Matrix<T> operator*(Matrix<T> const &l, T const &r)
{
	return Matrix<T>(l).scl(r);
}

template<typename T = float>
ostream &operator<<(ostream &out, const Matrix<T> &mat)
{
	mat.print(false);
	return out;
}

template<typename T = float>
Matrix<T> identity_mat(unsigned int size)
{
	vector<vector<T>> data(size, vector<T>(size, T()));
	while (size-- > 0)
		data[size][size] = 1;
	return Matrix<T>(data);
}

#endif //MATRIX_MATRIX_HPP
