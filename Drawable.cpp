#include "Drawable.h"

void Drawable::Draw()
{
	camera.SetCameraUniforms(shader);
}
