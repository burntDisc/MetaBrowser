#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(int windowWidth, int windowHeight) :
	windowWidth(windowWidth),
	windowHeight(windowHeight)
{
}

void Camera::Bind(glm::vec3* newTranslation, glm::vec3* newOrientation, float* newFeildOfView)
{
	translation = newTranslation;
	orientation = newOrientation;
	feildOfView = newFeildOfView;
}

void Camera::SetCameraUniforms(Shader& shader)
{
	float nearPlane = 0.01f;
	float farPlane = 60000.0f;
	// Makes camera look in the right direction from the right position

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(*translation, *translation + *orientation, up);

	// Adds perspective to the scene
	float aspectRatio = (float)windowWidth / windowHeight;

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(*feildOfView), aspectRatio, nearPlane, farPlane);

	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

//TODO: move window width and camera uniforms out of player
void Camera::SetSkyboxUniforms(Shader& skyboxShader)//, int windowWidth, int windowHeight, glm::vec3& translation, glm::vec3& orientation, float& feildOfView)
{

	skyboxShader.Activate();
	glm::mat4 skyboxView = glm::mat4(1.0f);
	glm::mat4 skyboxProjection = glm::mat4(1.0f);

	// mat4 -> mat3 -> mat4 removes last row and column for translation
	skyboxView = glm::mat4(glm::mat3(glm::lookAt(*translation, *translation + (*orientation), up)));
	skyboxProjection = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(skyboxProjection));
}
