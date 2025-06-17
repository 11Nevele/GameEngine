#include "acpch.h"
#ifndef ECS
#define ECS


#ifndef ECS_ASSERT
#define ECS_ASSERT(condition, msg) \
		if (!(condition)) { \
			std::cerr << "[ECS error]: " << msg << std::endl; \
			::abort(); \
		}
#endif
#define ECS_INFO_ENABLED
#ifndef ECS_INFO
#ifdef ECS_INFO_ENABLED
#define ECS_INFO(msg) std::cout << "[ECS info]: " << msg << "\n";
#else
#define ECS_INFO(msg);
#endif
#endif
#ifndef ECS_MSG
#define ECS_MSG(msg) std::cout << "[ECS]: " << msg << "\n";
#endif
namespace ac
{
	/**
	 * @brief Entity identifier type.
	 * 
	 * Entities are represented as 64-bit unsigned integers.
	 */
	using Entity = uint64_t;
	
	/**
	 * @brief Resource identifier type.
	 */
	using ResourceID = uint32_t;
	
	/**
	 * @brief Component type identifier.
	 * 
	 * Each type of component has a unique ID.
	 */
	using ComponentID = uint32_t;
	
	/**
	 * @brief Index within each type of component.
	 */
	using ComponentIndex = uint32_t;
	
	/**
	 * @brief Represents an invalid or null entity.
	 */
	constexpr Entity NULL_ENTITY = UINT64_MAX;
	
	/**
	 * @brief Maximum number of component types supported by the ECS.
	 */
	constexpr size_t MAX_COMPONENTS = 64;

	/**
	 * @brief Interface for sparse set implementations.
	 * 
	 * Defines the base functionality required for all sparse sets
	 * regardless of their internal component type.
	 */
	class ISparseSet
	{
	public:
		virtual ~ISparseSet() = default;
		
		/**
		 * @brief Removes an entity from the sparse set.
		 * @param entity The entity to remove.
		 */
		virtual void Delete(Entity entity) = 0;
		
		/**
		 * @brief Removes all entities from the sparse set.
		 */
		virtual void Clear() = 0;
		
		/**
		 * @brief Returns the number of entities in the sparse set.
		 * @return The number of entities.
		 */
		virtual std::size_t Size() = 0;
		
		/**
		 * @brief Checks if the sparse set contains an entity.
		 * @param id The entity ID to check.
		 * @return True if the entity exists in the sparse set, false otherwise.
		 */
		virtual bool Contains(Entity id) = 0;
		
		/**
		 * @brief Returns a list of all entities in the sparse set.
		 * @return A vector containing all entity IDs.
		 */
		virtual std::vector<Entity> GetEntityList() = 0;
	};

	/**
	 * @brief SparseSet is a data structure that efficiently manages entities and their associated data.
	 * 
	 * It provides fast access, insertion, and deletion operations for entities.
	 * The SparseSet uses a dense array for storing entities and their data,
	 * and a sparse array for mapping entity IDs to indices in the dense array.
	 * 
	 * @tparam Type The type of data associated with entities.
	 */
	template <typename Type>
	struct SparseSet final : public ISparseSet
	{
	private:
		/** Stores the data associated with entities. */
		std::vector<Type> objects;

		/** Stores the list of entities in the sparse set. */
		std::vector<Entity> dense;

		/** Maps entities to their indices in the dense list. */
		std::vector<size_t> sparse;

		/** Maximum value for entity IDs. */
		uint32_t maxValue;

		/** Represents an invalid entity index. */
		const size_t Tombstone = UINT64_MAX;

		/**
		 * @brief Retrieves the dense index of an entity.
		 * 
		 * @param element The entity ID.
		 * @return The index in the dense array, or Tombstone if the entity is not found.
		 */
		size_t GetDenseID(Entity element)
		{
			if (!Contains(element))
				return Tombstone;
			return sparse[element];
		}

	public:
		/**
		 * @brief Constructor to initialize the sparse set with a maximum value for entity IDs.
		 * 
		 * @param maxValue The maximum value for entity IDs. Default is 1000.
		 */
		SparseSet(uint32_t maxValue = 1000)
		{
			this->maxValue = maxValue;
			sparse.resize(maxValue + 1, Tombstone);
		}

		/**
		 * @brief Destructor to clean up resources.
		 */
		~SparseSet() override
		{
			dense.clear();
			sparse.clear();
			objects.clear();
		}

		/**
		 * @brief Clears all entities and data from the sparse set.
		 */
		void Clear() override
		{
			dense.clear();
			sparse.clear();
			sparse.resize(1001);
			maxValue = 1000;
			objects.clear();
		}

