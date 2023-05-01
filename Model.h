#pragma once

#include <json/json.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBufferObject.h"

using json = nlohmann::json;

class Model
{
public:
	// Loads in a model from a file and stores file info
	Model(const char* file, glm::vec3 modelOffset = glm::vec3(0.0f, 0.0f, 0.0f));

	void Draw
	(
		Shader& shader,
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::mat4 objectTransform = glm::mat4(1.0f)
	);

	std::vector<Mesh> meshes;
	std::vector<glm::mat4> meshTransforms;

private:
	glm::vec3 baseTranslation;
	// gtlf file data
	const char* file;
	std::vector<unsigned char> bin;
	json gtlfJSON;

	// Holds number of instances (if 1 the mesh will be rendered normally)
	unsigned int instancing;

	// textures
	std::vector<std::string> loadedTexturePaths;
	std::vector<Texture> loadedTextures;

	// Load Mesh by index
	void LoadMesh(unsigned int indMesh);

	// Traverse node and children recursively set object data members
	void TraverseNode(unsigned int nextNode, glm::mat4 IntialTransformMatrix = glm::mat4(1.0f));

	// Gets the binary data from a file
	std::vector<unsigned char> LoadBin();

	// Interpret binary data into values
	std::vector<float> FloatsFromBin(json accessor);
	std::vector<GLuint> IndicesFromBin(json accessor);
	std::vector<Texture> TexturesFromBin();

	// Assembles all the floats into vertices
	std::vector<Vertex> AssembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	// filters floats by type
	std::vector<glm::vec2> GroupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> GroupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> GroupFloatsVec4(std::vector<float> floatVec);
};