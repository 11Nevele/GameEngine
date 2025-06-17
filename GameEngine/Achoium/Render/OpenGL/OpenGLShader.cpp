#include <acpch.h>
#include "Debug.h"
#include "OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

namespace ac
{
    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
        : m_Name(name)
    {
        GLint sucess = 1;

        vertexSource = vertexSrc;
        vertexID = glCreateShader(GL_VERTEX_SHADER);
        const char* const v = (vertexSource.c_str());
        glShaderSource(vertexID, 1, &v, NULL);
        glCompileShader(vertexID);
        glGetShaderiv(vertexID, GL_COMPILE_STATUS, &sucess);
        if (!sucess)
        {
            char* info = new char[512];
            glGetShaderInfoLog(vertexID, 512, NULL, info);
            ACERR("Error at shader vertex shader: " << name << ": " << info);
        }

        fragementSource = fragmentSrc;
        fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
        const char* const v2 = (fragementSource.c_str());
        glShaderSource(fragmentID, 1, &v2, NULL);
        glCompileShader(fragmentID);
        glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &sucess);
        if (!sucess)
        {
            char* info = new char[512];
            glGetShaderInfoLog(fragmentID, 512, NULL, info);
            ACERR("Error at shader fragment shader: " << name << ": " <<  info);
        }

        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, vertexID);
        glAttachShader(m_RendererID, fragmentID);
        glLinkProgram(m_RendererID);
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &sucess);
        if (!sucess)
        {
            char* info = new char[512];
            glGetProgramInfoLog(fragmentID, 512, NULL, info);
            ACERR(info);
        }

        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);
    }


    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1iv(location, count, values);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

}