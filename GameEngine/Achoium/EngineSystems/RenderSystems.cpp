#include "acpch.h"
#include "RenderSystems.h"
#include "AssetManagement\AssetManager.h"
#include "Math/Transform.h"
#include "EngineComponents/Physics/Physics.h"
#include "EngineComponents/Tilemap.h"
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
				renderer.Submit(&(vao), t.asMat4(), sprite.color);
			});
	}
	void RenderCircle(World& world)
	{
		OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
		TextureManager& textureManager = world.GetResourse<TextureManager>();
		ModelManager& modelManager = world.GetResourse<ModelManager>();
		float radius = 100;
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

		world.View<CircleCollider2D, Transform>().ForEach([&modelManager, &textureManager, &renderer](Entity e, CircleCollider2D& colli, Transform& trans)
			{
				renderer.SubmitCircle(&modelManager.GetModel(0), colli.radius, trans);
			});
	}
	void RenderTilemap(World& world)
	{
		OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
		TextureManager& textureManager = world.GetResourse<TextureManager>();
		ModelManager& modelManager = world.GetResourse<ModelManager>();
		world.View<TilemapElement, Sprite>().ForEach([&world, &modelManager, &textureManager, &renderer](Entity e, TilemapElement& tilemapElement, Sprite& sprite)
			{
				textureManager.GetTexture(sprite.textureID).Bind();
				OpenGLVertexArray& vao = modelManager.GetModel(0);
				Tilemap& tilemap = world.Get<Tilemap>(tilemapElement.tilemap);
				Transform t;
				if (world.Has<Transform>(tilemapElement.tilemap))
					Transform t = world.Get<Transform>(tilemapElement.tilemap);
				t.position += glm::vec3(tilemap.gridWidth, tilemap.gridHeight, 0) * t.scale * glm::vec3(tilemapElement.x, tilemapElement.y,0);
				t.scale.x = sprite.width;
				t.scale.y = sprite.height;

				renderer.Submit(&(vao), t.asMat4(), sprite.color);
			});
	}
	void SyncCamera(World& world)
	{
		OpenGLRenderer& renderer = world.GetResourse<OpenGLRenderer>();
		world.View<Camera, Transform>().ForEach([&renderer](Entity e, Camera& camera, Transform& transform)
			{
				renderer.UpdateCamera(transform.asMat4(true));
			});
	}
}