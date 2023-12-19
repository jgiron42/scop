#include "../includes/Scene.hpp"
#include <iostream>
#include "Perspective.hpp"
#include "BasicTransformation.hpp"
#include "WorldToView.hpp"
#include <algorithm>

Scene::Scene() : window(NULL), projection(Perspective(1, 4)), inputManager(this->window, *this) {
	glfwInit();
	this->createWindow(800, 800, "scop");

	glfwMakeContextCurrent(this->window);

	glViewport(0, 0, 800, 800);
	glfwSetFramebufferSizeCallback(this->window, &Scene::resizeWindowCallback);
	glfwSetScrollCallback(window, InputManager::scrollCallback);

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		auto &&e = std::runtime_error("Error: '" + std::string((char *)glewGetErrorString(res)) + "'");
		glfwTerminate();
		throw e;
	}

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	this->view.setPos(Vector<float>{0, 0, -2.5});
	this->projection.setFov(87. * M_PI / 180 );
}

Scene::~Scene() {
	this->closeWindow();
	glfwTerminate();
}

Scene::object_id Scene::addObject(const Object &o) {
	this->objs.push_back(o);
	return (object_id)this->objs.size();
}

Object	&Scene::getObject(Scene::object_id id)
{
	return this->objs[id - 1];
}

void	Scene::removeObject(const object_id id)
{
	this->objs.erase(this->objs.begin() + id - 1);
}

void Scene::loop() {
	static double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	while(!glfwWindowShouldClose(this->window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (Object &o : this->objs)
			o.draw(this->projection * this->view);

		glfwSwapBuffers(this->window);

		this->inputManager.poll();
		this->processInput();
	}
}

size_t Scene::get_win_height() const {
	return this->win_height;
}

size_t Scene::get_win_width() const {
	return this->win_width;
}

GLFWwindow *Scene::get_window() const {
	return this->window;
}

void Scene::processInput() {
	glfwGetWindowSize(this->window, &this->win_width, &this->win_height);
	this->projection.setAspectRatio((float)this->win_height / (float)this->win_width);
}

void Scene::createWindow(int width, int height, const std::string &name, GLFWmonitor *monitor, GLFWwindow *share) {
	assert(!this->window);
	std::cout << "bonjour" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	this->window = glfwCreateWindow(width, height, name.c_str(), monitor, share);
	if (!this->window)
		throw std::runtime_error("Failed to create GLFW window");
}

void Scene::closeWindow() {
	if (this->window)
	{
		glfwDestroyWindow(this->window);
		this->window = nullptr;
	}
}

void Scene::resizeWindowCallback(GLFWwindow *, int width, int height) {
	glViewport(0, 0, width, height);
}

InputManager &Scene::getInputManager() {return this->inputManager;}

void Scene::close() {
	glfwSetWindowShouldClose(window, true);
}