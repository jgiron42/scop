#include "ObjectFactory.hpp"

ObjectFactory::ObjectFactory(std::string const &vs, std::string const &fs) : projection(vs, fs) {
}

Object ObjectFactory::create() {
	return *this;
}

void ObjectFactory::addShader(const std::string &vs, const std::string &fs) {
	shaders.emplace(shaders.end(), vs, fs);
}