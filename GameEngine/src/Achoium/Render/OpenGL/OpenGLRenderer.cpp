#include "acpch.h"
#include "OpenGLRenderer.h"
#include "OpenGLShader.h"
#include <glad/glad.h>

namespace ac
{
	void OpenGLRenderer::Init()
	{
		// Enable depth testing for proper rendering of 3D objects
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRenderer::Shutdown()
	{
		// Cleanup logic if needed (e.g., releasing OpenGL resources)
	}

	void OpenGLRenderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		// Update the OpenGL viewport to match the new window dimensions
		glViewport(0, 0, width, height);
	}

	void OpenGLRenderer::BeginScene()
	{
		// Prepare the scene data (e.g., set up view-projection matrix)
		// This would typically be set by the camera system
		// Example:
		// s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void OpenGLRenderer::EndScene()
	{
		// Finalize the scene rendering (if needed)
		// Typically, this might involve swapping buffers or clearing temporary data
	}

	void OpenGLRenderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		// Bind the shader program
		shader->Bind();

		// Upload the transformation matrix and view-projection matrix to the shader
		shader->SetMat4("u_ViewProjection", s_SceneData.ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		// Bind the vertex array
		vertexArray->Bind();

		// Issue the draw call
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderer::UpdateCamera(const glm::mat4& cameraTransform)
	{
		s_SceneData.ViewProjectionMatrix = cameraTransform;
	}
}