#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "window.hpp"
#include "MeshFactory.hpp"
#include "Object.hpp"
#include "MeshData.hpp"
#include "Scene.hpp"
#include "Motion.hpp"

using my_clock = std::chrono::system_clock;

void rotateObject(Scene &scene, double xpos, double ypos, Object &obj) {
	xpos /= -(double)scene.get_win_width() / 2;
	ypos /= -(double)scene.get_win_height() / 2;
	obj.
			rotate(atan((float)ypos),
				   atan((float)xpos), 0);
}

void translateObject(Scene &scene, double xpos, double ypos, Object &obj) {
	xpos /= (double)scene.get_win_width() / 2;
	ypos /= -(double)scene.get_win_height() / 2;
	obj.translate(Vector<float>({(float)xpos, (float)ypos, 0.}));
}


void process_mouse(Scene &scene, Object &obj)
{
	static bool click = false;
	static double speed[2] = {0,0};
	static double old_pos[2] = {NAN, NAN};
	double pos[2];
	if (!click)
	{
		if (glfwGetMouseButton(scene.get_window(), GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS && glfwGetMouseButton(scene.get_window(), GLFW_MOUSE_BUTTON_MIDDLE) != GLFW_PRESS)
		{
			obj.rotate(0,0.01, 0);
			return;
		}
		else
			click = true;
	}
	glfwGetCursorPos(scene.get_window(), &pos[0], &pos[1]);
	if (old_pos[0] != old_pos[0])
	{
		old_pos[0] = pos[0];
		old_pos[1] = pos[1];
	}

	if (glfwGetMouseButton(scene.get_window(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if ((pos[0] - old_pos[0] != 0 || pos[1] - old_pos[1] != 0))
			rotateObject(scene, pos[0] - old_pos[0], pos[1] - old_pos[1], obj);
		speed[0] = pos[0] - old_pos[0];
		speed[1] = pos[1] - old_pos[1];
	}
	else
	{
		speed[0] /= 1.02;
		speed[1] /= 1.02;
		rotateObject(scene, speed[0], speed[1], obj);
	}
	if ((pos[0] - old_pos[0] != 0 || pos[1] - old_pos[1] != 0)
		&& glfwGetMouseButton(scene.get_window(), GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		translateObject(scene, pos[0] - old_pos[0], pos[1] - old_pos[1], obj);
	old_pos[0] = pos[0];
	old_pos[1] = pos[1];
}

void	bindInputs(Scene &scene, Motion<float> &m,Motion<Vector<float>> &mv, Object &obj)
{
	scene.getInputManager().onClick(GLFW_KEY_ESCAPE, [&scene](int, InputManager::coord, InputManager::coord){scene.close();});
	scene.getInputManager().onClick(GLFW_KEY_SPACE, [&obj, &m](int, InputManager::coord, InputManager::coord){
		static float current = obj.texture_switch;
		current = 1 - current;
		m.add_transition(&obj.texture_switch, current, 1s);
	});
	static Vector<float> current{0, 0, 0};
	scene.getInputManager().onClick(GLFW_KEY_A, [&mv](int, InputManager::coord, InputManager::coord){
		auto next = current;
		next.data[0] = 1 - next.data[0];
		mv.add_transition(&current, next, 1s);
	});
	scene.getInputManager().onTick([&m, &mv](){m.update();mv.update();});
	scene.getInputManager().onTick([&scene, &obj](){ process_mouse(scene, obj); });
	scene.getInputManager().onTick([&obj](){obj.setSize(exp((float)scroll_pos / 5));});
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	try {
		auto obj_data = MeshData(std::string(argv[1]));

		Scene scene;
		Motion<float> m;
		Motion<Vector<float>> mv;

		MeshFactory meshFactory(obj_data);
		auto &obj = scene.getObject(scene.addObject(meshFactory.create()));
		bindInputs(scene, m, mv, obj);

		scene.loop();
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}