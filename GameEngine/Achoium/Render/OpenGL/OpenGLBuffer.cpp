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
       glCreateBuffers(1, &m_RendererID);
       glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
       glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
   }  

   /// Constructor for OpenGLVertexBuffer.  
   /// Initializes the vertex buffer with vertex data, size, and layout.  
   /// @param vertices A unique pointer to the vertex data.  
   /// @param size The size of the buffer in bytes.  
   /// @param layout The layout of the buffer describing its attributes.  
   OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, const BufferLayout& layout) :  
       size(size), m_Layout(layout), m_RendererID(0)  
   {  
       glCreateBuffers(1, &m_RendererID);
       glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
       glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
   }

   OpenGLVertexBuffer::OpenGLVertexBuffer(const OpenGLVertexBuffer& other):
       size(other.size), m_RendererID(other.m_RendererID),
       m_Layout(other.m_Layout)
   {
   }

   /// Move constructor for OpenGLVertexBuffer.  
   /// Transfers ownership of the vertex buffer data and properties.  
   /// @param other The other vertex buffer to move from.  
   OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept :  
       size(other.size), m_RendererID(other.m_RendererID),  
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

   /// Sets new data for the vertex buffer.  
   /// @param data A unique pointer to the new vertex data.  
   /// @param size The size of the new data in bytes.  
   void OpenGLVertexBuffer::SetData(float* data, uint32_t size)  
   {  
       glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
       glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
   }  

   



   /// Constructor for OpenGLIndexBuffer.  
   /// Initializes the index buffer with index data and count.  
   /// @param indices A unique pointer to the index data.  
   /// @param count The number of indices.  
   OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)  
       : m_Count(count), m_RendererID(0)  
   {  
       glCreateBuffers(1, &m_RendererID);

       // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
       // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
   }

   OpenGLIndexBuffer::OpenGLIndexBuffer(const OpenGLIndexBuffer& other):
       m_Count(other.m_Count), m_RendererID(other.m_RendererID)
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


   /// Move constructor for OpenGLIndexBuffer.  
   /// Transfers ownership of the index buffer data and properties.  
   /// @param other The other index buffer to move from.  
   OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept :  
       m_RendererID(other.m_RendererID), m_Count(other.m_Count)  
   {  
       other.m_RendererID = 0;  
       other.m_Count = 0;  
   }  

   /// Destructor for OpenGLIndexBuffer.  
   /// Deletes the index buffer and releases its resources.  
   OpenGLIndexBuffer::~OpenGLIndexBuffer()  
   {  
       glDeleteBuffers(1, &m_RendererID);  
   }  
}