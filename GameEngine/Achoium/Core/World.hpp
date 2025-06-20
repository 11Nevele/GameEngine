#pragma once

#include "ECSDebug.h"
#include "sparseset.hpp"
#include <iostream>
#include "ECSEvents.h"
#include "Event\Event.hpp"
namespace ac
{


	/**
 * @brief The World class is a core component of the ECS (Entity-Component-System) architecture.
 *
 * It serves as the central manager for entities, components, and systems within the game engine.
 * This class is designed to be final, meaning it cannot be inherited.
 *
 * Responsibilities:
 * - Manage the lifecycle of entities and components.
 * - Facilitate communication between systems.
 * - Provide utilities for querying and manipulating entities and their associated components.
 */
	class World final
	{
	private:
		

		/** The next entity ID to be assigned. */
		Entity maxEnity = 0;

		/** Bitmask type for tracking which components an entity has. */
		using ComponentMask = std::bitset<MAX_COMPONENTS>;

		/** Maps entities to their component masks. */
		SparseSet<ComponentMask> entityComponentMasks;

		/** Pool of available entity IDs for reuse. */
		std::vector<Entity> entityPool;

		/** Maps entities to debug names/tags. */
		std::unordered_map<Entity, std::string> entityTag;

		/** Maps component types to their bitmask indices. */
		std::unordered_map<std::type_index, size_t> typeToBitMaskInd;

		/** Stores all component pools, one per component type. */
		std::vector<std::unique_ptr<ISparseSet>> ComponentPools;

		/** Maps resource types to their indices. */
		std::unordered_map<std::type_index, size_t> resourceID;

		/** Stores all resources. */
		std::vector<std::shared_ptr<void>> resourceList;

		std::array<std::vector<void(*)(World&)>, MAX_SYSTEM_LAYER> preUpdateSystems;

		std::array<std::vector<void(*)(World&)>, MAX_SYSTEM_LAYER> updateSystems;

		std::array<std::vector<void(*)(World&)>, MAX_SYSTEM_LAYER> postUpdateSystems;

		/**
		 * @brief Retrieves the tag/name of an entity.
		 *
		 * @param id The entity ID.
		 * @return The tag as a string.
		 */
		std::string GetEntityTag(Entity id)
		{
			return entityTag[id];
		}
#define ENTITY_INFO(id) \
		"['" << GetEntityTag(id) << "', ID: " << id << "]"

#define ECS_ASSERT_VALID_ENTITY(id) \
		ECS_ASSERT(id != NULL_ENTITY, "NULL_ENTITY cannot be operated on by the ECS") \
		ECS_ASSERT(id < maxEnity && id >= 0, "Invalid entity ID out of bounds: " + id);

#define ECS_ASSERT_ALIVE_ENTITY(id) \
		ECS_ASSERT(entityComponentMasks.Contains(id), "Attempting to access inactive entity with ID: " + id);

		/**
		 * @brief Gets the component ID for a specific component type.
		 *
		 * @tparam T The component type.
		 * @return The component's unique ID, or UINT64_MAX if not registered.
		 */
		template<class T>
		size_t GetComponentID()
		{
			std::type_index ind(typeid(T));
			auto  t = typeToBitMaskInd.find(ind);
			if (t == typeToBitMaskInd.end())
				return UINT64_MAX;
			return t->second;
		}

		/**
		 * @brief Gets the sparse set pointer for a component type.
		 *
		 * @tparam T The component type.
		 * @return Pointer to the sparse set for the specified component type.
		 */
		template<class T>
		SparseSet<T>* GetSparseSetPtr()
		{
			size_t ind = GetComponentID<T>();
			ECS_ASSERT(ind != UINT64_MAX, "Try to access a type that does not exist");
			return static_cast<SparseSet<T>*>(ComponentPools[ind].get());
		}

		/**
		 * @brief Validates system priority to ensure it's within bounds.
		 * 
		 * @param priority Priority level (0-9).
		 * @return Valid priority value.
		 */
		size_t ValidatePriority(size_t priority)
		{
			ECS_ASSERT(priority < MAX_SYSTEM_LAYER, "System priority must be in range [0," << MAX_SYSTEM_LAYER << "]");
			return priority;
		}

