#pragma once
#include "Renderer.h"
#include "Shader.h"

class Texture
{
public:
	Texture(const std::string& path);
	Texture(const std::string& path, Shader& shader);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	inline unsigned int GetRendererID() const { return m_RendererID; }

private:
	void SetupTexture(const std::string& path);

private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_localBuffer;
	int m_Width, m_Height, m_BPP;

};