		/**
		 * @brief Adds or updates an entity with associated data in the sparse set.
		 * 
		 * @param element The entity ID.
		 * @param obj The data to associate with the entity.
		 * @return A pointer to the data associated with the entity.
		 */
		Type* Set(Entity element, Type&& obj)
		{
			if (Contains(element))
			{
				size_t ind = GetDenseID(element);
				objects[ind] = obj;
				return &objects[ind];
			}

			if (element > maxValue)
				sparse.resize(10 * maxValue);
			dense.push_back(element);
			objects.emplace_back(std::forward<Type>(obj));
			sparse[element] = dense.size() - 1;
			return &objects.back();
		}

		/**
		 * @brief Checks if the sparse set contains the specified entity.
		 * 
		 * @param element The entity ID.
		 * @return True if the entity is in the sparse set, false otherwise.
		 */
		bool Contains(Entity element) override
		{
			return sparse[element] < dense.size() && dense[sparse[element]] == element;
		}

		/**
		 * @brief Retrieves the data associated with the specified entity.
		 * 
		 * @param id The entity ID.
		 * @return A pointer to the data associated with the entity, or nullptr if the entity is not found.
		 */
		Type* Get(Entity id)
		{
			if (!Contains(id))
				return nullptr;
			std::size_t index = GetDenseID(id);
			return &objects[index];
		}

		/**
		 * @brief Retrieves a reference to the data associated with the specified entity.
		 * 
		 * @param id The entity ID.
		 * @return A reference to the data associated with the entity.
		 * @throws An assertion failure if the entity is not found.
		 */
		Type& GetRef(Entity id)
		{
			if (!Contains(id))
				ECS_ASSERT(false, "GetRef called on invalid entity with ID " << id);
			std::size_t index = GetDenseID(id);

			return objects[index];
		}

		/**
		 * @brief Deletes an entity from the sparse set.
		 * 
		 * @param element The entity ID.
		 */
		void Delete(Entity element) override
		{
			if (!Contains(element))
				return;

			Entity lastElement = dense.back();
			size_t targetInd = GetDenseID(element);
			dense[targetInd] = lastElement;
			sparse[lastElement] = targetInd;
			objects[targetInd] = objects.back();

			objects.pop_back();
			dense.pop_back();
		}

		/**
		 * @brief Returns the number of entities in the sparse set.
		 * 
		 * @return The number of entities.
		 */
		std::size_t Size() override
		{
			return dense.size();
		}

		/**
		 * @brief Retrieves a list of all entities in the sparse set.
		 * 
		 * @return A vector containing all entity IDs.
		 */
		std::vector<Entity> GetEntityList() override
		{
			return dense;
		}

		/**
		 * @brief Checks if the sparse set is empty.
		 * 
		 * @return True if the sparse set is empty, false otherwise.
		 */
		bool IsEmpty() const
		{
			return dense.empty();
		}

		/**
		 * @brief Retrieves a read-only list of data associated with entities.
		 * 
		 * @return A const reference to the vector of data.
		 */
		const std::vector<Type>& Data() const
		{
			return objects;
		}

		/**
		 * @brief Prints the dense list of data associated with entities.
		 */
		void PrintDense()
		{
			for (const Type& e : objects)
			{
				std::cout << e << ", ";
			}
			std::cout << std::endl;
		}
	};

	/**
	 * @brief A type list that holds multiple component types.
	 * 
	 * This is a utility template used for storing and accessing
	 * component type information in the ECS.
	 * 
	 * @tparam Types The component types to store.
	 */
	template <class... Types>
	struct type_list {
		/** Tuple representation of the type list. */
		using type_tuple = std::tuple<Types...>;

		/**
		 * @brief Get the type at a specific index.
		 * 
		 * @tparam Index The index of the type to get.
		 */
		template <std::size_t Index>
		using get = std::tuple_element_t<Index, type_tuple>;

		/** The number of types in the list. */
		static constexpr std::size_t size = sizeof...(Types);
	};

	/**
	 * @brief SimpleView is a utility class for iterating over entities that have specific components.
	 * 
	 * It provides efficient access to entities and their associated components,
	 * allowing operations to be performed on matching entities.
	 * 
	 * @tparam Components The types of components to include in the view.
	 */
	template <typename... Components>
	class SimpleView {
	private:
		/** A type list representing the component types in the view. */
		using componentTypes = type_list<Components...>;

		/** Pools for components in the view. */
		std::array<ISparseSet*, sizeof...(Components)> m_viewPools;

		/** Sparse set with the smallest number of components. */
		ISparseSet* m_smallest = nullptr;

