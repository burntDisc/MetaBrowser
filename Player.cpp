#include "Player.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/projection.hpp>

#include "MotionHandler.h"
#include "InputHandler.h"

Player::Player(glm::vec3 startPosition) :
	translation(startPosition)
{

}

void Player::Jump()
{
	if ((surfaceNormal != glm::vec3(0.0f, 0.0f, 0.0f) && currentTime > prevJumpTime + jumpInterval)
		|| velocity == glm::vec3(0.0f, 0.0f, 0.0f))
	{
		prevJumpTime = currentTime;
		velocity = jumpVelocity * up + velocity;
		stepPosition.y += paceLength;
	}

}

void Player::Break()
{
	breaking = true;
	velocity -= velocity;
}

void Player::DirectionalDash(glm::vec3 direction, bool& ready)
{
	if (ready)
	{
		ready = false;
		velocity = glm::dot(direction, velocity) > 0.0f ?
			glm::proj(velocity, direction) + dashInitVelocity * direction :
			dashInitVelocity * direction;
	}
	else
	{
		velocity += dashAcceleration * direction * (float)timeDelta;
	}
}

void Player::ReadyDashForward()
{
	DashForwardReady = true;
}
void Player::DashForward()
{
	glm::vec3 direction = orientation;
	DirectionalDash(direction, DashForwardReady);
}
void Player::ReadyDashBack()
{
	DashBackReady = true;
}
void Player::DashBack()
{
	glm::vec3 direction = -orientation;
	DirectionalDash(direction, DashBackReady);
}
void Player::ReadyDashLeft()
{
	DashLeftReady = true;
}
void Player::DashLeft()
{
	glm::vec3 direction = -glm::normalize(glm::cross(orientation, up));
	DirectionalDash(direction, DashLeftReady);
}
void Player::ReadyDashRight()
{
	DashRightReady = true;
}
void Player::DashRight()
{
	glm::vec3 direction = glm::normalize(glm::cross(orientation, up));
	DirectionalDash(direction, DashRightReady);
}


void Player::Update(double time)
{
	Updatable::Update(time);

	// Process Inputs---------------------------------------------------------------------------------------------------------------
	AdjustVelocity(InputHandler::state.axes[GLFW_GAMEPAD_AXIS_LEFT_X], InputHandler::state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
	AdjustOrientation(InputHandler::state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], InputHandler::state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
	if (InputHandler::state.buttons[GLFW_GAMEPAD_BUTTON_A]) Jump();
	if (InputHandler::state.buttons[GLFW_GAMEPAD_BUTTON_B]) Break(); else breaking = false;
	if (InputHandler::state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB])
	{
		if (!lockPressed)
		{
			lockPressed = true;
			locked = !locked;
		}
	}
	else
	{
		lockPressed = false;
	}



	// adjust velocity based on collision ------------------------------------------------------------------------
	bool colliding = surfaceNormal != glm::vec3(0.0, 0.0, 0.0);

	if (colliding)
	{
		lastCollision = translation;
		if (!flatNav)
		{
			if (length(translation - stepPosition) > paceLength)
			{
				stepPosition = translation;
			}
			flatNav = true;
		}

		// newVelocity for bounce
		float dot = glm::dot(velocity, surfaceNormal);
		if (dot < 0.0f)
		{
			// remove towards-surface component
			glm::vec3 proj = glm::proj(velocity, surfaceNormal);
			velocity = velocity - repulsionFac * proj;
		}
	}
	else
	{
		// apply gravity on Second update (captured by flatNav)
		if (!flatNav)
		{

			if (glm::length(translation - lastCollision) > preGravTravel)
			{
				velocity.y -= gravity * (float)timeDelta;
			}
		}
		else
		{
			flatNav = false;
		}
	}

	//apply friction-----------------------------------------------------------------
	if (velocity != glm::vec3(0.0f, 0.0f, 0.0f))
		velocity -= normalize(velocity) * friction * (float)timeDelta;

	// Translate player----------------------------------------------------------------------------------
	if (length(velocity) > minVelocity)
	{
		translation = MotionHandler::CollideAndSlide(translation, velocity, timeDelta, surfaceNormal);
		// correct for positive only bounds (TODO elegant)
		if (translation.x < 0.0f)
		{
			translation.x = 0.0f;
			if(velocity.x < 0.0f)
				velocity.x = 0.0f;
		}
		if (translation.y < 0.0f)
		{
			translation.y = 0.0f;
			if (velocity.y < 0.0f)
				velocity.y = 0.0f;
		}
		if (translation.z < 0.0f)
		{
			translation.z = 0.0f;
			if (velocity.z < 0.0f)
				velocity.z = 0.0f;
		}
	}
	else
	{
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	// stop on ground
	if (glm::length(velocity) < minGroundVelocity && surfaceNormal != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	}
}

void Player::Delete()
{
	delete this;
}


void Player::AdjustVelocity(float xAxis, float yAxis)
{
	InputHandler::ScaleAxis(xAxis, yAxis);

	if (abs(xAxis) > 0.0f || abs(yAxis) > 0.0f)
	{
		// Allows for flat movement as opposed to flying
		bool alt = false;

		glm::vec3 relativeUp = surfaceNormal == glm::vec3(0.0, 0.0, 0.0) ? up : surfaceNormal;
		glm::vec3 normalizedSide = alt ?
			glm::normalize(glm::cross(orientation, relativeUp)) :
			glm::normalize(glm::cross(orientation, up));
		glm::vec3 normalizedFront = glm::normalize(glm::cross(relativeUp, normalizedSide));
		glm::vec3 stickSideComponent = xAxis * normalizedSide;
		glm::vec3 stickFrontComponent = alt ?
			-yAxis * normalizedFront :
			-yAxis * orientation;
		glm::vec3 stickDirection = normalize(stickSideComponent + stickFrontComponent);

		// apply change from input
		glm::vec3 newVelocity =
			flatNav ?
			collisionAcceleration * (float)timeDelta * stickDirection + velocity :
			airAcceleration * (float)timeDelta * stickDirection + velocity;

		//removing y component to ignore gravity
		glm::vec3 newHorizontalVelocity = glm::vec3(newVelocity.x, 0.0, newVelocity.z);
		glm::vec3 horizontalVelocity = glm::vec3(velocity.x, 0.0, velocity.z);

		// change direction but do not increase speed if at max (excluding vertical)
		glm::vec3 calibratedHorizontalVelocity = glm::length(newHorizontalVelocity) > maxSpeed ?
			normalize(newHorizontalVelocity) * length(horizontalVelocity) :
			newHorizontalVelocity;

		velocity = glm::vec3(
			calibratedHorizontalVelocity.x,
			newVelocity.y,
			calibratedHorizontalVelocity.z);

	}
}

void Player::AdjustOrientation(float xAxis, float yAxis)
{
	InputHandler::ScaleAxis(xAxis, yAxis);

	float rotX = lookSensitivity * xAxis * (float)timeDelta;
	float rotY = lookSensitivity * yAxis * (float)timeDelta;

	// Calculates upcoming vertical change in the orientation
	glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotY), glm::normalize(glm::cross(orientation, up)));

	// Decides whether or not the next vertical orientation is legal or not
	if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	{
		orientation = newOrientation;
	}

	// Rotates the orientation left and right
	orientation = glm::rotate(orientation, glm::radians(-rotX), up);
}
