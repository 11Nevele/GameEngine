#include "acpch.h"  
#include "OpenGLRenderer.h"  
#include "OpenGLShader.h"  
#include <glad/glad.h>  
#include <glm/gtc/matrix_transform.hpp>  
#include "Util/util.h"
namespace ac  
{
	OpenGLRenderer::OpenGLRenderer(): s_SceneData()
	{
		shader2D = new OpenGLShader("name",
			util::ReadFile("C:/C++Projet/GameEngine/GameEngine/SandBox/Shader/2DVertexShader.txt"),
			util::ReadFile("C:/C++Projet/GameEngine/GameEngine/SandBox/Shader/2DFragmentShader.txt"));
		shaderDebug = new OpenGLShader("name",
			util::ReadFile("C:/C++Projet/GameEngine/GameEngine/SandBox/Shader/DebugVertexShader.txt"),
			util::ReadFile("C:/C++Projet/GameEngine/GameEngine/SandBox/Shader/DebugFragmentShader.txt"));
		circleShader = new OpenGLShader("circleShader",
			util::ReadFile("C:/C++Projet/GameEngine/GameEngine/SandBox/Shader/CircleShaderVertex.glsl"),
			util::ReadFile("C:/C++Projet/GameEngine/GameEngine/SandBox/Shader/CircleShaderFragment.glsl"));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Standard alpha blending
	}
	/// Initializes the OpenGL renderer.  
/// Enables depth testing for proper rendering of 3D objects.  
void OpenGLRenderer::Init()  
{  
	glEnable(GL_DEPTH_TEST);  
}  

/// Shuts down the OpenGL renderer.  
/// Performs cleanup logic if needed, such as releasing OpenGL resources.  
void OpenGLRenderer::Shutdown()  
{  
	// Cleanup logic if needed (e.g., releasing OpenGL resources)  
}  

/// Updates the OpenGL viewport to match the new window dimensions.  
/// @param width The new width of the window.  
/// @param height The new height of the window.  
void OpenGLRenderer::OnWindowResize(uint32_t width, uint32_t height)  
{  
	glViewport(0, 0, width, height);  
}  

/// Prepares the scene for rendering.  
/// Typically sets up view-projection matrices or other scene data.  
void OpenGLRenderer::BeginScene()  
{  
	// Prepare the scene data (e.g., set up view-projection matrix)  
	// This would typically be set by the camera system  
	// Example:  
	// s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();  
}  

/// Finalizes the scene rendering.  
/// Typically involves swapping buffers or clearing temporary data.  
void OpenGLRenderer::EndScene()  
{  
	// Finalize the scene rendering (if needed)  
	// Typically, this might involve swapping buffers or clearing temporary data  
}  

/// Submits a draw call to the renderer.  
/// @param shader The shader program to use for rendering.  
/// @param vertexArray The vertex array containing the geometry data.  
/// @param transform The transformation matrix for the object being rendered.  
void OpenGLRenderer::Submit(VertexArray* vertexArray, const glm::mat4& transform)  
{  
	glm::mat4 projection = glm::perspective(  
		glm::radians(45.0f),  // Field of View (FOV) angle  
		16.0f / 9.0f,         // Aspect ratio (width/height)  
		0.1f,                 // Near clipping plane  
		100.0f                // Far clipping plane  
	);  
	projection = glm::ortho(  
		0.0f, 1280.0f,        // Left, Right  
		0.0f, 720.0f          // Bottom, Top  
	);  

	// Bind the shader program  
	shader2D->Bind();  

	// Upload the transformation matrix and view-projection matrix to the shader  
	shader2D->SetMat4("u_ViewProjection", s_SceneData.ViewProjectionMatrix);
	shader2D->SetMat4("u_Transform", transform);
	shader2D->SetMat4("projection", projection);

	// Bind the vertex array  
	vertexArray->Bind();  

	// Issue the draw call  
	uint32_t cnt = vertexArray->GetIndexBuffer()->GetCount();  
	glDrawElements(GL_TRIANGLES, cnt, GL_UNSIGNED_INT, 0);  
}  

void OpenGLRenderer::SubmitDebug(VertexArray* vertexArray, const glm::mat4& transform)
{
	glm::mat4 projection = glm::ortho(
		0.0f, 1280.0f,        // Left, Right  
		0.0f, 720.0f          // Bottom, Top  
	);

	// Bind the shader program  
	shaderDebug->Bind();

	// Upload the transformation matrix and view-projection matrix to the shader  
	shaderDebug->SetMat4("u_ViewProjection", s_SceneData.ViewProjectionMatrix);
	shaderDebug->SetMat4("u_Transform", transform);
	shaderDebug->SetMat4("projection", projection);

	// Bind the vertex array  
	vertexArray->Bind();

	// Issue the draw call  
	uint32_t cnt = vertexArray->GetIndexBuffer()->GetCount();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, cnt, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OpenGLRenderer::SubmitCircle(VertexArray* vertexArray, float radius, Transform transform)
{
	glm::mat4 projection = glm::ortho(
		0.0f, 1280.0f,        // Left, Right  
		0.0f, 720.0f          // Bottom, Top  
	);

	radius *= transform.scale.x;
	transform.scale = { radius * 2, radius * 2, radius * 2 };

	glm::mat4 transMat = transform.asMat4();

	// Bind the shader program  
	circleShader->Bind();

	// Upload the transformation matrix and view-projection matrix to the shader  
	circleShader->SetMat4("u_ViewProjection", s_SceneData.ViewProjectionMatrix);
	circleShader->SetMat4("u_Transform", transMat * glm::translate(glm::mat4(1), glm::vec3(-0.5, -0.5, 0)));
	circleShader->SetMat4("projection", projection);

	glm::vec4 center = s_SceneData.ViewProjectionMatrix * glm::vec4(transform.position,1);
	
	circleShader->SetFloat2("u_Center", center);
	circleShader->SetFloat4("u_Color", glm::vec4(1, 0, 0, 1));
	circleShader->SetFloat("u_Radius", radius);
	circleShader->SetFloat("u_Thickness", 1);
	circleShader->SetFloat("u_rotation", transform.getRotationAsDegrees().z);

	vertexArray->Bind();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

/// Updates the camera's view-projection matrix.  
/// @param cameraTransform The transformation matrix representing the camera's view and projection.  
void OpenGLRenderer::UpdateCamera(const glm::mat4& cameraTransform)  
{  
	s_SceneData.ViewProjectionMatrix = cameraTransform;  
}  
}