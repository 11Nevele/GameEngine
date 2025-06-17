#include "acpch.h"  
#include "OpenGLBuffer.h"  
#include "Debug.h"  

namespace ac  
{  
   /// Constructor for OpenGLVertexBuffer.  
   /// Initializes the vertex buffer with a specified size.  
   /// @param size The size of the buffer in bytes.  
   OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) : size(size), m_RendererID(0), m_Layout()  
   {  
   }  

   /// Constructor for OpenGLVertexBuffer.  
   /// Initializes the vertex buffer with vertex data, size, and layout.  
   /// @param vertices A unique pointer to the vertex data.  
   /// @param size The size of the buffer in bytes.  
   /// @param layout The layout of the buffer describing its attributes.  
   OpenGLVertexBuffer::OpenGLVertexBuffer(unique_ptr<float[]> vertices, uint32_t size, const BufferLayout& layout) :  
       vertices(std::move(vertices)), size(size), m_Layout(layout), m_RendererID(0)  
   {  
   }  

   /// Move constructor for OpenGLVertexBuffer.  
   /// Transfers ownership of the vertex buffer data and properties.  
   /// @param other The other vertex buffer to move from.  
   OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept :  
       vertices(std::move(other.vertices)), size(other.size), m_RendererID(other.m_RendererID),  
       m_Layout(other.m_Layout)  
   {  
       other.m_RendererID = 0;  
   }  

   /// Destructor for OpenGLVertexBuffer.  
   /// Deletes the vertex buffer and releases its resources.  
   OpenGLVertexBuffer::~OpenGLVertexBuffer()  
   {  
       ACMSG("VBO: " << m_RendererID << " Deleted");  
       glDeleteBuffers(1, &m_RendererID);  
       delete vertices.release();  
   }  

   /// Binds the vertex buffer for rendering.  
   void OpenGLVertexBuffer::Bind() const  
   {  
       glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);  
   }  

   /// Unbinds the vertex buffer.  
   void OpenGLVertexBuffer::Unbind() const  
   {  
       glBindBuffer(GL_ARRAY_BUFFER, 0);  
   }  

   /// Uploads the vertex buffer data to the GPU.  
   void OpenGLVertexBuffer::Upload()  
   {  
       glGenBuffers(1, &m_RendererID);  
       ACMSG("VBO: " << m_RendererID << " Created");  
       glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);  
       glBufferData(GL_ARRAY_BUFFER, size, vertices.get(), GL_STATIC_DRAW);  
   }  

   /// Deletes the vertex buffer from the GPU.  
   void OpenGLVertexBuffer::Delete()  
   {  
       glDeleteBuffers(1, &m_RendererID);  
   }  

   /// Checks if the vertex buffer is uploaded to the GPU.  
   /// @return True if uploaded, false otherwise.  
   bool OpenGLVertexBuffer::IsUploaded() const  
   {  
       return m_RendererID != 0;  
   }  

   /// Sets new data for the vertex buffer.  
   /// @param data A unique pointer to the new vertex data.  
   /// @param size The size of the new data in bytes.  
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

   /// Constructor for OpenGLIndexBuffer.  
   /// Initializes the index buffer with index data and count.  
   /// @param indices A unique pointer to the index data.  
   /// @param count The number of indices.  
   OpenGLIndexBuffer::OpenGLIndexBuffer(unique_ptr<uint32_t[]> indices, uint32_t count)  
       : m_Count(count), indicies(std::move(indices)), m_RendererID(0)  
   {  
   }  

   /// Binds the index buffer for rendering.  
   void OpenGLIndexBuffer::Bind() const  
   {  
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);  
   }  

   /// Unbinds the index buffer.  
   void OpenGLIndexBuffer::Unbind() const  
   {  
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  
   }  

   /// Uploads the index buffer data to the GPU.  
   void OpenGLIndexBuffer::Upload()  
   {  
       glCreateBuffers(1, &m_RendererID);  
       ACMSG("EBO: " << m_RendererID << " Created");  
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);  
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), indicies.get(), GL_STATIC_DRAW);  
   }  

   /// Deletes the index buffer from the GPU.  
   void OpenGLIndexBuffer::Delete()  
   {  
       glDeleteBuffers(1, &m_RendererID);  
   }  

   /// Checks if the index buffer is uploaded to the GPU.  
   /// @return True if uploaded, false otherwise.  
   bool OpenGLIndexBuffer::IsUploaded() const  
   {  
       return m_RendererID != 0;  
   }  

   /// Move constructor for OpenGLIndexBuffer.  
   /// Transfers ownership of the index buffer data and properties.  
   /// @param other The other index buffer to move from.  
   OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept :  
       indicies(std::move(other.indicies)), m_RendererID(other.m_RendererID), m_Count(other.m_Count)  
   {  
       other.m_RendererID = 0;  
       other.m_Count = 0;  
   }  

   /// Destructor for OpenGLIndexBuffer.  
   /// Deletes the index buffer and releases its resources.  
   OpenGLIndexBuffer::~OpenGLIndexBuffer()  
   {  
       ACMSG("EBO: " << m_RendererID << " Deleted");  
       glDeleteBuffers(1, &m_RendererID);  
       delete indicies.release();  
   }  
}