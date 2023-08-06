#pragma once

#include <Core/Event/EventData/BaseEvent.h>

namespace PR
{
	using ListenerID = std::pair<EventType, uint64_t>;
	using EventCallbackFn = std::function<void(Event&)>;

	class EventDispatcher
	{
	public:
		uint64_t AddListener(EventCallbackFn callback);
		bool RemoveListener(uint64_t listenerID);
		void RemoveAllListeners();
		void Dispatch(Event& e);

	private:
		std::unordered_map<uint64_t, EventCallbackFn> m_Callbacks;
		uint64_t m_ListenerID = 0;
	};

	class EventDispatchers
	{
	public:
		ListenerID AddListener(EventType eventType, EventCallbackFn callback);
		bool RemoveListener(ListenerID listenerID);
		void RemoveAllListeners();
		void Dispatch(Event& e);

	private:
		std::unordered_map<EventType, EventDispatcher> m_Dispatchers;
	};
}