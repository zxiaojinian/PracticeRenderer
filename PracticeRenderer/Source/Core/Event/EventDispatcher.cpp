#include "pch.h"
#include "EventDispatcher.h"

namespace PR
{
    uint64_t EventDispatcher::AddListener(EventCallbackFn callback)
    {
        uint64_t listenerID = m_ListenerID++;
        m_Callbacks.emplace(listenerID, callback);
        return listenerID;
    }

    bool EventDispatcher::RemoveListener(uint64_t listenerID)
    {
        return m_Callbacks.erase(listenerID) != 0;
    }

    void EventDispatcher::RemoveAllListeners()
    {
        m_Callbacks.clear();
    }

    void EventDispatcher::Dispatch(Event& e)
    {
        for (const auto& [key, value] : m_Callbacks)
            value(e);
    }

    ListenerID EventDispatchers::AddListener(EventType eventType, EventCallbackFn callback)
    {
        //if (m_Dispatchers.find(eventType) == m_Dispatchers.end())
        //{
        //    m_Dispatchers.emplace(eventType, EventDispatcher());
        //}
        auto id = m_Dispatchers[eventType].AddListener(callback);
        return ListenerID(eventType, id);
    }

    bool EventDispatchers::RemoveListener(ListenerID listenerID)
    {
        if (m_Dispatchers.find(listenerID.first) == m_Dispatchers.end())
            return false;
        return m_Dispatchers[listenerID.first].RemoveListener(listenerID.second);
    }

    void EventDispatchers::RemoveAllListeners()
    {
        m_Dispatchers.clear();
    }

    void EventDispatchers::Dispatch(Event& e)
    {
        if (m_Dispatchers.find(e.GetEventType()) != m_Dispatchers.end())
        {
            m_Dispatchers[e.GetEventType()].Dispatch(e);
        }
    }
}
