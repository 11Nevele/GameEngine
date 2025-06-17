#pragma once

namespace ac
{
	/**
	 * @brief Abstract base class for graphics rendering contexts.
	 * 
	 * The GraphicContext class provides an interface for platform-specific
	 * rendering context implementations. It handles the initialization of 
	 * graphics APIs and buffer swapping operations.
	 */
	class GraphicContext
	{
	public:
		/**
		 * @brief Initializes the graphics context.
		 * 
		 * This method must be implemented by derived classes to perform
		 * any necessary setup for the specific graphics API being used.
		 */
		virtual void Init() = 0;
		
		/**
		 * @brief Swaps the front and back buffers.
		 * 
		 * This method must be implemented by derived classes to display
		 * the rendered content to the screen by swapping the back buffer
		 * (where rendering occurs) with the front buffer (what is displayed).
		 */
		virtual void SwapBuffers() = 0;
		
		/**
		 * @brief Virtual destructor to ensure proper cleanup.
		 * 
		 * Ensures that derived class destructors are called when deleting
		 * through a base class pointer.
		 */
		virtual ~GraphicContext() {}
	};
}