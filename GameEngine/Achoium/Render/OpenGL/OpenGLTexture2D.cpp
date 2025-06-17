#include "acpch.h"
#include "OpenGLTexture2D.h"
#include "stb_image.h"
#include "glad.h"
#include "Debug.h"
namespace ac
{
	OpenGLTexture2D::OpenGLTexture2D(OpenGLTexture2D&& other) noexcept:
		m_RenderID(other.m_RenderID), textureInfo(other.textureInfo), data(other.data)
	{
		other.data = nullptr;
		other.m_RenderID = 0;
	}

	ac::OpenGLTexture2D::OpenGLTexture2D(const std::string& path):m_RenderID(0)
	{
		int width, height, channel;
		data = stbi_load(path.c_str(), &width, &height, &channel, 0);

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
	
	}
	OpenGLTexture2D::OpenGLTexture2D(stbi_uc* data, TextureInfo info):
		data(data), textureInfo(info), m_RenderID(0)
	{
	}
	void OpenGLTexture2D::SetData(stbi_uc* data, TextureInfo info)
	{
		stbi_image_free(data);
		this->textureInfo = info;
		this->data = data;
		
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
		stbi_image_free(data);
		glDeleteTextures(1, &m_RenderID);
	}

	void OpenGLTexture2D::Upload()
	{
		if (m_RenderID != 0)
		{
			ACMSG("Texture: " << m_RenderID << " is already uploaded");
			return;
		}
		glGenTextures(1, & m_RenderID);
		glBindTexture(GL_TEXTURE_2D, m_RenderID);
		glTextureStorage2D(m_RenderID, 1, textureInfo.internalFormat, textureInfo.width, textureInfo.height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_RenderID, 0, 0, 0,
			textureInfo.width, textureInfo.height,
			textureInfo.dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Delete()
	{
		glDeleteTextures(1, &m_RenderID);
	}

	bool OpenGLTexture2D::IsUploaded() const
	{
		return m_RenderID != 0;
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
		glBindTextureUnit(slot, m_RenderID);
	}
}