		template<class... Ts>
		friend class SimpleView;
	public:

		World()
		{
			AddResource<EventManager>(new EventManager());
		}

		/**
		 * @brief Adds a resource to the world.
		 *
		 * @warning YOU MUST PASS IN A POINTER ON HEAP
		 *
		 * @tparam T The resource type.
		 * @param obj Pointer to the resource.
		 */
		template<class T>
		void AddResource(T* obj)
		{
			auto it = resourceID.find(typeid(T));
			if (it != resourceID.end())
			{
				ECS_MSG("WARNING try to add a resource that is already added");
				return;
			}
			resourceID[typeid(T)] = resourceList.size();
			resourceList.push_back(std::shared_ptr<T>(obj));
		}

		/**
		 * @brief Gets a reference to a resource.
		 *
		 * @tparam T The resource type.
		 * @return Reference to the resource.
		 */
		template<class T>
		T& GetResourse()
		{
			auto it = resourceID.find(typeid(T));
			ECS_ASSERT(it != resourceID.end(), "Try to access unregistered resourse");
			T* t = static_cast<T*>(resourceList[it->second].get());
			return *t;
		}

		/**
		 * @brief Resets the world, removing all entities and components.
		 */
		void Reset()
		{
			entityComponentMasks.Clear();
			entityPool.clear();
			entityTag.clear();

			typeToBitMaskInd.clear();

			ComponentPools.clear();
			maxEnity = 0;
		}

		/**
		 * @brief Creates a new entity.
		 *
		 * @param str Optional tag/name for the entity.
		 * @return The new entity ID.
		 */
		Entity CreateEntity(std::string_view str = "")
		{
			Entity id;
			if (entityPool.empty())
			{
				ECS_ASSERT(maxEnity < NULL_ENTITY, "Enity Exceed max Entity");
				id = maxEnity++;
			}
			else
				id = entityPool.back(), entityPool.pop_back();

			entityComponentMasks.Set(id, {});
			if (!str.empty())
				entityTag[id] = str;
			ECS_INFO("Entity Created: " << ENTITY_INFO(id));
			return id;
		}

		/**
		 * @brief Deletes an entity and all its components.
		 *
		 * @param id The entity to delete.
		 */
		void DeleteEntity(Entity id)
		{
			ECS_ASSERT_VALID_ENTITY(id);
			ECS_ASSERT_ALIVE_ENTITY(id);

			if (entityTag.find(id) != entityTag.end())
				entityTag.erase(id);

			ComponentMask* mask = entityComponentMasks.Get(id);
			for (int i = 0; i < mask->size(); ++i)
				if ((*mask)[i])
					ComponentPools[i]->Delete(id);

			entityComponentMasks.Delete(id);
			entityPool.push_back(id);
		}


		/**
		 * @brief Registers a component type with the world.
		 *
		 * @tparam T The component type to register.
		 */
		template <class T>
		void RegisterType()
		{
			ECS_ASSERT(ComponentPools.size() < MAX_COMPONENTS, "Type of Component greate than MAX_COMPONENTS");
			std::type_index typeID(typeid(T));
			typeToBitMaskInd[typeID] = ComponentPools.size();
			ComponentPools.push_back(std::make_unique<SparseSet<T>>());
			GetResourse<EventManager>().RegisterEvent<OnAdded<T>>()
				.RegisterEvent<OnDeleted<T>>();

			ECS_INFO("Registerd component: " << typeid(T).name() << " with index of: " << ComponentPools.size() - 1);
		}

