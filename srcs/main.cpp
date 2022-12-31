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
	scene.getInputManager().onTick([&obj](){std::cout<<current<<std::endl;obj.setPos(current);});
	scene.getInputManager().onTick([&m, &mv](){m.update();mv.update();});
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