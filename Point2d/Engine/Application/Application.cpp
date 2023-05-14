#include "Application.h"
#include "Target/Target.h"

float Engine::Application::GetFrameTime()
{
	static Engine::Timing::tick_t LastFrameStartTick = 0;

	float FrameTime;

	Engine::Timing::tick_t CurrentFrameStartTick = Engine::Timing::GetCurrentTickCounter();

	if (LastFrameStartTick == 0)
	{
		FrameTime = 1.0f / 60.0f;
	}
	else
	{
		FrameTime = Engine::Timing::GetTimeDiff_ms(CurrentFrameStartTick, LastFrameStartTick) / 1000.0f;
	}

	LastFrameStartTick = CurrentFrameStartTick;

	return Engine::Target::Windows::IsDebuggerPresent() ? (1.0f / 60.0f) : FrameTime;
}


void Engine::Application::Shutdown()
{
	// IMPORTANT:  Tell GLib to shutdown, releasing resources.
	GLib::Shutdown();
}