		/**
		 * @brief Adds a component to an entity.
		 *
		 * @tparam T The component type.
		 * @param id The entity ID.
		 * @param obj The component instance (default constructed if not provided).
		 * @return Reference to the added component.
		 */
		template <class T>
		T& Add(Entity id, T&& obj = {})
		{
			ECS_ASSERT_VALID_ENTITY(id);
			ECS_ASSERT_ALIVE_ENTITY(id);
			auto t = typeToBitMaskInd.find(typeid(T));
			if (t == typeToBitMaskInd.end())
			{
				RegisterType<T>();
				t = typeToBitMaskInd.find(typeid(T));
			}

			size_t bitMaskInd = t->second;
			ComponentMask* mask = entityComponentMasks.Get(id);
			mask->set(bitMaskInd, true);

			ISparseSet* set = ComponentPools[bitMaskInd].get();
			SparseSet<T>* p = static_cast<SparseSet<T>*>(set);

			T& o = *(p->Set(id, std::move(obj)));
			GetResourse<EventManager>().Invoke(OnAdded<T>{id, o}, AllowToken<OnAdded<T>>());
			return o;
		}

		/**
		 * @brief Removes a component from an entity.
		 *
		 * @tparam T The component type to remove.
		 * @param id The entity ID.
		 */
		template <class T>
		void Delete(Entity id)
		{
			ECS_ASSERT_VALID_ENTITY(id);
			ECS_ASSERT_ALIVE_ENTITY(id);
			auto t = typeToBitMaskInd.find(typeid(T));
			ECS_ASSERT(t != typeToBitMaskInd.end(), "Try to delete component that have never been addded");

			SparseSet<T>* p = static_cast<SparseSet<T>*>(ComponentPools[t->second].get());

			T& o= *(p->Get(id));
			GetResourse<EventManager>().Invoke(OnDeleted<T>{id, o}, AllowToken<OnDeleted<T>>());

			size_t bitMaskInd = t->second;
			ComponentMask* mask = entityComponentMasks.Get(id);
			mask->set(bitMaskInd, false);

			ComponentPools[bitMaskInd]->Delete(id);
		}

		/**
		 * @brief Retrieves the specified component for the given entity.
		 *
		 * Usage example: ecs.Get<Transform>(player);
		 *
		 * @tparam T The component type.
		 * @param id The entity ID.
		 * @return Reference to the component.
		 */
		template <typename T>
		T& Get(Entity id) {
			ECS_ASSERT_VALID_ENTITY(id);
			ECS_ASSERT_ALIVE_ENTITY(id);

			auto t = typeToBitMaskInd.find(typeid(T));
			ECS_ASSERT(t != typeToBitMaskInd.end(), "Tryed to get component that is never registerd");

			SparseSet<T>* p = static_cast<SparseSet<T>*>(ComponentPools[t->second].get());

			return *(p->Get(id));
		}

		/**
		 * @brief Retrieves a pointer to the specified component for the given entity.
		 *
		 * Usage example: ecs.GetPtr<Transform>(player);
		 *
		 * @tparam T The component type.
		 * @param id The entity ID.
		 * @return Pointer to the component, or nullptr if the entity doesn't have the component.
		 */
		template <typename T>
		T* GetPtr(Entity id) {
			ECS_ASSERT_VALID_ENTITY(id);
			ECS_ASSERT_ALIVE_ENTITY(id);

			auto t = typeToBitMaskInd.find(typeid(T));
			ECS_ASSERT(t != typeToBitMaskInd.end(), "Tryed to get component that is never registerd");

			SparseSet<T>* p = static_cast<SparseSet<T>*>(ComponentPools[t->second].get());

			return p->Get(id);
		}

		/**
		 * @brief Checks if an entity has all of the specified component types.
		 *
		 * @tparam Ts The component types to check.
		 * @param id The entity ID.
		 * @return True if the entity has all the specified components, false otherwise.
		 */
		template <typename... Ts>
		bool HasAll(Entity id) {
			return (Has<Ts>(id) && ...);
		}

		/**
		 * @brief Checks if an entity has any of the specified component types.
		 *
		 * @tparam Ts The component types to check.
		 * @param id The entity ID.
		 * @return True if the entity has any of the specified components, false otherwise.
		 */
		template <typename... Ts>
		bool HasAny(Entity id) {
			return (Has<Ts>(id) || ...);
		}

