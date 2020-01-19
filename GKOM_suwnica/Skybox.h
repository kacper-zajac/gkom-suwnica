#include <gl/glew.h>
#include <SOIL.h>
#include <vector>
#include <iostream>

class Skybox {
	unsigned int m_RendererID;

	std::vector<std::string> faces
	{
	"textures/skybox/right1.jpg",
	"textures/skybox/left1.jpg",
	"textures/skybox/top1.jpg",
	"textures/skybox/bottom1.jpg",
	"textures/skybox/front1.jpg",
	"textures/skybox/back1.jpg"
	};

public:
	unsigned int loadCubemap()
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		int m_Width, m_Height, m_BPP;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char *m_LocalBuffer = SOIL_load_image(faces[i].c_str(), &m_Width, &m_Height, &m_BPP, SOIL_LOAD_RGB);
			if (m_LocalBuffer)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB,m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer
				);
				SOIL_free_image_data(m_LocalBuffer);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				SOIL_free_image_data(m_LocalBuffer);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		return m_RendererID;
	}

	void Bind(unsigned int slot = 0) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}

	~Skybox()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

};




// ... draw rest of the scene