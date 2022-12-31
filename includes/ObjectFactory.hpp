#ifndef SCOP_OBJECTFACTORY_HPP
# define SCOP_OBJECTFACTORY_HPP
# include "Object.hpp"
# include <GL/glew.h>
# include <GLFW/glfw3.h>
# include <string>
# include <vector>
# include "shader.hpp"

class Object;

class ObjectFactory {
private:
	std::vector<shader> shaders;
	shader projection;
protected:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int tex;
	GLsizei EBO_size;
public:
	ObjectFactory(std::string const &vs, std::string const &fs);
	virtual void build() = 0;
	void	addShader(std::string const &vs, std::string const &fs);
	Object create();

	friend Object;
};

#endif //SCOP_OBJECTFACTORY_HPP
