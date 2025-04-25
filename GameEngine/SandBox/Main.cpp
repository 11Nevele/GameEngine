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

unsigned int vertexArray, vertexBuffer, indexBuffer;
int main()
{
	//StartTest();
	ac::World world;
	world.AddResource<ac::EventManager>(new EventManager());
	world.AddResource<Window>(Window::Create({"AC", 1280, 720}, world.GetResourse<EventManager>()));
	while (true)
	{
		Window& win = world.GetResourse<Window>();
		glClearColor(0, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);
		glGenBuffers(1, &vertexBuffer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
		};
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		win.OnUpdate();
	}
	
}