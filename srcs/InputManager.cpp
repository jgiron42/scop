#include "../includes/InputManager.hpp"
#include <iostream>

InputManager::InputManager(GLFWwindow *&w, Scene &s) : window(w), scene(s) {}

void InputManager::onClick(int key, clickCallback && cb) {
	this->click_listeners[key].callbacks.push_back(cb);
}

void InputManager::onTick(std::function<void()> &&cb) {
	this->tick_listeners.push_back(cb);
}

void InputManager::poll() {
	glfwPollEvents();
	this->watchClicks();
	this->watchTicks();
}

void InputManager::watchTicks() {
	for (auto &e : this->tick_listeners)
		e();
}

void InputManager::watchClicks() {
	coord now = {};
	glfwGetCursorPos(window, &now.x, &now.y);
	now.t = glfwGetTime();

	for (auto &p : this->click_listeners)
	{
		auto &l = p.second;
		int	current_state = glfwGetKey(window, p.first);
		if ((current_state == GLFW_PRESS && l.is_pressed) ||
			(current_state == GLFW_RELEASE && !l.is_pressed))
			continue; // nothing has changed
		if (current_state == GLFW_PRESS)
		{
			l.is_pressed = true;
			l.begin = now;
		}
		else
		{
			l.is_pressed = false;
			for (const auto& cb : l.callbacks)
				cb(p.first, l.begin, now);
		}
	}
}

InputManager::coord operator-(const InputManager::coord &l, const InputManager::coord &r)
{
	return ((InputManager::coord){
				l.x - r.x,
				l.y - r.y,
				l.t - r.t
			});
}

double scroll_pos = 0;

void InputManager::scrollCallback(GLFWwindow *, double , double yoffset) {
	scroll_pos += yoffset;
}