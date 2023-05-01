#pragma once

#include <vector>
#include <functional>
#include <GLFW/glfw3.h>

class InputHandler
{
public:
	static void UpdateGamepad();
	static void ScaleAxis(float& xAxis, float& yAxis);

	static GLFWgamepadstate state;
	static bool connected;
private:
};


