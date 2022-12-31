#include "../includes/Scene.hpp"
#include <iostream>
#include "Perspective.hpp"
#include "BasicTransformation.hpp"
#include "WorldToView.hpp"
#include <algorithm>

Scene::Scene() : projection(Perspective(2, 5)), inputManager(this->window, *this){
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

	this->view.setPos(Vector<float>{0, 0, -3});
	this->projection.setFov(M_PI / 3);
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
	std::cout << xpos << ", " << ypos << std::endl;
	while(!glfwWindowShouldClose(this->window))
	{
		this->processInput();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (Object &o : this->objs)
			o.draw(this->projection * this->view);

		glfwSwapBuffers(this->window);

		this->inputManager.poll();
	}
}

void Scene::processInput() {
	glfwGetWindowSize(this->window, &this->win_width, &this->win_height);
	this->projection.setAspectRatio((float)this->win_height / (float)this->win_width);

	static double new_pos[2];
	glfwGetCursorPos(window, &new_pos[0], &new_pos[1]);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if ((new_pos[0] - this->pos[0] != 0 || new_pos[1] - this->pos[1] != 0))
			this->rotateView(new_pos[0] - this->pos[0], new_pos[1] - this->pos[1]);
		speed[0] = new_pos[0] - this->pos[0];
		speed[1] = new_pos[1] - this->pos[1];
	}
	else
	{
		speed[0] /= 1.02;
		speed[1] /= 1.02;
		this->rotateView(speed[0], speed[1]);
	}
	if ((new_pos[0] - this->pos[0] != 0 || new_pos[1] - this->pos[1] != 0)
		&& glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		this->translateView(new_pos[0] - this->pos[0], new_pos[1] - this->pos[1]);
	this->pos[0] = new_pos[0];
	this->pos[1] = new_pos[1];
}

void Scene::rotateView(double xpos, double ypos) {
	xpos /= -(double)this->win_width / 2;
	ypos /= -(double)this->win_height / 2;
	this->objs[0].rotate(atan((float)ypos), atan((float)xpos), 0);
}

void Scene::translateView(double xpos, double ypos) {
	xpos /= (double)this->win_height / 2; // TODO: better
	ypos /= -(double)this->win_height / 2;
	this->objs[0].translate(Vector<float>({(float)xpos, (float)ypos, 0.}));
}

void Scene::createWindow(int width, int height, const std::string &name, GLFWmonitor *monitor, GLFWwindow *share) {
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