#include "acpch.h"
#include "RenderSystems.h"
#include "AssetManagement\AssetManager.h"
#include "Math/Transform.h"
#include "EngineComponents/Physics/Physics.h"
#include "EngineComponents/Tilemap.h"
#include "EngineComponents/TextComponent.h"
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
		/*world.View<Sprite, Transform>().ForEach([&modelManager, &textureManager, &renderer](Entity e, Sprite& sprite, Transform& trans)
			{
				textureManager.GetTexture(sprite.textureID).Bind();
				OpenGLVertexArray& vao = modelManager.GetModel(0);
				Transform t = trans;
				t.scale.x *= sprite.width;
				t.scale.y *= sprite.height;
				renderer.Submit(&(vao), t.asMat4(), sprite.color);
			});
			*/
		struct renderable
		{
			Entity e;
			float zIndex;
			int type = 0;//0 sprite, 1 tiled sprite, 2 text
		};
		std::vector<renderable> renderables;
		world.View<Sprite, Transform>().ForEach([&renderables](Entity e, Sprite& sprite, Transform& t)
			{
				renderables.emplace_back(renderable{ e, t.position.z, 0 });
			});
		world.View<Sprite, TilemapElement>().ForEach([&renderables, &world](Entity e, Sprite&s, TilemapElement& te)
			{
				
				renderables.emplace_back(renderable{ e, world.Get<Transform>(te.tilemap).position.z, 1 });
			});
		world.View<Text, Transform>().ForEach([&renderables](Entity e, Text& text, Transform& t)
			{
				renderables.emplace_back(renderable{ e, t.position.z, 2 });
			});
		std::sort(renderables.begin(), renderables.end(), [](const renderable& a, const renderable& b)
			{
				return a.zIndex > b.zIndex;
			});
		for (const auto& r : renderables)
		{
			switch (r.type)
			{
			case 0:
			{
				Sprite& sprite = world.Get<Sprite>(r.e);
				Transform t = world.Get<Transform>(r.e);
				textureManager.GetTexture(sprite.textureID).Bind();
				OpenGLVertexArray& vao = modelManager.GetModel(0);
				t.scale.x *= sprite.width;
				t.scale.y *= sprite.height;
				renderer.Submit(&(vao), t.asMat4(), sprite.color);
			}
				

				break;
			case 1:
			{
				Sprite& tiledSprite = world.Get<Sprite>(r.e);
				TilemapElement& tilemapElement = world.Get<TilemapElement>(r.e);
				textureManager.GetTexture(tiledSprite.textureID).Bind();
				OpenGLVertexArray& vao = modelManager.GetModel(0);
				Tilemap& tilemap = world.Get<Tilemap>(tilemapElement.tilemap);
				Transform t2;
				if (world.Has<Transform>(tilemapElement.tilemap))
					t2 = world.Get<Transform>(tilemapElement.tilemap);
				t2.position += glm::vec3(tilemap.gridWidth, tilemap.gridHeight, 0) * t2.scale * glm::vec3(tilemapElement.x, tilemapElement.y, 0);
				t2.scale.x *= tilemap.gridHeight;
				t2.scale.y *= tilemap.gridHeight;
				renderer.Submit(&(vao), t2.asMat4(), tiledSprite.color);
			}
				
				break;
			case 2:
			{
				Text& text = world.Get<Text>(r.e);
				Transform t3 = world.Get<Transform>(r.e);
				float scaleFactor = text.fontSize / 48.0f; // 假设基础字体大小为48
				t3.scale *= scaleFactor;
				renderer.SubmitText(text.text, t3, text.color, text.pivot);
			}
				
				break;
			}
		}

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
					t = world.Get<Transform>(tilemapElement.tilemap);
				t.position += glm::vec3(tilemap.gridWidth, tilemap.gridHeight, 0) * t.scale * glm::vec3(tilemapElement.x, tilemapElement.y,0);
				t.scale.x *= tilemap.gridHeight;
				t.scale.y *= tilemap.gridHeight;

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