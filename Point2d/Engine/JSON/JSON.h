#pragma once

#include <string>
#include <vector>

#include "GLib.h"
#include <nlohmann/json.hpp>
#include "Math/Vector2D.h"
#include "GameObject/GameObject.h"
#include "Rendering/Rendering.h"
#include "Physics/Physics.h"
#include "Collision/Collision.h"
#include "Pointers/Smart_Ptr.h"
#include "File/LoadFile.h"
#include "Logging/Logging.h"

namespace Engine
{
	namespace JSON
	{
		Engine::Pointer::SmartPtr<GameObject> CreateGameObject(const std::string JSONFileName);
		Engine::Pointer::SmartPtr<GameObject>  CreateGameObjectFromJSON(nlohmann::json JSONData);
	}
}