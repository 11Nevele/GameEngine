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
	void RenderCircle(World& world)
	{
		OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
		TextureManager& textureManager = world.GetResourse<TextureManager>();
		ModelManager& modelManager = world.GetResourse<ModelManager>();
		float radius = 250;
		static Transform t;
		t.position.x += 1;
		t.RotateZ(0.1);
		renderer.SubmitCircle(&modelManager.GetModel(0), radius, t);
	}
	void RenderCollider(World& world)
	{
		OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
		TextureManager& textureManager = world.GetResourse<TextureManager>();
		ModelManager& modelManager = world.GetResourse<ModelManager>();
		world.View<RectCollider2D, Transform>().ForEach([&modelManager, &textureManager, &renderer](Entity e, RectCollider2D& colli, Transform& trans)
			{
				glm::vec2 tmp = (colli.offset - colli.halfSize) / colli.halfSize / 2.0f;
				glm::vec3 offset = glm::vec3(tmp,0);
				Transform t = trans;
				t.scale.x *= colli.halfSize.x * 2;
				t.scale.y *= colli.halfSize.y * 2;
				glm::mat4 m = t.asMat4() * glm::translate(glm::mat4(1),offset);
				renderer.SubmitDebug(&modelManager.GetModel(0), m);
			});
	}
}