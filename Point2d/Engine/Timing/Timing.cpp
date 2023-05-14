#include "Timing.h"

#include <Windows.h>
#include <assert.h>


namespace Engine
{
	namespace Timing
	{
		unsigned int LastFrame_ms;

		LARGE_INTEGER PerformanceFrequency = { 0 };

		void Init()
		{
			BOOL b_Success = QueryPerformanceFrequency(&PerformanceFrequency);
			assert(b_Success == TRUE);
		}


		tick_t GetCurrentTickCounter()
		{
			LARGE_INTEGER CurrentFrameCounter;

			BOOL success = QueryPerformanceFrequency(&CurrentFrameCounter);
			assert(success == TRUE);

			return CurrentFrameCounter.QuadPart;
		}

		tick_t GetTimeDiff_ms(tick_t i_StartTime, tick_t i_EndTime)
		{
			assert(PerformanceFrequency.QuadPart != 0);
			return (1000 * static_cast<float>(i_EndTime - i_StartTime)) / PerformanceFrequency.QuadPart;
		}

	}
}

