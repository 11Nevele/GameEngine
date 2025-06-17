#pragma once

#include "Render/Renderer.h"
namespace ac
{
	class OpenGLRenderer : public Renderer
	{
	public:
		void Init() override;
		void Shutdown() override;
		void OnWindowResize(uint32_t width, uint32_t height) override;

		void BeginScene() override;
		void EndScene() override;

		void Submit(Shader* shader, VertexArray* vertexArray, const glm::mat4& transform = glm::mat4(1.0f)) override;

		void UpdateCamera(const glm::mat4& cameraTransform) override;

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		SceneData s_SceneData;
	};
}