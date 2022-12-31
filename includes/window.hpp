#ifndef SCOP_WINDOW_HPP
#define SCOP_WINDOW_HPP
#include <GLFW/glfw3.h>
#include <string>

class	window {
private:
	GLFWwindow* ptr;
public:
	window(int width, int height, const std::string &name, GLFWmonitor *monitor = nullptr, GLFWwindow* share = nullptr)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		this->ptr = glfwCreateWindow(width, height, name.c_str(), monitor, share);
		if (!this->ptr)
		{
			std::cerr << "Failed to create GLFW window" << std::endl;
			throw std::exception();
		}
	}
	GLFWwindow *getptr() const {
		return this->ptr;
	}
	~window() {
		glfwDestroyWindow(this->ptr);
	}
};

#endif //SCOP_WINDOW_HPP
