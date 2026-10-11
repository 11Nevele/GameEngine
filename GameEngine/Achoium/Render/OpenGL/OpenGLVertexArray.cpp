#include <acpch.h>  
#include "Render/OpenGL/OpenGLVertexArray.h"  

namespace ac  
{  
   /// Constructor for OpenGLVertexArray.  
   /// Creates a new OpenGL vertex array object.  
   OpenGLVertexArray::OpenGLVertexArray()  
   {  
       glCreateVertexArrays(1, &m_RendererID);  
   }

   OpenGLVertexArray::OpenGLVertexArray(OpenGLVertexArray&& other) noexcept:
       m_RendererID(other.m_RendererID),
	   m_VertexBufferIndex(other.m_VertexBufferIndex),
	   vertexBuffers(std::move(other.vertexBuffers)),
	   indexBuffers(std::move(other.indexBuffers))
   {
	   other.m_RendererID = 0;
	   other.m_VertexBufferIndex = 0;
	   other.vertexBuffers.clear();
	   other.indexBuffers = nullptr;
   }

   /// Destructor for OpenGLVertexArray.  
   /// Deletes the OpenGL vertex array object.  
   OpenGLVertexArray::~OpenGLVertexArray()  
   {  
       if (m_RendererID != 0)
       {
           ACMSG("VAO: " << m_RendererID << " Deleted");
           glDeleteVertexArrays(1, &m_RendererID);
       }
   }  

   /// Binds the vertex array object for rendering.  
   void OpenGLVertexArray::Bind() const  
   {  
       glBindVertexArray(m_RendererID);  
   }  

   /// Unbinds the vertex array object.  
   void OpenGLVertexArray::Unbind() const  
   {  
       glBindVertexArray(0);  
   }  

   /// Adds a vertex buffer to the vertex array object.  
   /// @param vertexBuffer The vertex buffer to add.  
   void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)  
   {  
       Bind(); // Bind VAO first
       vertexBuffers.push_back(vertexBuffer);  
       vertexBuffer->Bind();
       SetAttrib(vertexBuffer->GetLayout());
   }  

   /// Sets the index buffer for the vertex array object.  
   /// @param indexBuffert The index buffer to set.  
   void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffert)  
   {  
       Bind(); // Bind VAO first
       this->indexBuffers = indexBuffert;  
       indexBuffert->Bind(); // Bind the index buffer
   }  

   /// Retrieves the vertex buffers associated with the vertex array object.  
   /// @return A vector of shared pointers to the vertex buffers.  
   const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const  
   {  
       return vertexBuffers;  
   }  

   /// Retrieves the index buffer associated with the vertex array object.  
   /// @return A shared pointer to the index buffer.  
   const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const  
   {  
       return indexBuffers;  
   }  

   /// Configures vertex attributes based on the provided buffer layout.  
   /// @param layout The buffer layout describing the vertex attributes.  
   void OpenGLVertexArray::SetAttrib(const BufferLayout& layout)  
   {  
       ACASSERT(layout.GetElements().size() > 0, "Layout is empty");  
       for (const BufferElement& element : layout)  
       {  
           switch (element.Type)  
           {  
           case ShaderDataType::Float:  
           case ShaderDataType::Float2:  
           case ShaderDataType::Float3:  
           case ShaderDataType::Float4:  
           {  
               glEnableVertexAttribArray(m_VertexBufferIndex);  
               glVertexAttribPointer(m_VertexBufferIndex,  
                   element.GetComponentCount(),  
                   ShaderDataToGLData(element.Type),  
                   element.Normalized ? GL_TRUE : GL_FALSE,  
                   layout.GetStride(),  
                   (const void*)element.Offset);  
               m_VertexBufferIndex++;  
               break;  
           }  
           case ShaderDataType::Int:  
           case ShaderDataType::Int2:  
           case ShaderDataType::Int3:  
           case ShaderDataType::Int4:  
           case ShaderDataType::Bool:  
           {  
               glEnableVertexAttribArray(m_VertexBufferIndex);  
               glVertexAttribIPointer(m_VertexBufferIndex,  
                   element.GetComponentCount(),  
                   ShaderDataToGLData(element.Type),  
                   layout.GetStride(),  
                   (const void*)element.Offset);  
               m_VertexBufferIndex++;  
               break;  
           }  
           case ShaderDataType::Mat3:  
           case ShaderDataType::Mat4:  
           {  
               uint8_t count = element.GetComponentCount();  
               for (uint8_t i = 0; i < count; i++)  
               {  
                   glEnableVertexAttribArray(m_VertexBufferIndex);  
                   glVertexAttribPointer(m_VertexBufferIndex,  
                       count,  
                       ShaderDataToGLData(element.Type),  
                       element.Normalized ? GL_TRUE : GL_FALSE,  
                       layout.GetStride(),  
                       (const void*)(element.Offset + sizeof(float) * count * i));  
                   glVertexAttribDivisor(m_VertexBufferIndex, 1);  
                   m_VertexBufferIndex++;  
               }  
               break;  
           }  
           default:  
               ACASSERT(false, "Unknown ShaderDataType!");  
           }  
       }  
   }  
}