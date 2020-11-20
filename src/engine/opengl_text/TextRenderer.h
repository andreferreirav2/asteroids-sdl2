#include <iostream>
#include <map>
#include <string>

#include <gl/glew.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H


/// Holds all state information relevant to a character as loaded using FreeType
struct Character
{
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class TextRenderer
{
public:
	TextRenderer(std::string fontPath, std::string textVertexShaderPath, std::string textFragShaderPath);
	glm::vec2 renderText(std::string text, float x, float y, float scale, glm::vec3 color, glm::vec2 screen);

private:
	void loadShader(std::string textVertexShaderPath, std::string textFragShaderPath);

	GLuint m_glProgramID, m_vaoID, m_vboID;
	std::map<GLchar, Character> m_characters;
};

