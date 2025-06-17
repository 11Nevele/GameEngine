#pragma once
#include "Render/Shader.h"
#include <glad/glad.h>
#include <unordered_map>

namespace ac
{
	/**
	 * @brief OpenGL implementation of the shader program.
	 * 
	 * This class provides an OpenGL-specific implementation of the abstract Shader interface.
	 * It handles compilation, linking, and usage of GLSL shader programs, as well as
	 * setting uniform values for use in rendering.
	 */
	class OpenGLShader : public Shader
	{
	public:
		/**
		 * @brief Constructs a shader program from vertex and fragment source code.
		 * 
		 * @param name The name identifier for this shader
		 * @param vertexSrc GLSL source code for the vertex shader stage
		 * @param fragmentSrc GLSL source code for the fragment shader stage
		 */
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		
		/**
		 * @brief Destructor. Cleans up OpenGL shader resources.
		 */
		virtual ~OpenGLShader();

		/**
		 * @brief Activates this shader program for use.
		 */
		virtual void Bind() const override;
		
		/**
		 * @brief Deactivates this shader program.
		 */
		virtual void Unbind() const override;

		/**
		 * @brief Sets an integer uniform value.
		 * 
		 * @param name The name of the uniform in the shader
		 * @param value The integer value to set
		 */
		virtual void SetInt(const std::string& name, int value) override;
		
		/**
		 * @brief Sets an array of integer uniform values.
		 * 
		 * @param name The name of the uniform in the shader
		 * @param values Pointer to the array of integers
		 * @param count Number of elements in the array
		 */
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		
		/**
		 * @brief Sets a float uniform value.
		 * 
		 * @param name The name of the uniform in the shader
		 * @param value The float value to set
		 */
		virtual void SetFloat(const std::string& name, float value) override;
		
		/**
		 * @brief Sets a 2D vector uniform value.
		 * 
		 * @param name The name of the uniform in the shader
		 * @param value The vec2 value to set
		 */
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		
		/**
		 * @brief Sets a 3D vector uniform value.
		 * 
		 * @param name The name of the uniform in the shader
		 * @param value The vec3 value to set
		 */
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		
		/**
		 * @brief Sets a 4D vector uniform value.
		 * 
		 * @param name The name of the uniform in the shader
		 * @param value The vec4 value to set
		 */
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		
		/**
		 * @brief Sets a 3x3 matrix uniform value.
		 * 
		 * @param name The name of the uniform in the shader
		 * @param value The mat3 value to set
		 */
		virtual void SetMat3(const std::string& name, const glm::mat3& value) override;
		
		/**
		 * @brief Sets a 4x4 matrix uniform value.
		 * 
		 * @param name The name of the uniform in the shader
		 * @param value The mat4 value to set
		 */
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		/**
		 * @brief Gets the name of this shader program.
		 * 
		 * @return const std::string& The shader's name
		 */
		virtual const std::string& GetName() override { return m_Name; }
	private:
		uint32_t m_RendererID;          ///< OpenGL handle to the shader program
		std::string m_Name;             ///< Name identifier for this shader
		std::string vertexSource;       ///< Source code for the vertex shader
		std::string fragementSource;    ///< Source code for the fragment shader
		std::uint32_t vertexID;         ///< OpenGL handle to the compiled vertex shader
		std::uint32_t fragmentID;       ///< OpenGL handle to the compiled fragment shader
	};
}