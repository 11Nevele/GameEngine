#include "acpch.h"
#include "UISystems.h"

void UISystems::UpdateUI(World& world)
{
	OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
	SceneData& sceneData = world.GetResourse<SceneData>();
	//renderer.SubmitText("Mod: " + std::to_string(sceneData.mod), Transform(glm::vec3(10, 10, 0), glm::vec3(1, 1, 1)));
}
