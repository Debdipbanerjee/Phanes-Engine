#pragma once

#include <string>

#include "GLib.h"
#include "Math/Vector2D.h"
#include "Logging/Logging.h"


namespace Engine
{
	class GameObject
	{

	private:


	public:

		std::string Name;
		Engine::Math::Vector2D Velocity;
		GLib::Point2D Position;

		GameObject();
		~GameObject();
		GameObject(std::string name, GLib::Point2D position, Engine::Math::Vector2D velocity)
		{
			this->Name = name;
			this->Position = position;
			this->Velocity = velocity;
	}
	};
}