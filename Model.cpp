#include"Model.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "FileLoader.h"

Model::Model(const char* file, glm::vec3 modelOffset) :
	baseTranslation(modelOffset)
{
	if (file != nullptr)
	{
		std::string text;
		// Make a gtlfJSON object
		try
		{
			text = FileLoader::GetFileContents(file);
		}
		catch (...)
		{
			std::cout << "Model failed to load" << std::endl;
			std::cout << "File: " << file << std::endl;
		}


		gtlfJSON = json::parse(text);

		// Get the binary data
		Model::file = file;
		bin = LoadBin();

		// Traverse all nodes
		TraverseNode(0);
	}
}

void Model::Draw(Shader& shader, glm::vec3 translation, glm::quat rotation, glm::vec3 scale, glm::mat4 objectTransform)
{
	// Go over all meshes and draw each one
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Mesh::Draw(shader, meshTransforms[i], translation, rotation, scale, objectTransform);
	}
}

void Model::LoadMesh(unsigned int indMesh)
{
	// Get all mesh accessor indices
	unsigned int positionAccessorIndex
		= gtlfJSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccessorIndex
		= gtlfJSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int textureAccessorIndex
		= gtlfJSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indiceAccessorIndex
		= gtlfJSON["meshes"][indMesh]["primitives"][0]["indices"];

	// Get vertex components from binary
	std::vector<float> posVec = FloatsFromBin(gtlfJSON["accessors"][positionAccessorIndex]);
	std::vector<glm::vec3> positions = GroupFloatsVec3(posVec);
	std::vector<float> normalVec = FloatsFromBin(gtlfJSON["accessors"][normalAccessorIndex]);
	std::vector<glm::vec3> normals = GroupFloatsVec3(normalVec);
	std::vector<float> texVec = FloatsFromBin(gtlfJSON["accessors"][textureAccessorIndex]);
	std::vector<glm::vec2> texUVs = GroupFloatsVec2(texVec);

	// prepare mesh components
	std::vector<Vertex> vertices = AssembleVertices(positions, normals, texUVs);
	std::vector<GLuint> indices = IndicesFromBin(gtlfJSON["accessors"][indiceAccessorIndex]);
	std::vector<Texture> textures = TexturesFromBin();

	// create Mesh
	meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::TraverseNode(unsigned int nextNode, glm::mat4 IntialTransformMatrix)
{
	json node = gtlfJSON["nodes"][nextNode];

	// Get translation if present
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			transValues[i] = (node["translation"][i]);
		translation = glm::make_vec3(transValues);
	}
	// Get quaternion if present
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		float rotValues[4] =
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		rotation = glm::make_quat(rotValues);
	}
	// Get scale if present
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);
		scale = glm::make_vec3(scaleValues);
	}
	// Get matrix if present
	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = (node["matrix"][i]);
		matNode = glm::make_mat4(matValues);
	}

	// convert transformations to matricies
	glm::mat4 translationMat = glm::mat4(1.0f);
	glm::mat4 rotationMat = glm::mat4(1.0f);
	glm::mat4 scaleMat = glm::mat4(1.0f);

	translationMat = glm::translate(translationMat, translation);
	rotationMat = glm::mat4_cast(rotation);
	scaleMat = glm::scale(scaleMat, scale);

	// Create updated Transformation matrix
	glm::mat4 finalTransformMatrix = IntialTransformMatrix * matNode * translationMat * rotationMat * scaleMat;

	// Load mesh if present
	if (node.find("mesh") != node.end())
	{
		meshTransforms.push_back(finalTransformMatrix);

		LoadMesh(node["mesh"]);
	}

	// Pass Transformation Matrix to children
	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); i++)
			TraverseNode(node["children"][i], finalTransformMatrix);
	}
}

std::vector<unsigned char> Model::LoadBin()
{
	// Create a place to store the raw text, and get the uri of the .bin file
	std::string bytesText;
	std::string uri = gtlfJSON["buffers"][0]["uri"];

	// Store raw text data into bytesText
	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesText = FileLoader::GetFileContents((fileDirectory + uri).c_str());

	// Transform the raw text data into bytes and put them in a vector
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
	return data;
}

std::vector<float> Model::FloatsFromBin(json accessor)
{
	std::vector<float> floatVec;

	// Get meta data from accessor
	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	// Get meta data from accessor's bufferView
	json bufferView = gtlfJSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// interpret data size
	unsigned int numPerVert;
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

	// Load data
	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVert;
	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
	{
		unsigned char bytes[] = { bin[i++], bin[i++], bin[i++], bin[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
	}

	return floatVec;
}

std::vector<GLuint> Model::IndicesFromBin(json accessor)
{
	std::vector<GLuint> indices;

	// Get meta data from accessor
	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	// Get meta data from accessor's bufferView
	json bufferView = gtlfJSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Get indices with regards to their type: unsigned int, unsigned short, or short
	unsigned int beginningOfData = byteOffset + accByteOffset;
	if (componentType == 5125)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bytes[] = { bin[i++], bin[i++], bin[i++], bin[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { bin[i++], bin[i++] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { bin[i++], bin[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}

	return indices;
}

std::vector<Texture> Model::TexturesFromBin()
{
	std::vector<Texture> textures;

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	// Go over all images
	for (unsigned int i = 0; i < gtlfJSON["images"].size(); i++)
	{
		// uri of current texture
		std::string texPath = gtlfJSON["images"][i]["uri"];

		// Check if the texture has already been loaded
		bool skip = false;
		for (unsigned int j = 0; j < loadedTexturePaths.size(); j++)
		{
			if (loadedTexturePaths[j] == texPath)
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}

		// If the texture has been loaded, skip this
		if (!skip)
		{
			// Load diffuse texture
			if (texPath.find("baseColor") != std::string::npos || texPath.find("diffuse") != std::string::npos)
			{
				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", (GLuint)loadedTextures.size());
				textures.push_back(diffuse);
				loadedTextures.push_back(diffuse);
				loadedTexturePaths.push_back(texPath);
			}
			// Load specular texture
			else if (texPath.find("metallicRoughness") != std::string::npos || texPath.find("specular") != std::string::npos)
			{
				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", (GLuint)loadedTextures.size());
				textures.push_back(specular);
				loadedTextures.push_back(specular);
				loadedTexturePaths.push_back(texPath);
			}
		}
	}

	return textures;
}

std::vector<Vertex> Model::AssembleVertices
(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs
)
{
	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++)
	{
		vertices.push_back
		(
			Vertex
			{
				positions[i] + baseTranslation,
				normals[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				texUVs[i]
			}
		);
	}
	return vertices;
}

std::vector<glm::vec2> Model::GroupFloatsVec2(std::vector<float> floatVec)
{
	std::vector<glm::vec2> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}
	return vectors;
}
std::vector<glm::vec3> Model::GroupFloatsVec3(std::vector<float> floatVec)
{
	std::vector<glm::vec3> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}
std::vector<glm::vec4> Model::GroupFloatsVec4(std::vector<float> floatVec)
{
	std::vector<glm::vec4> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}