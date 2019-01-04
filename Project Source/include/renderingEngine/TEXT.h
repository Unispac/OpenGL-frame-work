#ifndef  TEXT_H
#define TEXT_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
#include<renderingEngine/MYTOOLKIT.h>
#include<ft2build.h>
#include <vector>

#include FT_FREETYPE_H  
using namespace std;
class Text
{
private:
	struct Character {
		GLuint     TextureID;  // ���������ID
		glm::ivec2 Size;       // ���δ�С
		glm::ivec2 Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
		GLuint     Advance;    // ԭ�����һ������ԭ��ľ���
	};
	map<GLchar, Character> Characters;
	unsigned int VAO, VBO;

	void initial()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
public:
	Text(string fontPath)
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
			std::cout<< "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

		FT_Face face;
		if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

		FT_Set_Pixel_Sizes(face, 0, 24);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //�����ֽڶ�������
		for (GLubyte c = 0; c < 128; c++)
		{
			// �����ַ������� 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// ��������
			GLuint texture;
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
			// ��������ѡ��
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// �����ַ���֮��ʹ��
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			Characters.insert(std::pair<GLchar, Character>(c, character));
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
		initial();
	}
	void RenderText(Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
	{
		// �����Ӧ����Ⱦ״̬
		s.use();
		s.setFloat3("textColor", color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);
		// �����ı������е��ַ�
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			GLfloat xpos = x + ch.Bearing.x * scale;
			GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			GLfloat w = ch.Size.x * scale;
			GLfloat h = ch.Size.y * scale;
			// ��ÿ���ַ�����VBO
			GLfloat vertices[6][4] = {
				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos,     ypos,       0.0, 1.0 },
				{ xpos + w, ypos,       1.0, 1.0 },

				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos + w, ypos,       1.0, 1.0 },
				{ xpos + w, ypos + h,   1.0, 0.0 }
			};
			// ���ı����ϻ�����������

			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// ����VBO�ڴ������
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// �����ı���
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// ����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
			x += (ch.Advance >> 6) * scale; // λƫ��6����λ����ȡ��λΪ���ص�ֵ (2^6 = 64)
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

};


#endif // ! TEXT_H