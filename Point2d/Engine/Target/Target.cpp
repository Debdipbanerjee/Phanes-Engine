#include "Target.h"

bool Engine::Target::Windows::IsDebuggerPresent()
{

		return ::IsDebuggerPresent();
}
