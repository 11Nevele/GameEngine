#pragma once
#include "Render/Buffer.h"

namespace ac
{
	class OpenGLVertexBuffer : public VertexBuffer
	{

	public:
		OpenGLVertexBuffer() = default;
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(unique_ptr<float[]> vertices, uint32_t size, const BufferLayout& layout);
		OpenGLVertexBuffer(const OpenGLVertexBuffer& other) = delete;
		OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept;
		virtual ~OpenGLVertexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Upload() override;
		virtual void Delete() override;
		virtual bool IsUploaded() const override;

		virtual void SetData(unique_ptr<float[]> data, uint32_t size) override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
	private:
		unique_ptr<float[]> vertices;
		uint32_t m_RendererID = 0;
		uint32_t size = 0;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer() = default;
		OpenGLIndexBuffer(unique_ptr<uint32_t[]> indices, uint32_t count);
		OpenGLIndexBuffer(const OpenGLIndexBuffer& other) = delete;
		OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept;
		virtual ~OpenGLIndexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Upload() override;
		virtual void Delete() override;
		virtual bool IsUploaded() const override;

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		unique_ptr<uint32_t[]> indicies;
		uint32_t m_RendererID = 0;
		uint32_t m_Count = 0;
	};

}