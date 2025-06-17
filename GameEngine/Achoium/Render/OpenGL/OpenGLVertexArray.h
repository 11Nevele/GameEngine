#pragma once
#include "Render/VertexArray.h"
namespace ac
{
	/**
	 * @brief OpenGL implementation of a vertex array object.
	 * 
	 * This class provides the OpenGL-specific implementation of the abstract VertexArray interface.
	 * It manages the creation, binding, and configuration of OpenGL Vertex Array Objects (VAOs),
	 * which store the state needed for rendering vertex data.
	 */
	class OpenGLVertexArray: public VertexArray
	{
	public:
		/**
		 * @brief Constructs a new OpenGL vertex array.
		 */
		OpenGLVertexArray();
		
		/**
		 * @brief Destructor. Cleans up OpenGL resources.
		 */
		virtual ~OpenGLVertexArray() override;
		
		/**
		 * @brief Binds this vertex array for use in rendering.
		 */
		virtual void Bind() const override;
		
		/**
		 * @brief Unbinds this vertex array.
		 */
		virtual void Unbind() const override;
		
		/**
		 * @brief Checks if the vertex array has been uploaded to GPU.
		 * 
		 * @return true If the vertex array is uploaded
		 * @return false If the vertex array is not uploaded
		 */
		virtual bool IsUploaded() const override;
		
		/**
		 * @brief Uploads the vertex array to the GPU.
		 * 
		 * Creates the VAO on the GPU if it hasn't been created yet.
		 */
		virtual void Upload() override;
		
		/**
		 * @brief Deletes the vertex array from GPU memory.
		 */
		virtual void Delete() override;
		
		/**
		 * @brief Adds a vertex buffer to this vertex array.
		 * 
		 * Links the vertex attributes from the provided buffer to this VAO.
		 * 
		 * @param vertexBuffer The vertex buffer to add
		 */
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		
		/**
		 * @brief Sets the index buffer for this vertex array.
		 * 
		 * @param indexBuffer The index buffer to use
		 */
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		
		/**
		 * @brief Gets the collection of vertex buffers attached to this array.
		 * 
		 * @return const std::vector<std::shared_ptr<VertexBuffer>>& Reference to the vertex buffer collection
		 */
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;
		
		/**
		 * @brief Gets the index buffer attached to this array.
		 * 
		 * @return const std::shared_ptr<IndexBuffer>& Reference to the index buffer
		 */
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;
		
	private:
		/**
		 * @brief Sets up vertex attribute pointers based on buffer layout.
		 * 
		 * @param layout The buffer layout describing the vertex attributes
		 */
		void SetAttrib(const BufferLayout& layout);
		
		bool isUploaded = false;                  ///< Flag indicating if the VAO has been uploaded to GPU
		uint32_t m_RendererID;                    ///< OpenGL handle to the VAO
		uint32_t m_VertexBufferIndex = 0;         ///< Index for the next vertex buffer to be added
		std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers; ///< Collection of vertex buffers
		std::shared_ptr<IndexBuffer> indexBuffers;               ///< The index buffer
	};
}

