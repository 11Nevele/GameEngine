#include "acpch.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glfw3.h>
#include "Achoium.h"

#include "TestUnit.h"
#include "Benchmark.h"
using namespace ac;

EventManager eventManager;
class Test
{
public:
	int data;
	bool OnEvent(const int& i)
	{
		std::cout << i + data << std::endl;
		return true;
	}
};
void Add()
{
	Test test;
	test.data = 42;
	eventManager.AddListener<int>(std::bind(&Test::OnEvent, &test, std::placeholders::_1));
}
using namespace std;
using namespace ac;

unsigned int vertexArray, vertexBuffer, indexBuffer;
float verteies[] = {
	// Front face
	 0.5,  0.5,  0.5,  // 0
	 0.5, -0.5,  0.5,  // 1
	-0.5, -0.5,  0.5,  // 2
	-0.5,  0.5,  0.5,  // 3

	// Back face
	 0.5,  0.5, -0.5,  // 4
	 0.5, -0.5, -0.5,  // 5
	-0.5, -0.5, -0.5,  // 6
	-0.5,  0.5, -0.5   // 7
};

uint32_t index[] = {
	// Front face
	0, 1, 2,
	2, 3, 0,

	// Back face
	4, 5, 6,
	6, 7, 4,

	// Left face
	3, 2, 6,
	6, 7, 3,

	// Right face
	0, 1, 5,
	5, 4, 0,

	// Top face
	0, 3, 7,
	7, 4, 0,

	// Bottom face
	1, 2, 6,
	6, 5, 1
};
int main()
{
	//fStartTest();
	ac::World world;
	world.AddResource<ac::EventManager>(new EventManager());
	world.AddResource<Window>(Window::Create({"AC", 1280, 720}, world.GetResourse<EventManager>()));
	world.AddResource<ac::Renderer>(new OpenGLRenderer());

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	OpenGLShader shader("name",
		util::ReadFile("C:/C++Projet/GameEngine/GameEngine/SandBox/Shader/ShaderTest.txt"),
		util::ReadFile("C:/C++Projet/GameEngine/GameEngine/SandBox/Shader/fragmentShader.txt"));

	

	OpenGLVertexArray vao;
	OpenGLIndexBuffer ebo(index, sizeof(index) / sizeof(int32_t));
	OpenGLVertexBuffer vbo(verteies, sizeof(verteies));
	BufferLayout layout({ BufferElement(ShaderDataType::Float3, "coordinate") });
	vbo.SetLayout(layout);
	vao.AddVertexBuffer(util::Ref(vbo));
	vao.SetIndexBuffer(util::Ref(ebo));

	Transform transform;
	transform.position = { 0,1 ,3 };

	while (true)
	{

		Window& win = world.GetResourse<Window>();
		Renderer& renderer = world.GetResourse<Renderer>();
		renderer.UpdateCamera(glm::mat4(1.0));
		renderer.Submit(util::Ref(shader), util::Ref(vao), transform.asMat4());

		transform.RotateY(1);

		win.OnUpdate();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
}