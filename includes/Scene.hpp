#ifndef SCOP_SCENE_HPP
#define SCOP_SCENE_HPP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "ModelToWorld.hpp"
#include "Object.hpp"
#include "Perspective.hpp"
#include <map>
#include <boost/container/stable_vector.hpp>
#include "WorldToView.hpp"
#include "InputManager.hpp"

class Scene {
private:
	GLFWwindow*								window;
	WorldToView								view;
	Perspective								projection;
	boost::container::stable_vector<Object>	objs;
	InputManager							inputManager;
	int 									win_height;
	int 									win_width;
//	double									pos[2];
//	double									speed[2];
public:
	typedef boost::container::stable_vector<Object>::difference_type object_id;

	Scene();
	~Scene();
	void loop();
	void processInput();
	InputManager &getInputManager();

	size_t		get_win_width() const;
	size_t		get_win_height() const;
	GLFWwindow	*get_window() const;

	object_id	addObject(const Object &);
	Object	&getObject(object_id id);
	void	removeObject(object_id id);

	void	rotateView(double xpos, double ypos);
	void	translateView(double xpos, double ypos);
	void	close();

private:
	void createWindow(int width, int height, const std::string &name, GLFWmonitor *monitor = nullptr, GLFWwindow* share = nullptr);
	void closeWindow();
	static void resizeWindowCallback(GLFWwindow* window, int width, int height);
};


#endif //SCOP_SCENE_HPP
