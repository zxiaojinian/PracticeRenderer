#include "pch.h"
#include "Time.h"

namespace PR
{
	Time* Time::s_Instance = nullptr;

	Time::Time()
	{
		s_Instance = this;
	}

	void Time::Init(double startTime)
	{
		m_StartTime = startTime;
		m_LastFrameTime = startTime;
	}

	void Time::Upadte(double curTime)
	{
		m_DletaTime = static_cast<float>(curTime - m_LastFrameTime);
		m_LastFrameTime = curTime;
	}
}