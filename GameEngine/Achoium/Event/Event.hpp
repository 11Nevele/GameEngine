#ifndef EVENT_HPP
#define EVENT_HPP
#include "acpch.h"
#include <functional>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <memory>


#ifndef EVENT_ASSERT
#define EVENT_ASSERT(condition, msg) \
		if (!(condition)) { \
			std::cerr << "[EVENT error]: " << msg << std::endl; \
			::abort(); \
		}
#endif
//#define EVENT_INFO_ENABLED
#ifndef EVENT_INFO
#ifdef EVENT_INFO_ENABLED
#define EVENT_INFO(msg) std::cout << "[EVENT info]: " << msg << "\n";
#else
#define EVENT_INFO(msg);
#endif

#endif
#ifndef EVENT_MSG
#define EVENT_MSG(msg) std::cout << "[ECS]: " << msg << "\n";
#endif

namespace ac
{
	/**
	 * @brief Interface for event pool implementations.
	 * 
	 * Provides a common base class for type-specific event pools.
	 */
	class IEventPool
	{
	public:
		virtual ~IEventPool() = default;
	};

	/**
	 * @brief Type-specific implementation of an event pool.
	 * 
	 * Manages event listeners and event invocation for a specific event type T.
	 * 
	 * @tparam T The event data type this pool manages
	 */
	template<class T>
	class EventPool final : public IEventPool
	{
	public:
		EventPool() = default;
		~EventPool() override
		{
			eventListeners.clear();

		}

		/**
		 * @brief Registers a listener function for this event type.
		 * 
		 * @param func Function to be called when the event is invoked
		 */
		void AddListener(std::function<bool(const T&)> func)
		{
			eventListeners[func.target_type().hash_code()] = func;
		}

		/**
		 * @brief Removes a previously registered listener.
		 * 
		 * @param func Function to be removed
		 */
		void RemoveListener(std::function<bool(const T&)> func)
		{
			eventListeners.erase(func.target_type().hash_code());
		}

		/**
		 * @brief Triggers the event, calling all registered listeners.
		 * 
		 * @param data Event data to be passed to listeners
		 */
		void Invoke(const T& data)
		{
			for (auto& listener : eventListeners)
			{
				listener.second(data);
			}
		}
	private:
		std::unordered_map<size_t, std::function<bool(T)>> eventListeners; ///< Map of listener functions indexed by their hash
	};

	/**
	 * @brief Central manager for the event system.
	 * 
	 * Handles event registration, listener management, and event dispatch.
	 */
	class EventManager
	{
	public:
		/**
		 * @brief Registers a new event type in the system.
		 * 
		 * @tparam T The event data type to register
		 * @return EventManager& Reference to this manager for method chaining
		 */
		template <class T>
		EventManager& RegisterEvent()
		{
			EVENT_ASSERT(eventPools.size() < MAX_EVENT, "Event amount exceed max event count");
			auto it = eventID.find(typeid(T));
			if (it != eventID.end())
			{
				EVENT_MSG("WARNING try to register event already registered " << typeid(T).name());
				return *this;
			}

			eventID[typeid(T)] = eventPools.size();
			eventPools.push_back(std::make_unique<EventPool<T>>());
			EVENT_INFO("Registerd event: " << typeid(T).name());
			return *this;
		}

		/**
		 * @brief Adds a listener function for a specific event type.
		 * 
		 * @tparam T The event type to listen for
		 * @param func Function to be called when the event is triggered
		 * @return EventManager& Reference to this manager for method chaining
		 */
		template <class T>
		EventManager& AddListener(std::function<bool(const T&)> func)
		{
			auto it = eventID.find(typeid(T));
			EVENT_ASSERT(it != eventID.end(), "Try to add listener to an unregisterd event " << typeid(T).name());
			EventPool<T>* pool = GetPool<T>();
			pool->AddListener(func);
			EVENT_INFO("Added listener for event: " << (typeid(T).name()));
			return *this;
		}

		/**
		 * @brief Triggers an event, notifying all registered listeners.
		 * 
		 * @tparam T The event type to trigger
		 * @param event Event data to be passed to listeners
		 * @return EventManager& Reference to this manager for method chaining
		 */
		template<class T>
		EventManager& Invoke(const T& event)
		{
			auto it = eventID.find(typeid(T));
			EVENT_ASSERT(it != eventID.end(), "Try to invoke an unregisterd event " << typeid(T).name());
			EventPool<T>* pool = GetPool<T>();
			pool->Invoke(event);
			EVENT_INFO("Event invoked: ");
			return *this;
		}

		/**
		 * @brief Checks if an event type is registered in the system.
		 * 
		 * @tparam T The event type to check
		 * @return true If the event type is registered
		 * @return false If the event type is not registered
		 */
		template <class T>
		bool Contain()
		{
			auto it = eventID.find(typeid(T));
			return it != eventID.end();
		}
	private:
		/**
		 * @brief Gets the event pool for a specific event type.
		 * 
		 * @tparam T The event type to get the pool for
		 * @return EventPool<T>* Pointer to the event pool
		 */
		template <class T>
		EventPool<T>* GetPool()
		{
			auto it = eventID.find(typeid(T));
			EVENT_ASSERT(it != eventID.end(), "Try to use an unregisterd event");
			size_t ind = it->second;
			return static_cast<EventPool<T>*>(eventPools[ind].get());
		}
		const size_t MAX_EVENT = UINT64_MAX; ///< Maximum number of event types that can be registered

		std::unordered_map<std::type_index, size_t> eventID; ///< Maps event types to their pool index
		std::vector<std::unique_ptr<IEventPool>> eventPools; ///< Stores all event pools
	};
}

#endif // !EVENT_HPP
