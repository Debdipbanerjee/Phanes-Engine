#pragma once

#include <vector>

#include "Math/Vector2D.h"
#include "GameObject/GameObject.h"
#include "nlohmann/json.hpp"
#include "Pointers/Smart_Ptr.h"
#include "Pointers/Weak_Ptr.h"
#include "Math/Point2D.h"
#include "Logging/Logging.h"

namespace Engine
{
	namespace Physics
	{
		class PhysicsComponent
		{
		public:

			Engine::Pointer::WeakPtr<GameObject> Go;

			float Mass = 0.0f;
			float DragCF = 0.1f;
			Engine::Math::Vector2D Force = { 0,0 };
			Engine::Math::Vector2D Acceleration = { 0,0 };
			Engine::Math::Vector2D Drag = { 0 , 0 };

			static std::vector<PhysicsComponent*> PhysicsList;
			bool IsPhysical = true;

			//static void AddPhysicsComponent(Engine::Pointer::SmartPtr<GameObject> gameobject, float mass);
			static void AddPhysicsComponent(Engine::Pointer::SmartPtr<GameObject> gameobject, nlohmann::json& physics);
			static PhysicsComponent* GetPhysicsComponent(Engine::Pointer::SmartPtr<GameObject> gameobject);
			static PhysicsComponent* GetPhysicsComponentByName(std::string GoName);
			static void Update(float dt);
		};
	}
	
}