#include "Texture.h"
#include "Renderer.h"
#include "vendor/stbimage/stb_image.h"

Texture::Texture(const std::string& filePath)
	:m_RendererID(0),m_FilePath(filePath),m_RawData(nullptr),m_BPP(0),m_Width(0),m_Heigth(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_RawData = stbi_load(m_FilePath.c_str(), &m_Width, &m_Heigth, &m_BPP, 4);
	GLCallV(glGenTextures(1, &m_RendererID));
	GLCallV(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//you must set the following parameter, or you will get black window!
	GLCallV(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCallV(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCallV(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCallV(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// bind the texture data
	// note that the difference between internalformat and format in the glTexImage2D parameter!
	GLCallV(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_RawData));
	GLCallV(glBindTexture(GL_TEXTURE_2D, 0));

	//free data from cpu
	if (m_RawData)
		stbi_image_free(m_RawData);
}

Texture::~Texture()
{
	GLCallV(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot)
{
	// be careful with slot, GL_TEXTURE is different with GL_TEXTURE0
	GLCallV(glActiveTexture(GL_TEXTURE0 + slot));
	GLCallV(glBindTexture(GL_TEXTURE_2D, m_RendererID));

}

void Texture::UnBind()
{
	GLCallV(glBindTexture(GL_TEXTURE_2D, 0));
}
