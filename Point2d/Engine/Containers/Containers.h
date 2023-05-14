#pragma once

#include <vector>

#include "GameObject/GameObject.h"
#include "Pointers/Smart_Ptr.h"

namespace Engine
{
	namespace Containers
	{
		std::vector<Engine::Pointer::SmartPtr<Engine::GameObject>> MasterGameObjectList;
	}

}