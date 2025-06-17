#pragma once
#include "Render/Buffer.h"

namespace ac
{
    /**
     * @brief OpenGL implementation of vertex buffer.
     * 
     * Manages OpenGL vertex buffer objects (VBOs) for storing and accessing
     * vertex data on the GPU.
     */
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        /**
         * @brief Default constructor.
         */
        OpenGLVertexBuffer() = default;
        
        /**
         * @brief Constructs a vertex buffer with a specified size.
         * 
         * @param size Size of the buffer in bytes
         */
        OpenGLVertexBuffer(uint32_t size);
        
        /**
         * @brief Constructs a vertex buffer with data and layout.
         * 
         * @param vertices Array of vertex data
         * @param size Size of the vertex data in bytes
         * @param layout Buffer layout describing the vertex attributes
         */
        OpenGLVertexBuffer(unique_ptr<float[]> vertices, uint32_t size, const BufferLayout& layout);
        
        /**
         * @brief Copy constructor (deleted).
         */
        OpenGLVertexBuffer(const OpenGLVertexBuffer& other) = delete;
        
        /**
         * @brief Move constructor.
         * 
         * @param other Buffer to move from
         */
        OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept;
        
        /**
         * @brief Destructor. Cleans up OpenGL resources.
         */
        virtual ~OpenGLVertexBuffer() override;

        /**
         * @brief Binds the vertex buffer for use.
         */
        virtual void Bind() const override;
        
        /**
         * @brief Unbinds the vertex buffer.
         */
        virtual void Unbind() const override;

        /**
         * @brief Uploads vertex data to the GPU.
         */
        virtual void Upload() override;
        
        /**
         * @brief Deletes the vertex buffer from GPU memory.
         */
        virtual void Delete() override;
        
        /**
         * @brief Checks if the buffer has been uploaded to GPU.
         * 
         * @return true If the buffer is uploaded
         * @return false If the buffer is not uploaded
         */
        virtual bool IsUploaded() const override;

        /**
         * @brief Sets the buffer data.
         * 
         * @param data Array of vertex data to set
         * @param size Size of the vertex data in bytes
         */
        virtual void SetData(unique_ptr<float[]> data, uint32_t size) override;

        /**
         * @brief Sets the buffer layout.
         * 
         * @param layout Buffer layout describing the vertex attributes
         */
        virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
        
        /**
         * @brief Gets the buffer layout.
         * 
         * @return const BufferLayout& Reference to the buffer layout
         */
        virtual const BufferLayout& GetLayout() const override { return m_Layout; }
    private:
        unique_ptr<float[]> vertices;  ///< Vertex data stored on CPU
        uint32_t m_RendererID = 0;     ///< OpenGL handle to the buffer
        uint32_t size = 0;             ///< Size of the buffer in bytes
        BufferLayout m_Layout;         ///< Layout of the vertex attributes
    };

    /**
     * @brief OpenGL implementation of index buffer.
     * 
     * Manages OpenGL index buffer objects (IBOs/EBOs) for storing and accessing
     * index data on the GPU.
     */
    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        /**
         * @brief Default constructor.
         */
        OpenGLIndexBuffer() = default;
        
        /**
         * @brief Constructs an index buffer with data.
         * 
         * @param indices Array of index data
         * @param count Number of indices
         */
        OpenGLIndexBuffer(unique_ptr<uint32_t[]> indices, uint32_t count);
        
        /**
         * @brief Copy constructor (deleted).
         */
        OpenGLIndexBuffer(const OpenGLIndexBuffer& other) = delete;
        
        /**
         * @brief Move constructor.
         * 
         * @param other Buffer to move from
         */
        OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept;
        
        /**
         * @brief Destructor. Cleans up OpenGL resources.
         */
        virtual ~OpenGLIndexBuffer() override;

        /**
         * @brief Binds the index buffer for use.
         */
        virtual void Bind() const override;
        
        /**
         * @brief Unbinds the index buffer.
         */
        virtual void Unbind() const override;

        /**
         * @brief Uploads index data to the GPU.
         */
        virtual void Upload() override;
        
        /**
         * @brief Deletes the index buffer from GPU memory.
         */
        virtual void Delete() override;
        
        /**
         * @brief Checks if the buffer has been uploaded to GPU.
         * 
         * @return true If the buffer is uploaded
         * @return false If the buffer is not uploaded
         */
        virtual bool IsUploaded() const override;

        /**
         * @brief Gets the number of indices in the buffer.
         * 
         * @return uint32_t Number of indices
         */
        virtual uint32_t GetCount() const override { return m_Count; }
    private:
        unique_ptr<uint32_t[]> indicies; ///< Index data stored on CPU
        uint32_t m_RendererID = 0;       ///< OpenGL handle to the buffer
        uint32_t m_Count = 0;            ///< Number of indices in the buffer
    };
}