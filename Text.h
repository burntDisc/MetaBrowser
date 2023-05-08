#pragma once

#include "Shader.h"
#include <string>
#include <glm/glm.hpp>
#include <map>

#include "Player.h"

class Text
{
private:
    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    Player& player;
    glm::vec3 translation;
    glm::mat4 translationMatrix;
    glm::mat4 scaleMatrix;

    glm::vec3 scale;

    glm::quat OrientationToRotation(glm::vec3 orientation);

    /// Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    std::map<GLchar, Character> Characters;

    unsigned int VAO, VBO;
public:
	Text(Shader shader, glm::vec3 translation, glm::quat rotation, glm::vec3 scale, Player& player);
    void RenderText(Shader& shader, std::string text, float x, float y, float size, glm::vec3 color);

};

