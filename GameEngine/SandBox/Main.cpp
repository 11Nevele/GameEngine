#include "acpch.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glfw3.h>
#include "Achoium.h"

#include "UnitTests/TestUnit.h"
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

using mTexture2D = OpenGLTexture2D;
using mInput = WindowsInput;
using mWindow = WinWindow;
using mRenderer = OpenGLRenderer;

unsigned int vertexArray, vertexBuffer, indexBuffer;
float verteies[] = {
	// Front face
	 0.5,  0.5,  0.5, 1.0, 1.0,  // 0
	 0.5, -0.5,  0.5, 1.0, 0.0, // 1
	-0.5, -0.5,  0.5, 0.0, 0.0, // 2
	-0.5,  0.5,  0.5, 0.0, 1.0, // 3

	// Back face
	0.5,  0.5, -0.5, 1.0, 1.0, // 4
	0.5, -0.5, -0.5, 1.0, 0.0, // 5
	-0.5, -0.5, -0.5, 0.0, 0.0, // 6
	-0.5,  0.5, -0.5, 0.0, 1.0  // 7
};

float fullcube[] = {
	// Positions          // Texture Coords
	// Front face (Z = 0.5)
	-0.5f, -0.5f,  0.5f,  0.0f, 2.0f/3.0f,  // Bottom-left
	 0.5f, -0.5f,  0.5f,  0.25f, 2.0f / 3.0f,  // Bottom-right
	 0.5f,  0.5f,  0.5f,  0.25f, 1.0f,  // Top-right
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // Top-left

	// Back face (Z = -0.5)
	-0.5f, -0.5f, -0.5f,  0.25f, 2.0f / 3.0f,  // Bottom-right (mirrored)
	 0.5f, -0.5f, -0.5f,  0.0f, 2.0f / 3.0f,  // Bottom-left (mirrored)
	 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // Top-left (mirrored)
	-0.5f,  0.5f, -0.5f,  0.25f, 1.0f,  // Top-right (mirrored)

	// Left face (X = -0.5)
	-0.5f, -0.5f, -0.5f,  0.0f, 2.0f / 3.0f,  // Bottom-back
	-0.5f, -0.5f,  0.5f, 0.25f, 2.0f / 3.0f,  // Bottom-front
	-0.5f,  0.5f,  0.5f,  0.25f, 1.0f,  // Top-front
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // Top-back

	// Right face (X = 0.5)
	 0.5f, -0.5f,  0.5f,   0.0f, 2.0f / 3.0f,  // Bottom-front
	 0.5f, -0.5f, -0.5f,  0.25f, 2.0f / 3.0f,  // Bottom-back
	 0.5f,  0.5f, -0.5f,  0.25f, 1.0f,  // Top-back
	 0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // Top-front

	 // Bottom face (Y = -0.5)
	 -0.5f, -0.5f, -0.5f,  0.0f, 2.0 / 3.0f,  // Back-left
	  0.5f, -0.5f, -0.5f,  0.25f, 2.0 / 3.0f,  // Back-right
	  0.5f, -0.5f,  0.5f,  0.25f, 1.0 / 3.0f,  // Front-right
	 -0.5f, -0.5f,  0.5f,  0.0f, 1.0/3.0f,  // Front-left

	 // Top face (Y = 0.5)
	 -0.5f,  0.5f,  0.5f,  0.5f, 1.0/3.0f,  // Front-left
	  0.5f,  0.5f,  0.5f,  0.75f, 1.0/3.0f,  // Front-right
	  0.5f,  0.5f, -0.5f,  0.75f, 2.0 / 3.0f,  // Back-right
	 -0.5f,  0.5f, -0.5f,  0.5f, 2.0 / 3.0f   // Back-left
};

uint32_t index[] = {
	// Front face
	0, 1, 2,
	2, 3, 0,
	
	// Back face
	4, 6, 5,
	6, 4, 7,

	// Left face
	3, 2, 6,
	6, 7, 3,

	// Right face
	0, 5, 1,
	5, 0, 4,

	// Top face
	0, 3, 7,
	7, 4, 0,

	// Bottom face
	1, 6, 2,
	6, 1, 5
	
};

unsigned int fullcubeindices[] = {
	// Front face
	0, 1, 2,   // Triangle 1
	2, 3, 0,   // Triangle 2
	// Back face
	4, 6, 5,   // Triangle 1
	6, 4, 7,   // Triangle 2
	// Left face
	8, 9, 10,  // Triangle 1
	10, 11, 8, // Triangle 2
	// Right face
	12, 13, 14, // Triangle 1
	14, 15, 12, // Triangle 2
	// Bottom face
	16, 17, 18, // Triangle 1
	18, 19, 16, // Triangle 2
	// Top face
	20, 21, 22, // Triangle 1
	22, 23, 20  // Triangle 2
};

