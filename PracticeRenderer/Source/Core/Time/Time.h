#pragma once

namespace PR
{
	class Time
	{
	public:
		Time();
		static Time& Get() { return *s_Instance; }
		static float DletaTime() { return s_Instance ? s_Instance->m_DletaTime : 0.0; }

		void Init(double startTime);
		void Upadte(double curTime);
		double GetDletaTime() const { return m_DletaTime; }
		
	private:
		static Time* s_Instance;

		double m_StartTime;
		double m_LastFrameTime;

		float m_DletaTime;
	};

}