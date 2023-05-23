#pragma once

#include "Shader.h"
#include <string>
#include <glm/glm.hpp>
#include <map>

#include <vector>
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
    std::string text;

    glm::quat OrientationToRotation(glm::vec3 orientation);

    /// Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    static std::vector<std::string> loadedFonts;
    static std::map<GLchar, Character> Characters;
    static unsigned int VAO, VBO;
public:
    static void LoadFont(std::string fontFile);
	Text(Shader shader, std::string text, glm::vec3 translation, glm::quat rotation, glm::vec3 scale, Player& player);
    void RenderText(Shader& shader, float x, float y, float size, glm::vec3 color);

};