struct Camera {
	glm::vec3 movementSpeed;
};
struct movement
{
	glm::vec3 velocity;
};

void CameraMovement(World& world)
{
	mInput& input = world.GetResourse<mInput>();
	mWindow& win = world.GetResourse<mWindow>();
	mRenderer& rend = world.GetResourse<mRenderer>();
	auto t = world.View<Camera, Transform>().GetPacked();
	for (auto i : t)
	{
		auto& transform = get<1>(i.components);
		auto& camera = get < 0> (i.components);
		if (input.IsKeyPressed(AC_KEY_A, win))
			transform.position.x -= camera.movementSpeed.x;

		if (input.IsKeyPressed(AC_KEY_D, win))
			transform.position.x += camera.movementSpeed.x;

		if (input.IsKeyPressed(AC_KEY_W, win))
			transform.position.z -= camera.movementSpeed.z;

		if (input.IsKeyPressed(AC_KEY_S, win))
			transform.position.z += camera.movementSpeed.z;

		if (input.IsKeyPressed(AC_KEY_LEFT_SHIFT, win))
			transform.position.y += camera.movementSpeed.y;

		if (input.IsKeyPressed(AC_KEY_LEFT_CONTROL, win))
			transform.position.y -= camera.movementSpeed.y;
		
		rend.UpdateCamera(transform.asMat4(true));
	}
}
void PhyscisMovement(World& world)
{
	world.View<movement, Transform>().ForEach([](Entity e, movement& mov, Transform& t)
		{
			t.position += mov.velocity;
			
		});
}
OpenGLShader* shader;
void RenderSprite(World& world)
{
	mRenderer& renderer = world.GetResourse<mRenderer>();
	TextureManager& textureManager = world.GetResourse<TextureManager>();
	ModelManager& modelManager = world.GetResourse<ModelManager>();
	
	shader->SetInt("u_Texture", 0);
	world.View<Sprite, Transform>().ForEach([&modelManager, &textureManager, &renderer](Entity e, Sprite& sprite, Transform& trans)
		{
			textureManager.GetTexture(sprite.textureID).Bind();
			OpenGLVertexArray& vao = modelManager.GetModel(0);
			Transform t = trans;
			t.scale.x *= sprite.width;
			t.scale.y *= sprite.height;
			renderer.Submit(shader, &(vao), t.asMat4());
		});

}

int main()
{
	TestAllTimeMethods();
	
	ac::World world;
	world.RegisterType<Camera>();
	world.RegisterType<Transform>();
	world.AddResource<ac::EventManager>(new EventManager());
	world.AddResource<mWindow>(new mWindow({"AC", 1280, 720}, world.GetResourse<EventManager>()));
	world.AddResource<mRenderer>(new mRenderer());
	world.AddResource<mInput>(new mInput());
	world.AddResource<TextureManager>(new TextureManager());
	world.AddResource<ModelManager>(new ModelManager());
	
	world.GetResourse<TextureManager>()
		.AddTexture("Default","C:/C++Projet/GameEngine/GameEngine/SandBox/Image/null.png")
		.AddTexture("Grass", "C:/C++Projet/GameEngine/GameEngine/SandBox/Image/grass.png")
		.AddTexture("Red", "C:/C++Projet/GameEngine/GameEngine/SandBox/Image/red.jpg");

	Entity c = world.CreateEntity();
	Entity sp = world.CreateEntity();
	world.Add<Camera>(c, {{10, 10, 10}});
	world.Add<Transform>(c, Transform());

	
	world.Add<movement>(sp, { {10,0,0} });
	Transform& tr = world.Add<Transform>(sp, Transform());
	tr.scale = { 1,1, 1 };
	Sprite &sprite = world.Add<Sprite>(sp, Sprite::Create("Default", world.GetResourse<TextureManager>()));
	
	shader = new OpenGLShader("name",
		util::ReadFile("C:/C++Projet/GameEngine/GameEngine/SandBox/Shader/2DVertexShader.txt"),
		util::ReadFile("C:/C++Projet/GameEngine/GameEngine/SandBox/Shader/2DFragmentShader.txt"));

	
	while (true)
	{
		mWindow& win = world.GetResourse<mWindow>();

		CameraMovement(world);
		PhyscisMovement(world);
		RenderSprite(world);

		win.OnUpdate();
		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
}