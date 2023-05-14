#include "Physics.h"

std::vector<Engine::Physics::PhysicsComponent*> Engine::Physics::PhysicsComponent::PhysicsList = std::vector< PhysicsComponent*>();

//void Engine::Physics::PhysicsComponent::AddPhysicsComponent(Engine::Pointer::SmartPtr<GameObject> gameobject, float mass)
//{
//
//	PhysicsComponent* PC = new PhysicsComponent();
//
//	PC->Go = gameobject.operator->();
//	PC->Mass = mass;
//
//	PhysicsList.push_back(PC);
//}

void Engine::Physics::PhysicsComponent::AddPhysicsComponent(Engine::Pointer::SmartPtr<GameObject> gameobject, nlohmann::json& physics)
{

	PhysicsComponent* PC = new PhysicsComponent();

	PC->Go = gameobject.operator->();

	nlohmann::json Physics = physics;

	if (Physics.contains("mass"))
	{
		PC->Mass = Physics["mass"];
		Engine::Log::I("mass = " + std::to_string(PC->Mass));
		
	}

	PhysicsList.push_back(PC);
}

Engine::Physics::PhysicsComponent* Engine::Physics::PhysicsComponent::GetPhysicsComponent(Engine::Pointer::SmartPtr<GameObject> gameobject)
{
	for (int i = 0; i < PhysicsList.size(); i++)
	{
		if (PhysicsList[i]->Go.IsValid() && PhysicsList[i]->IsPhysical)
		{
			if (PhysicsList[i]->Go.operator->() == gameobject.operator->())
			{
				return PhysicsList[i];
			}
		}
	}

	return nullptr;
}

Engine::Physics::PhysicsComponent* Engine::Physics::PhysicsComponent::GetPhysicsComponentByName(std::string GoName)
{
	for (int i = 0; i < PhysicsList.size(); i++)
	{
		if (PhysicsList[i]->Go.IsValid())
		{
			if (PhysicsList[i]->Go.operator->()->Name == GoName)
			{
				return PhysicsList[i];
			}
		}
	}

	return nullptr;
}




void Engine::Physics::PhysicsComponent::Update(float dt)
{
	//Euler Method
	for (int i = 0; i < PhysicsList.size(); i++)
	{
		//Drag
		PhysicsList[i]->Drag.x = -PhysicsList[i]->DragCF * PhysicsList[i]->Go->Velocity.x;
		PhysicsList[i]->Drag.y = -PhysicsList[i]->DragCF * PhysicsList[i]->Go->Velocity.y;
		//Engine::Log::I("Drag = " + std::to_string(Drag));

		//Sum of Forces
		PhysicsList[i]->Force.x = PhysicsList[i]->Force.x + PhysicsList[i]->Drag.x;
		PhysicsList[i]->Force.y = PhysicsList[i]->Force.y + PhysicsList[i]->Drag.y;
		//Engine::Log::I("Force = " + std::to_string(PhysicsList[i]->Force.x) + " , " + std::to_string(PhysicsList[i]->Force.y));

		//Acceleration (f = ma)
		PhysicsList[i]->Acceleration.x = PhysicsList[i]->Force.x / PhysicsList[i]->Mass;
		PhysicsList[i]->Acceleration.y = PhysicsList[i]->Force.y / PhysicsList[i]->Mass;
		//Engine::Log::I("Acc= " + std::to_string(Acceleration));
		

		//Velocity v = v` + a * dt
		PhysicsList[i]->Go->Velocity.x = PhysicsList[i]->Go->Velocity.x + PhysicsList[i]->Acceleration.x * dt;
		PhysicsList[i]->Go->Velocity.y = PhysicsList[i]->Go->Velocity.y + PhysicsList[i]->Acceleration.y * dt;
		//Engine::Log::I("Vel = " + std::to_string(PhysicsList[i]->Go->velocity));
		
		//position
		PhysicsList[i]->Go->Position.x = PhysicsList[i]->Go->Position.x + PhysicsList[i]->Go->Velocity.x * dt;
		PhysicsList[i]->Go->Position.y = PhysicsList[i]->Go->Position.y + PhysicsList[i]->Go->Velocity.y * dt;

		PhysicsList[i]->Force = { 0,0 };
	}

}