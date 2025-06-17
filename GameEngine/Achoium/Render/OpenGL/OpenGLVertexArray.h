#pragma once
#include "Render/VertexArray.h"
namespace ac
{
	class OpenGLVertexArray: public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual bool IsUploaded() const override;
		virtual void Upload() override;
		virtual void Delete() override;
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;
		void SetAttrib(const BufferLayout& layout);
	private:
		
		bool isUploaded = false;
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
		std::shared_ptr<IndexBuffer> indexBuffers;
	};
}