		/**
		 * @brief Checks if an entity has a specific component type.
		 *
		 * @tparam T The component type to check.
		 * @param id The entity ID.
		 * @return True if the entity has the component, false otherwise.
		 */
		template <typename T>
		bool Has(Entity id)
		{
			auto t = typeToBitMaskInd.find(typeid(T));
			if (t == typeToBitMaskInd.end())
				return false;
			ComponentMask* p = entityComponentMasks.Get(id);
			if (p == nullptr)
				return false;
			return (*p)[t->second];
		}

		/**
		 * @brief Creates a view for iterating over entities with specific components.
		 *
		 * @tparam Components The component types to include in the view.
		 * @return A SimpleView object for the specified component types.
		 */
		template <typename... Components>
		SimpleView<Components...> View() {
			// Pass a copy of array from fold expression into view.
			return { { GetSparseSetPtr<Components>()... } };
		}

		/**
		 * @brief Returns the total number of entities in the world.
		 *
		 * @return The entity count.
		 */
		std::size_t GetEntityCount() {
			return entityComponentMasks.Size();
		}

		/**
		 * @brief Returns the number of component pools in the world.
		 *
		 * @return The component pool count.
		 */
		std::size_t GetPoolCount() {
			return ComponentPools.size();
		}

		/**
		 * @brief Adds a system to be executed during the pre-update phase.
		 * 
		 * @param systemFunc Function pointer to the system (void(*)(World&)).
		 * @param priority Priority level (0-9), lower values execute first.
		 * @return Reference to this World for chaining.
		 */
		World& AddPreUpdateSystem(void(*systemFunc)(World&), size_t priority)
		{
			preUpdateSystems[ValidatePriority(priority)].push_back(systemFunc);
			return *this;
		}

		/**
		 * @brief Adds a system to be executed during the update phase.
		 * 
		 * @param systemFunc Function pointer to the system (void(*)(World&)).
		 * @param priority Priority level (0-9), lower values execute first.
		 * @return Reference to this World for chaining.
		 */
		World& AddUpdateSystem(void(*systemFunc)(World&), size_t priority)
		{
			updateSystems[ValidatePriority(priority)].push_back(systemFunc);
			return *this;
		}

		/**
		 * @brief Adds a system to be executed during the post-update phase.
		 * 
		 * @param systemFunc Function pointer to the system (void(*)(World&)).
		 * @param priority Priority level (0-9), lower values execute first.
		 * @return Reference to this World for chaining.
		 */
		World& AddPostUpdateSystem(void(*systemFunc)(World&), size_t priority)
		{
			postUpdateSystems[ValidatePriority(priority)].push_back(systemFunc);
			return *this;
		}

		/**
		 * @brief Executes all pre-update systems in order of priority.
		 * 
		 * Systems with lower priority values execute first.
		 * Within the same priority level, systems execute in the order they were added.
		 */
		void RunPreUpdateSystems()
		{
			for (size_t priority = 0; priority < preUpdateSystems.size(); ++priority)
			{
				for (auto& system : preUpdateSystems[priority])
				{
					system(*this);
				}
			}
		}

		/**
		 * @brief Executes all update systems in order of priority.
		 * 
		 * Systems with lower priority values execute first.
		 * Within the same priority level, systems execute in the order they were added.
		 */
		void RunUpdateSystems()
		{
			for (size_t priority = 0; priority < updateSystems.size(); ++priority)
			{
				for (auto& system : updateSystems[priority])
				{
					system(*this);
				}
			}
		}

		/**
		 * @brief Executes all post-update systems in order of priority.
		 * 
		 * Systems with lower priority values execute first.
		 * Within the same priority level, systems execute in the order they were added.
		 */
		void RunPostUpdateSystems()
		{
			for (size_t priority = 0; priority < postUpdateSystems.size(); ++priority)
			{
				for (auto& system : postUpdateSystems[priority])
				{
					system(*this);
				}
			}
		}

		void Update()
		{
			RunPreUpdateSystems();
			RunUpdateSystems();
			RunPostUpdateSystems();
		}
	};
}
