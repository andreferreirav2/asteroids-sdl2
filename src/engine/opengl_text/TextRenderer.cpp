#include "TextRenderer.h"
#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

TextRenderer::TextRenderer(std::string fontPath, std::string textVertexShaderPath, std::string textFragShaderPath)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Build shader
	loadShader(textVertexShaderPath, textFragShaderPath);

	// FreeType
	// --------
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}
	else
	{
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			m_characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// configure m_vaoID/m_vboID for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &m_vaoID);
	glGenBuffers(1, &m_vboID);
	glBindVertexArray(m_vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

glm::vec2 TextRenderer::renderText(std::string text, float x, float y, float scale, glm::vec3 color, glm::vec2 screen)
{
	// activate corresponding render state	
	glUseProgram(m_glProgramID);

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screen.x), 0.0f, static_cast<float>(screen.y)); // TODO remove from here
	glUniformMatrix4fv(glGetUniformLocation(m_glProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniform3f(glGetUniformLocation(m_glProgramID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_vaoID);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = m_characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update m_vboID for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of m_vboID memory
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return { x, y };
}

void TextRenderer::loadShader(std::string textVertexShaderPath, std::string textFragShaderPath)
{
	// Define shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load shaders from file
	string vertexShaderCode;
	ifstream vertexShaderStream = ifstream(textVertexShaderPath, ios::in);
	if (vertexShaderStream.is_open())
	{
		stringstream sstr;
		sstr << vertexShaderStream.rdbuf();
		vertexShaderCode = sstr.str();
		vertexShaderStream.close();
	}
	else
	{
		cerr << "Unable to open vertex shader file: " << textVertexShaderPath << endl;
	}

	string fragmentShaderCode;
	ifstream fragmentShaderStream = ifstream(textFragShaderPath, ios::in);
	if (fragmentShaderStream.is_open())
	{
		stringstream sstr;
		sstr << fragmentShaderStream.rdbuf();
		fragmentShaderCode = sstr.str();
		fragmentShaderStream.close();
	}
	else
	{
		cerr << "Unable to open fragment shader file: " << textFragShaderPath << endl;
	}


	GLint result = GL_FALSE;
	int InfoLogLength;

	// Compile vertex shader
	//const GLchar* vertexShaderSource = "#version 140\nin vec3 LVertexPos3D; void main() { gl_Position = vec4( LVertexPos3D.x, LVertexPos3D.y, LVertexPos3D.z, 1 ); }";
	char const* vertexShaderSource = vertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//Check for vertex shader errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (result != GL_TRUE)
	{
		auto vertexShaderErrorMessage = vector<char>(InfoLogLength + 1);
		glGetShaderInfoLog(vertexShader, InfoLogLength, NULL, &vertexShaderErrorMessage[0]);
		cerr << "Unable to compile vertex shader: " << vertexShader << endl;
		cerr << &vertexShaderErrorMessage[0] << endl;
	}

	// Compile fragment shader
	//const GLchar* fragmentShaderSource = "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }";
	char const* fragmentShaderSource = fragmentShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//Check for fragment shader errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (result != GL_TRUE)
	{
		auto fragmentShaderErrorMessage = vector<char>(InfoLogLength + 1);
		glGetShaderInfoLog(fragmentShader, InfoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		cerr << "Unable to compile fragment shader: " << fragmentShader << endl;
		cerr << &fragmentShaderErrorMessage[0] << endl;
	}


	// Link gl program
	m_glProgramID = glCreateProgram();
	glAttachShader(m_glProgramID, vertexShader);
	glAttachShader(m_glProgramID, fragmentShader);
	glLinkProgram(m_glProgramID);
	//Check for errors
	glGetShaderiv(m_glProgramID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(m_glProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (result != GL_TRUE)
	{
		auto programErrorMessage = vector<char>(InfoLogLength + 1);
		glGetShaderInfoLog(m_glProgramID, InfoLogLength, NULL, &programErrorMessage[0]);
		cerr << "Unable to compile gl program: " << m_glProgramID << endl;
		cerr << &programErrorMessage[0] << endl;
	}

	glDetachShader(m_glProgramID, vertexShader);
	glDetachShader(m_glProgramID, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
