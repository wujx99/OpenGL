#pragma once
#include <string>
class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_RawData;
	int m_BPP;
	int m_Width;
	int m_Heigth;

public:
	Texture(const std::string& filePath);
	~Texture();
	inline unsigned int GetWidth() { return m_Width; };
	inline unsigned int GetHeigth() { return m_Heigth; };

	void Bind(unsigned int slot = 0);
	void UnBind();
};
