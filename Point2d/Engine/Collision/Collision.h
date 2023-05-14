#pragma once

#include "nlohmann/json.hpp"
#include "Math/Point2D.h"
#include "Pointers/Weak_Ptr.h"
#include "Pointers/Smart_Ptr.h"
#include "GameObject/GameObject.h"
#include "Math/Matrix.h"
#include "Logging/Logging.h"

#include <vector>
#include <functional>

namespace Engine
{
	namespace Collision
	{
		class CollisionComponent
		{
		public:
			
			struct AABB
			{
				Engine::Math::Point2d centre;
				Engine::Math::Point2d extents;
			};
			AABB* aabb = nullptr;

			Engine::Pointer::WeakPtr<GameObject> WeakGo;
			static std::vector<CollisionComponent*> CollisionList;
			bool IsCollidable = true;

			CollisionComponent() {};
			CollisionComponent(Engine::Pointer::SmartPtr<GameObject>SmartGo) : WeakGo(SmartGo)
			{
				CollisionList.push_back(this);
				aabb = new AABB;
			};
			
			std::function<void(CollisionComponent* other)> CollisionHandle = nullptr;
			void AddCollisionEvent(std::function<void(CollisionComponent* other)> Event);
			static void Update(float dt);
			static bool checkCollision(float dt, Engine::Pointer::SmartPtr<GameObject> ObjA, Engine::Pointer::SmartPtr<GameObject> ObjB, CollisionComponent* ObjACol, CollisionComponent* ObjBCol);
			
			static void AddCollisionComponent(Engine::Pointer::SmartPtr<GameObject> gameobject, nlohmann::json& collision);
			static CollisionComponent* GetCollisionComponent(Engine::Pointer::SmartPtr<GameObject> gameobject);

			~CollisionComponent()
			{
				if (aabb)
				{
					delete aabb;
				}
			}

		private:

		};
	}
}