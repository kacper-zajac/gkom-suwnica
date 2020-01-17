#include "Texture.h"
#include <SOIL.h>

Texture::Texture(const std::string& path)
	: m_FilePath(path), m_LocalBuffer(nullptr), 
	m_Width(0), m_Height(0), m_BPP(0)
{
	m_LocalBuffer = SOIL_load_image(path.c_str(), &m_Width, &m_Height, &m_BPP, SOIL_LOAD_RGB);

	glGenTextures(1, &m_RendererID); 
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// GL_CLAMP

	// ew. GL_RGB
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer)
		SOIL_free_image_data(m_LocalBuffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);

}
