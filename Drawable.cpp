#include "Drawable.h"

Drawable::Drawable(Shader& shader, Camera& camera) : shader(shader), camera(camera)
{

}
void Drawable::Draw()
{
	camera.SetCameraUniforms(shader);
}
