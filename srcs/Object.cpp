#include "Object.hpp"
#include <algorithm>
#include "BasicTransformation.hpp"

Object::Object(const ObjectFactory &f) : factory(f), texture_switch(1.) {}

void Object::draw(const Transformation& t) {
	glBindVertexArray(this->factory.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->factory.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->factory.EBO);
	if (this->factory.tex)
		glBindTexture(GL_TEXTURE_2D, this->factory.tex);

	Vector<float> v{0.5, 0.5, 0.5, 1};

	auto tmp = ((t * this->position).compute().transpose() * v);

	glUniform1f(this->factory.projection.get_uniform_location("textureSwitch"), this->factory.tex ? this->texture_switch : -this->texture_switch);
	glUniformMatrix4fv(this->factory.projection.get_uniform_location("projection"), 1, true,
					   t
					   	 * this->position);
	this->factory.projection.use();
	glDrawElements(GL_TRIANGLES, this->factory.EBO_size, GL_UNSIGNED_INT, 0);

	if (this->factory.tex)
		glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Object &Object::translate(const Vector<float> &trans) {
	this->position.translate(trans);
	return *this;
}

Object& Object::rotate(float xrot, float yrot, float zrot) {
	this->position.rotate(xrot, yrot, zrot);
	return *this;
}

Object& Object::resize(float size_factor) {
	this->position.resize(size_factor);
	return *this;
}

Object& Object::reset() {
	this->position.reset();
	return *this;
}

Object& Object::setAngle(float xrot, float yrot, float zrot) {
	this->position.setAngle(xrot, yrot, zrot);
	return *this;
}

Object& Object::setPos(const Vector<float> &trans) {
	this->position.setPos(trans);
	return *this;
}

Object& Object::setSize(float size_factor) {
	this->position.setSize(size_factor);
	return *this;
}