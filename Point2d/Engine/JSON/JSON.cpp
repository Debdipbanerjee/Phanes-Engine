#include "JSON.h"

Engine::Pointer::SmartPtr<Engine::GameObject> Engine::JSON::CreateGameObject(const std::string JSONFileName)
{
	Engine::Pointer::SmartPtr<GameObject> NewObject;

	std::vector<uint8_t> Contents = Engine::File::LoadFile(JSONFileName);

	if (!Contents.empty())
	{
		nlohmann::json JSON = nlohmann::json::parse(Contents);

		NewObject = CreateGameObjectFromJSON(JSON);
		Engine::Log::I("new Gameobject created");
		return NewObject;
	}

	return Engine::Pointer::SmartPtr<GameObject>();
}

Engine::Pointer::SmartPtr<Engine::GameObject> Engine::JSON::CreateGameObjectFromJSON(nlohmann::json JSONData)
{
	GLib::Point2D GoPosition{ 0.0,0.0 };
	Engine::Math::Vector2D GoVelocity = Engine::Math::Vector2D(0.0, 0.0);
	std::string GoName = "Unknown";
	GLib::Sprite* GoSprite = nullptr;

	if (JSONData.contains("position"))
	{
		nlohmann::json Position = JSONData["position"];
		assert(Position.is_array());
		assert(Position.size() == 2);

		GoPosition = GLib::Point2D{ Position[0], Position[1] };
	}

	if (JSONData.contains("velocity"))
	{
		nlohmann::json Velocity = JSONData["velocity"];
		assert(Velocity.is_array());
		assert(Velocity.size() == 2);

		GoVelocity = Engine::Math::Vector2D(Velocity[0], Velocity[1]);
	}

	if (JSONData.contains("name"))
	{
		nlohmann::json Name = JSONData["name"];
		assert(Name.is_string());
		GoName = Name;
	}


	Engine::Pointer::SmartPtr<GameObject> NewObject (new GameObject(GoName, GoPosition, GoVelocity));

	if (JSONData.contains("components"))
	{
		assert(JSONData["components"].is_object());

		for (nlohmann::json::iterator it = JSONData["components"].begin(); it != JSONData["components"].end(); ++it)
		{
			
			if (it.key() == "physics")
			{
				nlohmann::json physics = it.value();

				Engine::Physics::PhysicsComponent::AddPhysicsComponent(NewObject, physics);
			}

			if (it.key() == "render")
			{
				nlohmann::json render = it.value();

				Engine::Renderer::RenderComponent::AddRenderComponent(NewObject, render);
				Engine::Log::I("new sprite created");
			}

			if (it.key() == "collision")
			{
				nlohmann::json collision = it.value();

				Engine::Collision::CollisionComponent::AddCollisionComponent(NewObject, collision);
				Engine::Log::I("new collision created");
			}
		}
	}

	return NewObject;
}
