#include "acpch.h"
#include "OpenGLRenderer.h"
#include "OpenGLShader.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

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

	void OpenGLRenderer::Submit(Shader* shader, VertexArray* vertexArray, const glm::mat4& transform)
	{

		glm::mat4 projection = glm::perspective(
			glm::radians(45.0f),  // Field of View (FOV) angle
			16.0f / 9.0f,      // Aspect ratio (width/height)
			0.1f,                 // Near clipping plane (objects closer than this are clipped)
			100.0f                // Far clipping plane (objects farther than this are clipped)
		);
		projection = glm::ortho(
			0.0f, 1280.0f,    // Left, Right
			0.0f, 720.0f   // Bottom, Top (flipped Y-axis)
		);

		// Bind the shader program
		shader->Bind();

		// Upload the transformation matrix and view-projection matrix to the shader
		shader->SetMat4("u_ViewProjection", s_SceneData.ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);
		shader->SetMat4("projection", projection);

		// Bind the vertex array
		vertexArray->Bind();

		// Issue the draw call
		uint32_t cnt = vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, cnt, GL_UNSIGNED_INT, 0);
	}

	void OpenGLRenderer::UpdateCamera(const glm::mat4& cameraTransform)
	{
		s_SceneData.ViewProjectionMatrix = cameraTransform;
	}
}