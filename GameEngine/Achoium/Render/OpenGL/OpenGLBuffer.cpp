#include "acpch.h"
#include "OpenGLBuffer.h"
#include "Debug.h"
namespace ac
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size):size(size),m_RendererID(0),m_Layout()
	{
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(unique_ptr<float[]> vertices, uint32_t size, const BufferLayout& layout) :
		vertices(std::move(vertices)), size(size), m_Layout(layout), m_RendererID(0)
	{
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept:
		vertices(std::move(other.vertices)), size(other.size), m_RendererID(other.m_RendererID),
		m_Layout(other.m_Layout)
	{
		other.m_RendererID = 0;
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{

		ACMSG("VBO: " << m_RendererID << " Deleted");
		glDeleteBuffers(1, &m_RendererID);
		delete vertices.release();
	}
	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void OpenGLVertexBuffer::Upload()
	{
		glGenBuffers(1, &m_RendererID);
		ACMSG("VBO: " << m_RendererID << " Created");
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices.get(), GL_STATIC_DRAW);
	}
	void OpenGLVertexBuffer::Delete()
	{
		glDeleteBuffers(1, &m_RendererID);
	}
	bool OpenGLVertexBuffer::IsUploaded() const
	{
		return m_RendererID != 0;
	}
	void OpenGLVertexBuffer::SetData(unique_ptr<float[]> data, uint32_t size)
	{
		delete this->vertices.release();
		vertices = std::move(data);
		this->size = size;
		
		if (m_RendererID != 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices.get());
		}

		
	}


	OpenGLIndexBuffer::OpenGLIndexBuffer(unique_ptr<uint32_t[]> indices, uint32_t count)
		: m_Count(count), indicies(std::move(indices)), m_RendererID(0)
	{
		}
	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void OpenGLIndexBuffer::Upload()
	{
		glCreateBuffers(1, &m_RendererID);
		ACMSG("EBO: " << m_RendererID << " Created");
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), indicies.get(), GL_STATIC_DRAW);

	}
	void OpenGLIndexBuffer::Delete()
	{
		glDeleteBuffers(1, &m_RendererID);
	}
	bool OpenGLIndexBuffer::IsUploaded() const
	{
		return m_RendererID!=0;
	}
	
	OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept:
		indicies(std::move(other.indicies)), m_RendererID(other.m_RendererID), m_Count(other.m_Count)
	{
		other.m_RendererID = 0;
		other.m_Count = 0;
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		ACMSG("EBO: " << m_RendererID << " Deleted");
		glDeleteBuffers(1, &m_RendererID);
		delete indicies.release();
	}
}