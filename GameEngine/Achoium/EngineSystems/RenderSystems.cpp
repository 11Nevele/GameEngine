#include "acpch.h"
#include "RenderSystems.h"
#include "AssetManagement\AssetManager.h"
#include "Math/Transform.h"
#include "EngineComponents/Physics/Physics.h"
namespace ac
{
	bool OnSpriteAdded(const OnAdded<Sprite>& event)
	{
		World& world = event.world;
		TextureManager& textureManager = world.GetResourse<TextureManager>();
		ModelManager& modelManager = world.GetResourse<ModelManager>();
		textureManager.AddReference(event.component.textureID);
		modelManager.AddReference(0);
		return true;
	}

	bool OnSpriteDelete(const OnDeleted<Sprite>& event)
	{
		World& world = event.world;
		TextureManager& textureManager = world.GetResourse<TextureManager>();
		ModelManager& modelManager = world.GetResourse<ModelManager>();
		textureManager.DeleteReference(event.component.textureID);
		modelManager.DeleteReference(0);
		return true;
	}

	void RenderSprite(World& world)
	{
		OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
		TextureManager& textureManager = world.GetResourse<TextureManager>();
		ModelManager& modelManager = world.GetResourse<ModelManager>();
		world.View<Sprite, Transform>().ForEach([&modelManager, &textureManager, &renderer](Entity e, Sprite& sprite, Transform& trans)
			{
				textureManager.GetTexture(sprite.textureID).Bind();
				OpenGLVertexArray& vao = modelManager.GetModel(0);
				Transform t = trans;
				t.scale.x *= sprite.width;
				t.scale.y *= sprite.height;
				renderer.Submit(&(vao), t.asMat4());
			});
	}
	void RenderCollider(World& world)
	{
		OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
		TextureManager& textureManager = world.GetResourse<TextureManager>();
		ModelManager& modelManager = world.GetResourse<ModelManager>();
		world.View<BoxCollider, Transform>().ForEach([&modelManager, &textureManager, &renderer](Entity e, BoxCollider& colli, Transform& trans)
			{
				glm::vec3 offset = colli.offset - colli.halfSize;
				Transform t = trans;
				t.position += offset;
				t.scale.x *= colli.halfSize.x * 2;
				t.scale.y *= colli.halfSize.y * 2;
				renderer.SubmitDebug(&modelManager.GetModel(0), t.asMat4());
			});
	}
}