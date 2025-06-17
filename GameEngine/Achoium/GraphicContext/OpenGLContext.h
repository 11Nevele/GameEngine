#pragma once
#include "acpch.h"
#include "GraphicContext/GraphicContext.h"
namespace ac
{
	/**
	 * @brief OpenGL implementation of the graphics context.
	 * 
	 * This class implements the GraphicContext interface using OpenGL
	 * and GLFW for window management and rendering context creation.
	 */
	class OpenGLContext : public GraphicContext
	{
	public:
		/**
		 * @brief Constructs an OpenGL context for a specified GLFW window.
		 * 
		 * @param window Pointer to the GLFW window to associate with this context
		 */
		OpenGLContext(GLFWwindow* window);
		
		/**
		 * @brief Initializes the OpenGL context.
		 * 
		 * Sets up the OpenGL context for the associated GLFW window,
		 * enabling features and setting initial state for rendering.
		 */
		virtual void Init() override;
		
		/**
		 * @brief Swaps the front and back buffers.
		 * 
		 * Presents the rendered content to the screen by swapping
		 * the back buffer with the front buffer using GLFW.
		 */
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* mWindow; ///< Pointer to the associated GLFW window
	};
}