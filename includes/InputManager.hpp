#ifndef SCOP_INPUTMANAGER_HPP
#define SCOP_INPUTMANAGER_HPP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include "Scene.hpp"
#include <map>
#include <list>
#include <vector>
#include <functional>

class Scene;
class InputManager {
public:
	struct coord
	{
		double x;
		double y;
		double t;
		friend coord operator-(const coord &l, const coord &r);
	};
	typedef std::list<std::function<void()> >::iterator tick_event_id;
private:
//	typedef void (*clickCallback)(int, coord a, coord b);
	typedef std::function<void(int, coord, coord)> clickCallback;
	GLFWwindow		*&window;
	Scene			&scene;
	struct clickListener {
		std::vector<clickCallback> callbacks;
		bool	is_pressed;
		coord	begin;
	};
	std::map<int, clickListener>		click_listeners;
	std::list<std::function<void()> >	tick_listeners;
public:
	InputManager(GLFWwindow *&w, Scene &s);
	void	onClick(int key, clickCallback && cb);
	tick_event_id	onTick(std::function<void()> &&);
	void	cancelTickEvent(tick_event_id id);
	void	watchClicks();
	void	watchTicks();
	void	poll();
	static void	scrollCallback(GLFWwindow *, double , double yoffset);
	// press
	// click (duration, position begin end)
	//
};

extern double scroll_pos;

#endif
