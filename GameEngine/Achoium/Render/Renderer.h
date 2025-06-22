#pragma once
#include <glm/glm.hpp>
#include "Render/VertexArray.h"
#include "Render/Shader.h"
namespace ac
{
	class Renderer
	{
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void OnWindowResize(uint32_t width, uint32_t height) = 0;

		virtual void BeginScene() = 0;
		virtual void EndScene() = 0;

		virtual void Submit(VertexArray* vertexArray, const glm::mat4& transform = glm::mat4(1.0f)) = 0;

		virtual void SubmitDebug(VertexArray* vertexArray, const glm::mat4& transform = glm::mat4(1.0f)) = 0;


		virtual void UpdateCamera(const glm::mat4& cameraTransform) = 0;
	};
}