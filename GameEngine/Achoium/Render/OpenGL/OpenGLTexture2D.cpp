#include "acpch.h"
#include "OpenGLTexture2D.h"
#include "stb_image.h"
#include "glad.h"
#include "Debug.h"
namespace ac
{
	OpenGLTexture2D::OpenGLTexture2D(OpenGLTexture2D&& other) noexcept:
		m_RenderID(other.m_RenderID), textureInfo(other.textureInfo)
	{
		other.m_RenderID = 0;
	}

	ac::OpenGLTexture2D::OpenGLTexture2D(const std::string& path):m_RenderID(0)
	{

		int width, height, channel;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channel, 0);

		GLenum internalFormat = 0, dataFormat = 0;
		if (channel == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channel == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		ACASSERT(data, "FAIL TO READ DATA FROM" << path);
		this->textureInfo.height = height;
		this->textureInfo.width = width;
		textureInfo.internalFormat = internalFormat;
		textureInfo.dataFormat = dataFormat;

		glGenTextures(1, &m_RenderID);
		glBindTexture(GL_TEXTURE_2D, m_RenderID);
		glTextureStorage2D(m_RenderID, 1, textureInfo.internalFormat, textureInfo.width, textureInfo.height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_RenderID, 0, 0, 0,
			textureInfo.width, textureInfo.height,
			textureInfo.dataFormat, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);  // <-- ADD THIS LINE

		glBindTexture(GL_TEXTURE_2D, 0);
		
		stbi_image_free(data);
	}
	OpenGLTexture2D::OpenGLTexture2D(stbi_uc* data, TextureInfo info):
		textureInfo(info), m_RenderID(0)
	{
		glGenTextures(1, &m_RenderID);
		glBindTexture(GL_TEXTURE_2D, m_RenderID);
		glTextureStorage2D(m_RenderID, 1, textureInfo.internalFormat, textureInfo.width, textureInfo.height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_RenderID, 0, 0, 0,
			textureInfo.width, textureInfo.height,
			textureInfo.dataFormat, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);  // <-- ADD THIS LINE

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void OpenGLTexture2D::SetData(stbi_uc* data, TextureInfo info)
	{
		stbi_image_free(data);
		this->textureInfo = info;
		
		if (m_RenderID != 0)//refresh data in 
		{
			glBindTexture(GL_TEXTURE_2D, m_RenderID);
			glTextureStorage2D(m_RenderID, 1, textureInfo.internalFormat, textureInfo.width, textureInfo.height);


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureSubImage2D(m_RenderID, 0, 0, 0,
				textureInfo.width, textureInfo.height,
				textureInfo.dataFormat, GL_UNSIGNED_BYTE, data);
		}
		
	}

	ac::OpenGLTexture2D::~OpenGLTexture2D()
	{

		ACMSG("OpenGLTexture2D Delete with id: " << m_RenderID);
		glDeleteTextures(1, &m_RenderID);
	}

	uint32_t ac::OpenGLTexture2D::GetWidth() const
	{
		return textureInfo.width;
	}

	uint32_t ac::OpenGLTexture2D::GetHeight() const
	{
		return textureInfo.height;
	}

	TextureInfo ac::OpenGLTexture2D::GetTextureInfo() const
	{
		return textureInfo;
	}

	void ac::OpenGLTexture2D::Bind(uint32_t slot) const
	{
		if(m_RenderID == 0)
			ACMSG("Trying to bind a texture that is not uploaded!!!");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_RenderID);
	}
}

