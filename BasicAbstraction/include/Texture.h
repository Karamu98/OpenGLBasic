#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>

class Texture
{
public:
	Texture(std::string a_textPath);
	~Texture();

	inline operator unsigned int() { return m_textureID; }

	void Bind(unsigned int a_texID);
	void Reload(std::string a_newPath = ""); // Recreates the texture, if supplied with a path, loads that image

private:
	unsigned int m_textureID;
	std::string m_filePath;
};

#endif