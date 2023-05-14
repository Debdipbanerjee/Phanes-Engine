#pragma once

#include <stdint.h>

namespace Engine
{
	namespace Timing
	{
		typedef uint64_t	tick_t;

		
		tick_t GetCurrentTickCounter();
		tick_t GetTimeDiff_ms(tick_t i_StartTime, tick_t i_EndTime = GetCurrentTickCounter());

		void Init();

		inline float CvtSecondsToMilliseconds(float i_seconds)
		{
			return  1000 * i_seconds;
		}

		inline float CvtPerSecondToPerMillisecond(float i_PerSecond)
		{
			return i_PerSecond / 1000;
		}

	}
}