#ifndef __TEXTURE_H__
#define __TEXTURE_H__

class Texture
{
public:
	Texture();
	~Texture();

	inline operator unsigned int() { return m_textureID; }

	bool Bind(unsigned int a_texID);
	void Reload(std::string a_newPath = ""); // Recreates the texture, if supplied with a path, loads that image
	inline unsigned int GetID() { return m_textureID; }
	inline glm::vec2 GetSize() { return m_textureSize; }

private:
	bool LoadTexture(const std::string& a_texturePath);

private:
	unsigned int m_textureID;
	std::string m_filePath;
	glm::vec2 m_textureSize;
	bool m_isActive;

public:
	static std::shared_ptr<Texture> CreateTexture(const std::string& a_texturePath);
};

#endif