		/**
		 * @brief Checks if all pools in the view contain the specified entity.
		 * 
		 * @param id The entity ID.
		 * @return True if all pools contain the entity, false otherwise.
		 */
		bool AllContain(Entity id) {
			return std::all_of(m_viewPools.begin(), m_viewPools.end(), [id](ISparseSet* pool)
				{
					return pool->Contains(id);
				});
		}

		/**
		 * @brief Retrieves a specific component pool by index.
		 * 
		 * @tparam Index The index of the component pool.
		 * @return A pointer to the component pool.
		 */
		template <std::size_t Index>
		auto GetPoolAt() {
			using componentType = typename componentTypes::template get<Index>;
			return static_cast<SparseSet<componentType>*>(m_viewPools[Index]);
		}

		/**
		 * @brief Creates a tuple of components for the specified entity.
		 * 
		 * @param id The entity ID.
		 * @param indices The indices of the components.
		 * @return A tuple containing references to the components.
		 */
		template <std::size_t... Indices>
		auto MakeComponentTuple(Entity id, std::index_sequence<Indices...>) {
			return std::make_tuple((std::ref(GetPoolAt<Indices>()->GetRef(id)))...);
		}

		/**
		 * @brief Internal implementation for iterating over entities in the view.
		 * 
		 * @tparam Func The type of the function to execute.
		 * @param func The function to execute for each entity.
		 */
		template <typename Func>
		void ForEachImpl(Func func) {
			auto inds = std::make_index_sequence<sizeof...(Components)>{};

			for (Entity id : m_smallest->GetEntityList()) {
				if (AllContain(id)) {
					if constexpr (std::is_invocable_v<Func, Entity, Components&...>)
					{
						std::apply(func, std::tuple_cat(std::make_tuple(id), MakeComponentTuple(id, inds)));
					}
					else if constexpr (std::is_invocable_v<Func, Components&...>) {
						std::apply(func, MakeComponentTuple(id, inds));
					}
					else {
						ECS_ASSERT(false,
							"Bad lambda provided to .ForEach(), parameter pack does not match lambda args");
					}
				}
			}
		}

	public:
		/** Function signature for iterating components. */
		using ForEachFunc = std::function<void(Components&...)>;

		/** Function signature for iterating components with entity ID. */
		using ForEachFuncWithID = std::function<void(Entity, Components&...)>;

		/**
		 * @brief Constructor to initialize the view with component pools.
		 * 
		 * @param pools An array of sparse sets representing the component pools.
		 */
		SimpleView(std::array<ISparseSet*, sizeof...(Components)> pools) :
			m_viewPools{ pools }
		{
			ECS_ASSERT(componentTypes::size == m_viewPools.size(), "Component type list and pool array size mismatch");

			auto smallestPool = std::min_element(m_viewPools.begin(), m_viewPools.end(),
				[](ISparseSet* a, ISparseSet* b) { return a->Size() < b->Size(); }
			);

			ECS_ASSERT(smallestPool != m_viewPools.end(), "Initializing invalid/empty view");

			m_smallest = *smallestPool;
		}

		/**
		 * @brief Holds an entity id and a tuple of references to the components.
		 * 
		 * Access components that are part of a pack like such:
		 * - auto [componentA, componentB] = pack.components;
		 */
		struct Pack {
			Entity id;
			std::tuple<Components&...> components;
		};

		/**
		 * @brief Returns a vector of packs for all matching entities.
		 * 
		 * Useful when you want a way to iterate a view via indices.
		 * e.g:
		 *   auto packed = ecs.View<A, B>().GetPacked();
		 *   for (size_t i = 0; i < packed.size(); i++) {
		 *     auto [a1, b1] = packed[i].components;
		 *   }
		 * 
		 * @return A vector of Pack objects.
		 */
		std::vector<Pack> GetPacked() {
			auto inds = std::make_index_sequence<sizeof...(Components)>{};
			std::vector<Pack> result;

			for (Entity id : m_smallest->GetEntityList())
				if (AllContain(id))
					result.push_back({ id, MakeComponentTuple(id, inds) });
			return result;
		}

		/**
		 * @brief Executes a lambda on all entities matching the parameter pack.
		 * 
		 * @param func The lambda function to execute, includes Entity ID.
		 */
		void ForEach(ForEachFuncWithID func) {
			ForEachImpl(func);
		}

		/**
		 * @brief Executes a lambda on all entities matching the parameter pack without entity ID.
		 * 
		 * @param func The lambda function to execute, components only.
		 */
		void ForEach(ForEachFunc func) {
			ForEachImpl(func);
		}
	};
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

		template<class... Ts>
		friend class SimpleView;
	public:
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

			return *(p->Set(id, std::move(obj)));
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
	};

}


#endif 
// !ECS