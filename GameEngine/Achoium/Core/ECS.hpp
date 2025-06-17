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

	using Entity = uint64_t;
	using ResourceID = uint32_t;
	using ComponentID = uint32_t; //each type of component has a unique ID
	using ComponentIndex = uint32_t; //a index within each type of component
	constexpr Entity NULL_ENTITY = UINT64_MAX;
	constexpr size_t MAX_COMPONENTS = 64;
	class ISparseSet
	{
	public:
		virtual ~ISparseSet() = default;
		virtual void Delete(Entity) = 0;
		virtual void Clear() = 0;
		virtual std::size_t Size() = 0;
		virtual bool Contains(Entity id) = 0;
		virtual std::vector<Entity> GetEntityList() = 0;

	};

	template <typename Type>
	struct SparseSet final : public ISparseSet
	{
	private:
		std::vector<Type> objects;
		std::vector<Entity> dense;
		std::vector<size_t> sparse;
		uint32_t maxValue;
		const size_t Tombstone = UINT64_MAX;


		//add an element to the set

		size_t GetDenseID(Entity element)
		{
			if (!Contains(element))
				return Tombstone;
			return sparse[element];
		}
	public:
		SparseSet(uint32_t maxValue = 1000)
		{
			this->maxValue = maxValue;
			sparse.resize(maxValue + 1, Tombstone);
		}
		~SparseSet() override
		{
			dense.clear();
			sparse.clear();
			objects.clear();
		}
		void Clear() override
		{
			dense.clear();
			sparse.clear();
			sparse.resize(1001);
			maxValue = 1000;
			objects.clear();
		}
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
		bool Contains(Entity element) override
		{
			return sparse[element] < dense.size() && dense[sparse[element]] == element;
		}
		Type* Get(Entity id)
		{
			if (!Contains(id))
				return nullptr;
			std::size_t index = GetDenseID(id);
			return &objects[index];
		}

		Type& GetRef(Entity id)
		{
			if (!Contains(id))
				ECS_ASSERT(false, "GetRef called on invalid entity with ID " << id);
			std::size_t index = GetDenseID(id);

			return objects[index];
		}
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
		std::size_t Size() override
		{
			return dense.size();
		}

		std::vector<Entity> GetEntityList() override
		{
			return dense;
		}

		bool IsEmpty() const
		{
			return dense.empty();
		}

		// Read-only dense list
		const std::vector<Type>& Data() const
		{
			return objects;
		}
		void PrintDense()
		{
			for (const Type& e : objects)
			{
				std::cout << e << ", ";
			}
			std::cout << std::endl;
		}
	};

	template <class... Types>
	struct type_list {
		using type_tuple = std::tuple<Types...>;

		template <std::size_t Index>
		using get = std::tuple_element_t<Index, type_tuple>;

		static constexpr std::size_t size = sizeof...(Types);
	};

	template <typename... Components>
	class SimpleView {
	private:

		using componentTypes = type_list<Components...>;

		std::array<ISparseSet*, sizeof...(Components)> m_viewPools;

		// Sparse set with the smallest number of components,
		// basis for ForEach iterations.
		ISparseSet* m_smallest = nullptr;

		/*
		*	Returns true iff all the pools in the view contain the given Entity
		*/
		bool AllContain(Entity id) {
			return std::all_of(m_viewPools.begin(), m_viewPools.end(), [id](ISparseSet* pool)
				{
					return pool->Contains(id);
				});
		}

		/*
		*	Index the generic pool array and downcast to a specific component pool
		*   by using compile time indices
		*/
		template <std::size_t Index>
		auto GetPoolAt()
		{
			using componentType = typename componentTypes::template get<Index>;
			return static_cast<SparseSet<componentType>*>(m_viewPools[Index]);
		}

		template <std::size_t... Indices>
		auto MakeComponentTuple(Entity id, std::index_sequence<Indices...>) {
			return std::make_tuple((std::ref(GetPoolAt<Indices>()->GetRef(id)))...);
		}

		/*
		*  Provided the function arguments are valid, this function will iterate over the smallest pool
		*  and run the lambda on all entities that contain all the components in the view.
		*
		*  Note: This is the internal implementation: opt for the more user friendly functional ones in the
		*        public interface.
		*/
		template <typename Func>
		void ForEachImpl(Func func) {
			auto inds = std::make_index_sequence<sizeof...(Components)>{};

			// Iterate smallest component pool and compare against other pools in view
			// Note this list is a COPY, allowing safe deletion during iteration.
			for (Entity id : m_smallest->GetEntityList()) {
				if (AllContain(id)) {

					// This branch is for [](EntityID id, Component& c1, Component& c2);
					// constexpr denotes this is evaluated at compile time, which prunes
					// invalid function call branches before runtime to prevent the
					// typical invoke errors you'd see after building.
					if constexpr (std::is_invocable_v<Func, Entity, Components&...>)
					{
						std::apply(func, std::tuple_cat(std::make_tuple(id), MakeComponentTuple(id, inds)));
					}

					// This branch is for [](Component& c1, Component& c2);
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

		// These are the function signatures you can pass to .ForEach()
		using ForEachFunc = std::function<void(Components&...)>;
		using ForEachFuncWithID = std::function<void(Entity, Components&...)>;

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

		/*
		*  Executes a passed lambda on all the entities that match the
		*  passed parameter pack.
		*
		*  Provided function should follow one of two forms:
		*  [](Component& c1, Component& c2);
		*  OR
		*  [](EntityID id, Component& c1, Component& c2);
		*/
		void ForEach(ForEachFunc func) {
			ForEachImpl(func);
		}

		void ForEach(ForEachFuncWithID func) {
			ForEachImpl(func);
		}

		/*
		*	Holds an entity id and a tuple of references to the components returned by the view.
		*	Access components that are part of a pack like such:
		*	- auto [componentA, componentB] = pack.components;
		*/
		struct Pack {
			Entity id;
			std::tuple<Components&...> components;
		};

		/*
		*  Useful when you want a way to iterate a view via indices.
		*  e.g:
			auto packed = ecs.View<A, B>().GetPacked();
			for (size_t i = 0; i < packed.size(); i++) {
				auto [a1, b1] = packed[i].components;
			}
		*/
		std::vector<Pack> GetPacked() {
			auto inds = std::make_index_sequence<sizeof...(Components)>{};
			std::vector<Pack> result;

			for (Entity id : m_smallest->GetEntityList())
				if (AllContain(id))
					result.push_back({ id, MakeComponentTuple(id, inds) });
			return result;
		}


	};

	class World final
	{


	private:


		Entity maxEnity = 0;
		using ComponentMask = std::bitset<MAX_COMPONENTS>;
		SparseSet<ComponentMask> entityComponentMasks;

		std::vector<Entity> entityPool;

		std::unordered_map<Entity, std::string> entityTag;

		std::unordered_map<std::type_index, size_t> typeToBitMaskInd;

		std::vector<std::unique_ptr<ISparseSet>> ComponentPools;

		std::unordered_map<std::type_index, size_t> resourceID;

		std::vector<std::shared_ptr<void>> resourceList;


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

		

		template<class T>
		size_t GetComponentID()
		{
			std::type_index ind(typeid(T));
			auto  t = typeToBitMaskInd.find(ind);
			if (t == typeToBitMaskInd.end())
				return UINT64_MAX;
			return t->second;
		}

		template<class T>
		SparseSet<T>* GetSparseSetPtr()
		{
			size_t ind = GetComponentID<T>();
			ECS_ASSERT(ind != UINT64_MAX, "Try to access a type that does not exist");
			return static_cast<SparseSet<T>*>(ComponentPools[ind].get());
		}
		template<class... Ts>
		friend class  SimpleView;
	public:

		template<class T>
		//!!!YOU MUST PASS IN A POINTER ON HEAP!!!
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
		template<class T>
		T& GetResourse()
		{
			auto it = resourceID.find(typeid(T));
			ECS_ASSERT(it != resourceID.end(), "Try to access unregistered resourse");
			T* t = static_cast<T*>(resourceList[it->second].get());
			return *t;
		}

		void Reset()
		{
			entityComponentMasks.Clear();
			entityPool.clear();
			entityTag.clear();

			typeToBitMaskInd.clear();

			ComponentPools.clear();
			maxEnity = 0;
		}
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

		template <class T>
		void RegisterType()
		{
			ECS_ASSERT(ComponentPools.size() < MAX_COMPONENTS, "Type of Component greate than MAX_COMPONENTS");
			std::type_index typeID(typeid(T));
			typeToBitMaskInd[typeID] = ComponentPools.size();
			ComponentPools.push_back(std::make_unique<SparseSet<T>>());
			ECS_INFO("Registerd component: " << typeid(T).name() << " with index of: " << ComponentPools.size() - 1);
		}

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

		/*
		*  Retrieves the specified component for the given entity
		*
		* - ecs.Get<Transform>(player);
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

		/*
		*  Retrieves a pointer to the specified component for the given entity
		*
		* - ecs.GetPtr<Transform>(player);
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

		template <typename... Ts>
		bool HasAll(Entity id) {
			return (Has<Ts>(id) && ...);
		}

		template <typename... Ts>
		bool HasAny(Entity id) {
			return (Has<Ts>(id) || ...);
		}

		template <typename T>
		bool Has(Entity id)
		{
			auto t = typeToBitMaskInd.find(typeid(T));
			if (t == typeToBitMaskInd.end())
				return false;
			ComponentMask *p = entityComponentMasks.Get(id);
			if (p == nullptr)
				return false;
			return (*p)[t->second];
		}

		template <typename... Components>
		SimpleView<Components...> View() {
			// Pass a copy of array from fold expression into view.
			return { { GetSparseSetPtr<Components>()... } };
		}

		std::size_t GetEntityCount() {
			return entityComponentMasks.Size();
		}

		std::size_t GetPoolCount() {
			return ComponentPools.size();
		}

	};

}


#endif 
// !ECS