#include "InputHandler.h"

GLFWgamepadstate InputHandler::state;
bool InputHandler::connected;

void InputHandler::UpdateGamepad()
{
	connected = glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
}

void InputHandler::ScaleAxis(float& xAxis, float& yAxis)
{
	const float joystickThreshold = 0.3f;
	float squaredMagnitude = xAxis * xAxis + yAxis * yAxis;
	if (squaredMagnitude > joystickThreshold * joystickThreshold)
	{
		double angle = atan2(xAxis, yAxis);
		double magnitude = sqrt(squaredMagnitude);

		xAxis = (magnitude - joystickThreshold) * sin(angle);
		yAxis = (magnitude - joystickThreshold) * cos(angle);
	}
	else
	{
		xAxis = 0.0f;
		yAxis = 0.0f;
	}
}
