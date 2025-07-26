#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "Render/VertexArray.h"
#include "Render/Shader.h"
#include "Math/Transform.h"
#include <string>
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

		virtual void Submit(VertexArray* vertexArray, const glm::mat4& transform = glm::mat4(1.0f), const glm::vec4& color = glm::vec4(1, 1, 1, 1)) = 0;

		virtual void SubmitCircle(VertexArray* vertexArray, float radius, Transform transform) = 0;

		virtual void SubmitDebug(VertexArray* vertexArray, const glm::mat4& transform = glm::mat4(1.0f)) = 0;

		virtual void SubmitText(const string& text, const Transform& transform,
			const glm::vec3& color = { 1,1,1 }, const glm::vec2& pivot = { 0,0 }) = 0;


		virtual void UpdateCamera(const glm::mat4& cameraTransform) = 0;
	};
}