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
int main()
{
	StartTest();
	ac::World world;
	world.AddResource<ac::EventManager>(new EventManager());
	world.AddResource<Window>(Window::Create({"AC", 1280, 720}, world.GetResourse<EventManager>()));
	while (true)
	{
		Window& win = world.GetResourse<Window>();
		glClearColor(0, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		win.OnUpdate();
	}
	
}