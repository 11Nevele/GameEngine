
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
	class IEventPool
	{
	public:
		virtual ~IEventPool() = default;
	};

	template<class T>
	class EventPool final : public IEventPool
	{
	public:
		EventPool() = default;
		~EventPool() override
		{
			eventListeners.clear();

		}
		void AddListener(std::function<bool(const T&)> func)
		{
			eventListeners[func.target_type().hash_code()] = func;
		}
		void RemoveListener(std::function<bool(const T&)> func)
		{
			eventListeners.erase(func.target_type().hash_code());
		}
		void Invoke(const T& data)
		{
			for (auto& listener : eventListeners)
			{
				listener.second(data);
			}
		}
	private:
		std::unordered_map<size_t, std::function<bool(T)>> eventListeners;
	};

	class EventManager
	{
	public:
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
		template <class T>
		bool Contain()
		{
			auto it = eventID.find(typeid(T));
			return it != eventID.end();
		}
	private:
		

		template <class T>
		EventPool<T>* GetPool()
		{
			auto it = eventID.find(typeid(T));
			EVENT_ASSERT(it != eventID.end(), "Try to use an unregisterd event");
			size_t ind = it->second;
			return static_cast<EventPool<T>*>(eventPools[ind].get());
		}
		const size_t MAX_EVENT = UINT64_MAX;

		std::unordered_map<std::type_index, size_t> eventID;

		std::vector<std::unique_ptr<IEventPool>> eventPools;
	};
}



#endif // !EVENT_HPP
