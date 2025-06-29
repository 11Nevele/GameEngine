#pragma once

#include "Render/Renderer.h"
namespace ac
{
	/**
	 * @brief OpenGL implementation of the renderer.
	 * 
	 * This class provides the OpenGL-specific implementation of the abstract Renderer interface.
	 * It handles scene rendering operations including initialization, draw calls, and camera updates.
	 */
	class OpenGLRenderer : public Renderer
	{
	public:

		OpenGLRenderer();

		/**
		 * @brief Initializes the OpenGL renderer.
		 * 
		 * Sets up the OpenGL rendering context and configures initial state.
		 */
		void Init() override;
		
		/**
		 * @brief Shuts down the OpenGL renderer.
		 * 
		 * Cleans up resources and releases the OpenGL context.
		 */
		void Shutdown() override;
		
		/**
		 * @brief Handles window resize events.
		 * 
		 * Adjusts the viewport and internal render targets when the window is resized.
		 * 
		 * @param width The new width of the window
		 * @param height The new height of the window
		 */
		void OnWindowResize(uint32_t width, uint32_t height) override;

		/**
		 * @brief Begins a new render scene.
		 * 
		 * Prepares the renderer for a new frame of rendering, setting up any necessary state.
		 */
		void BeginScene() override;
		
		/**
		 * @brief Ends the current render scene.
		 * 
		 * Finalizes rendering for the current frame, potentially flushing commands to the GPU.
		 */
		void EndScene() override;

		/**
		 * @brief Submits a draw command to the renderer.
		 * 
		 * Queues a mesh for rendering with the specified shader and transformation.
		 * 
		 * @param shader The shader program to use for rendering
		 * @param vertexArray The vertex array containing the geometry to render
		 * @param transform The model transformation matrix to apply
		 */
		void Submit(VertexArray* vertexArray, const glm::mat4& transform = glm::mat4(1.0f)) override;

		void SubmitDebug(VertexArray* vertexArray, const glm::mat4& transform = glm::mat4(1.0f)) override;

		void SubmitCircle(VertexArray* vertexArray, float radius, Transform transform) override;
		/**
		 * @brief Updates the camera view for rendering.
		 * 
		 * Sets the camera transformation matrix to be used for scene rendering.
		 * 
		 * @param cameraTransform The camera's view-projection matrix
		 */
		void UpdateCamera(const glm::mat4& cameraTransform) override;

	private:
		/**
		 * @brief Data structure for scene rendering information.
		 * 
		 * Stores the current view-projection matrix for the scene.
		 */
		Shader* shader2D;
		Shader* shaderDebug;

		Shader* circleShader;

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix; ///< Combined view and projection matrix
		};

		SceneData s_SceneData; ///< Current scene rendering data
	};
}