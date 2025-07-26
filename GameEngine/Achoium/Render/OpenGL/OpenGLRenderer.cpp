#include "acpch.h"  
#include "OpenGLRenderer.h"  
#include "OpenGLShader.h"  
#include <glad/glad.h>  
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>  
#include <filesystem>
#include "Util/util.h"
namespace ac  
{
	OpenGLRenderer::OpenGLRenderer(): s_SceneData()
	{
		std::string currentPath = filesystem::current_path().string();
		std::cout << "Current Path: " << currentPath << std::endl;
		shader2D = new OpenGLShader("name",
			util::ReadFile(currentPath + "/SandBox/Shader/2DVertexShader.txt"),
			util::ReadFile(currentPath + "/SandBox/Shader/2DFragmentShader.txt"));
		shaderDebug = new OpenGLShader("name",
			util::ReadFile(currentPath + "/SandBox/Shader/DebugVertexShader.txt"),
			util::ReadFile(currentPath + "/SandBox/Shader/DebugFragmentShader.txt"));
		circleShader = new OpenGLShader("circleShader",
			util::ReadFile(currentPath + "/SandBox/Shader/CircleShaderVertex.glsl"),
			util::ReadFile(currentPath + "/SandBox/Shader/CircleShaderFragment.glsl"));
		textShader = new OpenGLShader("textShader",
			util::ReadFile(currentPath + "/SandBox/Shader/TextVertexShader.glsl"),
			util::ReadFile(currentPath + "/SandBox/Shader/TextFragmentShader.glsl"));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Standard alpha blending
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用byte-alignment限制
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        // 检查文件是否存在
        if (!std::filesystem::exists((currentPath + "/Assets/Fonts/arial.ttf")))
            std::cout << "ERROR: Font file does not exist!" << std::endl;
        FT_Face face;
        if (FT_New_Face(ft, (currentPath + "/Assets/Fonts/arial.ttf").c_str(), 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            std::cout << "Attempted to load: " << (currentPath + "/Assets/Fonts/arial.ttf") << std::endl;
            
        }

        // 设置字体大小 - 宽度为0表示动态计算宽度
        FT_Set_Pixel_Sizes(face, 0, 48);  // 48像素高

        for (GLubyte c = 0; c < 128; c++)
        {
            // 加载字符的字形 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // 生成字形纹理
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // 设置纹理选项
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // 将字符存储到字符表中备用
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            Characters.insert(std::pair<GLchar, Character>(c, character));
        }
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
void OpenGLRenderer::Submit(VertexArray* vertexArray, const glm::mat4& transform, const glm::vec4& color)
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
	shader2D->SetFloat4("uColor", color);

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

void OpenGLRenderer::SubmitText(const string& text, const Transform& transform, const glm::vec3& color, const glm::vec2& pivot)
{
    glm::mat4 projection = glm::ortho(
        0.0f, (float)1280,        // Left, Right  
        0.0f, (float)720          // Bottom, Top  
    );

    // Activate the text shader
    textShader->Bind();

    // Set the projection matrix in the shader
    textShader->SetMat4("projection", projection);
	textShader->SetMat4("u_ViewProjection", s_SceneData.ViewProjectionMatrix);

    // Set text color (default to white if not specified)
    textShader->SetFloat3("textColor", color);

    // Create and configure VAO/VBO for rendering characters
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Each character is represented by 6 vertices (2 triangles forming a quad)
    // Each vertex has 4 components (x, y positions and texture coordinates)
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    // Set texture unit for the sampler
    glActiveTexture(GL_TEXTURE0);
    textShader->SetInt("text", 0);


    // Starting position for text rendering based on the transform


    float totalWidth = 0.0f, mxHeight = 0;
    for (char c : text)
    {
        Character ch = Characters[c];
        totalWidth += (ch.Advance >> 6); // Advance is in 1/64 pixels
        mxHeight = std::max(mxHeight, (float)ch.Size.y);
    }
	totalWidth *= transform.scale.x; // Scale the total width by the transform's scale
	mxHeight *= transform.scale.y; // Scale the max height by the transform's scale

    float x = transform.position.x - totalWidth * pivot.x;
    float y = transform.position.y - mxHeight * pivot.y;
    // Render each character in the text string
    for (char c : text)
    {

        Character ch = Characters[c];

        GLfloat xpos = x + ch.Bearing.x * transform.scale.x;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * transform.scale.y;

        GLfloat w = ch.Size.x * transform.scale.x;
        GLfloat h = ch.Size.y * transform.scale.y;
        // 当前字符的VBO
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // 在方块上绘制字形纹理
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // 更新当前字符的VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // 绘制方块
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        x += (ch.Advance >> 6) * transform.scale.x; //(2^6 = 64)
    }

    // Clean up
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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