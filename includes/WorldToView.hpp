#ifndef SCOP_WORLDTOVIEW_HPP
# define SCOP_WORLDTOVIEW_HPP
# include "Transformation.hpp"

class WorldToView : public Transformation {
private:
	Vector<float> cpos;
	Vector<float> ctar;
	Vector<float> up;
protected:
public:
	Matrix<float> compute() const override;
	WorldToView();
	void	setPos(const Vector<float> &);
	void	setTarget(const Vector<float> &);
	void	setUpVector(const Vector<float> &);
};

#endif //SCOP_WORLDTOVIEW_HPP
