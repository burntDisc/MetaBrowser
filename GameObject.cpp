#include "GameObject.h"

GameObject::GameObject(Shader shader, const char* modelFile, glm::vec3 initTranslation, glm::vec3 initScale, glm::quat initRotation, glm::vec3 modelOffset) :
	Drawable(shader),
	validWorldPositions(false),
	model(modelFile, modelOffset),
	translation(initTranslation),
	rotation(initRotation),
	scale(initScale)
{
}

void GameObject::Draw()
{
	glDepthFunc(GL_LESS);
	model.Draw(shader, translation, rotation, scale);
}

std::vector<glm::vec3> GameObject::GetVertexWorldNormals()
{
	if (validWorldPositions)
	{
		return vertexWorldNormals;
	}
	else
	{
		UpdateWorldVertices();
		return vertexWorldNormals;
	}
}

std::vector<glm::vec3> GameObject::GetVertexWorldPositions()
{

	if (validWorldPositions)
	{
		return vertexWorldPositions;
	}
	else
	{
		UpdateWorldVertices();
		return vertexWorldPositions;
	}
}

std::vector<glm::vec3> GameObject::GetTriangleWorldNormals()
{

	if (validWorldPositions)
	{
		return triangleWorldNormals;
	}
	else
	{
		UpdateWorldVertices();
		return triangleWorldNormals;
	}
}

std::vector<glm::vec3> GameObject::GetTriangleWorldPositions()
{
	if (validWorldPositions)
	{
		return triangleWorldPositions;
	}
	else
	{
		UpdateWorldVertices();
		return triangleWorldPositions;
	}
}

void GameObject::UpdateWorldVertices()
{
	// converting transformations to mat4 for combination
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	translationMatrix = glm::translate(translationMatrix, translation);
	rotationMatrix = glm::mat4_cast(rotation);
	scaleMatrix = glm::scale(scaleMatrix, scale);

	std::vector<Mesh>& meshes = model.meshes;
	std::vector<glm::mat4>& transforms = model.meshTransforms;

	for (int meshIndex = 0; meshIndex < meshes.size(); ++meshIndex)
	{
		Mesh& mesh = meshes[meshIndex];
		glm::mat4& meshTransform = transforms[meshIndex];

		// setting offest in case of multiple meshes
		GLuint indexOffset = (GLuint)vertexWorldPositions.size();

		std::vector<Vertex>& vertices = meshes[meshIndex].vertices;

		for (int vertexIndex = 0; vertexIndex < vertices.size(); ++vertexIndex)
		{
			Vertex& vertex = vertices[vertexIndex];
			// Apply transformation to get world position
			// TODO: There might be a better way to do this (copied from shader)
			glm::vec3 vertexWorldPosition(meshTransform * translationMatrix * rotationMatrix * scaleMatrix * glm::vec4(vertex.position, 1.0f));
			glm::vec3 vertexWorldNormal(rotationMatrix * glm::vec4(vertex.normal, 1.0f));

			vertexWorldPositions.push_back(vertexWorldPosition);
			vertexWorldNormals.push_back(normalize(vertexWorldNormal));
		}

		// collecting vertices in triangle order
		std::vector<GLuint>& indices = meshes[meshIndex].indices;

		for (int i = 0; i < indices.size(); ++i)
		{
			GLuint& vertexIndice = indices[i];
			glm::vec3& worldPosition = vertexWorldPositions[vertexIndice + indexOffset];
			glm::vec3& worldNormal = vertexWorldNormals[vertexIndice + indexOffset];
			triangleWorldPositions.push_back(worldPosition);
			triangleWorldNormals.push_back(worldNormal);
		}
	}
	validWorldPositions = true;
}

void GameObject::Delete()
{
	delete this;
}