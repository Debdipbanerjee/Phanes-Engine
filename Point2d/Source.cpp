#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

#include <DirectXColors.h>

#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

#include "Engine/Application/Application.h"
#include "Engine/Containers/Containers.h"
#include "Engine/GameObject/GameObject.h"
#include "Engine/Pointers/Smart_Ptr.h"
#include "Engine/Physics/Physics.h"
#include "Engine/File/LoadFile.h"
#include "Engine/JSON/JSON.h"
#include "Engine/Job/JobSystem/JobSystem.h"
#include "Engine/Rendering/Rendering.h"
#include "Engine/Collision/Collision.h"
#include "Math/Vector2D.h"
#include "Logging/Logging.h"


unsigned int KeyID;
bool KeyState;

void TestKeyCallback(unsigned int i_VKeyID, bool bWentDown)
{
#ifdef _DEBUG
	const size_t	lenBuffer = 65;
	char			Buffer[lenBuffer];

	sprintf_s(Buffer, lenBuffer, "VKey 0x%04x went %s\n", i_VKeyID, bWentDown ? "down" : "up");

	if(bWentDown == true)
	{
		KeyID = i_VKeyID;
		KeyState = bWentDown;
	}
	else
	{
		KeyState = bWentDown;
	}

	OutputDebugStringA(Buffer);
#endif // __DEBUG
}

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	// IMPORTANT: first we need to initialize GLib
	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "GLibTest", -1, 800, 600, true);
	Engine::Timing::Init();
	Engine::JobSystem::Init();

	
	if (bSuccess)
	{
		float forceVal = 0.9f;
		bool bQuit = false;

		// IMPORTANT (if we want keypress info from GLib): Set a callback for notification of key presses
		GLib::SetKeyStateChangeCallback(TestKeyCallback);

		//Player
		Engine::Pointer::SmartPtr<Engine::GameObject> Player;
		//Enemies
		Engine::Pointer::SmartPtr<Engine::GameObject> Enemy1;
		Engine::Pointer::SmartPtr<Engine::GameObject> Enemy2;
		Engine::Pointer::SmartPtr<Engine::GameObject> Enemy3;

		//UI
		Engine::Pointer::SmartPtr<Engine::GameObject> GameOver;
		Engine::Pointer::SmartPtr<Engine::GameObject> Win;

		//Job System
		{
			Engine::JobSystem::JobStatus JobStatus;

			Engine::JobSystem::RunJob(
				Engine::JobSystem::GetDefaultQueueName(),
				[&Player, &Enemy1, &Enemy2, &Enemy3, &GameOver, &Win]()
				{
					Player = Engine::JSON::CreateGameObject("data\\Temp.json");
					Enemy1 = Engine::JSON::CreateGameObject("data\\Enemy.json");
					Enemy2 = Engine::JSON::CreateGameObject("data\\Enemy2.json");
					Enemy3 = Engine::JSON::CreateGameObject("data\\Enemy3.json");
					GameOver = Engine::JSON::CreateGameObject("data\\GameOver.json");
					Win = Engine::JSON::CreateGameObject("data\\Win.json");
				},
				&JobStatus
					);

			JobStatus.WaitForZeroJobsLeft();
		}

		Engine::Containers::MasterGameObjectList.push_back(Player);
		Engine::Containers::MasterGameObjectList.push_back(Enemy1);
		Engine::Containers::MasterGameObjectList.push_back(Enemy2);
		Engine::Containers::MasterGameObjectList.push_back(Enemy3);
		Engine::Containers::MasterGameObjectList.push_back(GameOver);
		Engine::Containers::MasterGameObjectList.push_back(Win);


		// Collision Components
		Engine::Collision::CollisionComponent* e1_CollisionComp = Engine::Collision::CollisionComponent::GetCollisionComponent(Enemy1);
		Engine::Collision::CollisionComponent* e2_CollisionComp = Engine::Collision::CollisionComponent::GetCollisionComponent(Enemy2);
		Engine::Collision::CollisionComponent* e3_CollisionComp = Engine::Collision::CollisionComponent::GetCollisionComponent(Enemy3);
		Engine::Collision::CollisionComponent* p_CollisionComp = Engine::Collision::CollisionComponent::GetCollisionComponent(Player);

		// Physics components
		Engine::Physics::PhysicsComponent* P_PhysicsComp = Engine::Physics::PhysicsComponent::GetPhysicsComponent(Player);
		Engine::Physics::PhysicsComponent* e1_PhysicsComp = Engine::Physics::PhysicsComponent::GetPhysicsComponent(Enemy1);
		Engine::Physics::PhysicsComponent* e2_PhysicsComp = Engine::Physics::PhysicsComponent::GetPhysicsComponent(Enemy2);
		Engine::Physics::PhysicsComponent* e3_PhysicsComp = Engine::Physics::PhysicsComponent::GetPhysicsComponent(Enemy3);

		// Rendering Components
		Engine::Renderer::RenderComponent* p_RenderComp = Engine::Renderer::RenderComponent::GetRenderComponent(Player);
		Engine::Renderer::RenderComponent* e1_RenderComp = Engine::Renderer::RenderComponent::GetRenderComponent(Enemy1);
		Engine::Renderer::RenderComponent* e2_RenderComp = Engine::Renderer::RenderComponent::GetRenderComponent(Enemy2);
		Engine::Renderer::RenderComponent* e3_RenderComp = Engine::Renderer::RenderComponent::GetRenderComponent(Enemy3);
		Engine::Renderer::RenderComponent* gameover_RenderComp = Engine::Renderer::RenderComponent::GetRenderComponent(GameOver);
		gameover_RenderComp->IsRenderable = false;
		Engine::Renderer::RenderComponent* win_RenderComp = Engine::Renderer::RenderComponent::GetRenderComponent(Win);
		win_RenderComp->IsRenderable = false;

		// Collision Handles
		std::function<void(Engine::Collision::CollisionComponent* other)> EnemyCollisionEventHandle = [](Engine::Collision::CollisionComponent* other)
		{
			Engine::Pointer::SmartPtr<Engine::GameObject> otherObject(other->WeakGo);
			if (otherObject->Name == "temp")
			{
				Engine::Log::I("Collision detected");
			}
		};

		std::function<void(Engine::Collision::CollisionComponent* other)> PlayerCollisionEventHandle = [ e2_CollisionComp, e2_PhysicsComp, e2_RenderComp, p_CollisionComp, P_PhysicsComp, p_RenderComp,
			gameover_RenderComp, win_RenderComp](Engine::Collision::CollisionComponent* other)
		{
			Engine::Pointer::SmartPtr<Engine::GameObject> otherObject(other->WeakGo);
			if (otherObject->Name == "enemy1")
			{
				Engine::Log::I("Collision detected enemy1");
				P_PhysicsComp->IsPhysical = false;
				p_CollisionComp->IsCollidable = false;
				p_RenderComp->IsRenderable = false;
				
				gameover_RenderComp->IsRenderable = true;
			}

			if (otherObject->Name == "enemy2")
			{
				Engine::Log::I("Collision detected enemy2");
				e2_CollisionComp->IsCollidable = false;
				e2_PhysicsComp->IsPhysical = false;
				e2_RenderComp->IsRenderable = false;
				P_PhysicsComp->IsPhysical = false;
				p_CollisionComp->IsCollidable = false;
				p_RenderComp->IsRenderable = false;
				win_RenderComp->IsRenderable = true;
			}

			if (otherObject->Name == "enemy3")
			{
				Engine::Log::I("Collision detected enemy3");
				P_PhysicsComp->IsPhysical = false;
				p_CollisionComp->IsCollidable = false;
				p_RenderComp->IsRenderable = false;
				gameover_RenderComp->IsRenderable = true;
			}
		};

		// Binding
		e1_CollisionComp->AddCollisionEvent(EnemyCollisionEventHandle);
		e2_CollisionComp->AddCollisionEvent(EnemyCollisionEventHandle);
		e3_CollisionComp->AddCollisionEvent(EnemyCollisionEventHandle);
		p_CollisionComp->AddCollisionEvent(PlayerCollisionEventHandle);
		

		do
		{
			// IMPORTANT: We need to let GLib do it's thing. 
			GLib::Service(bQuit);

			Engine::Renderer::RenderComponent::BeginRendering();

			if (!bQuit)
			{
				float dt = Engine::Application::GetFrameTime();

					//A
					if (KeyID == 0x0041 && KeyState)
					{
						P_PhysicsComp->Force = Engine::Math::Vector2D (-forceVal, 0);
					}
					// D
					if (KeyID == 0x0044 && KeyState)
					{
						P_PhysicsComp->Force = Engine::Math::Vector2D (forceVal, 0);
					}
					// W
					if (KeyID == 0x0057 && KeyState)
					{
						P_PhysicsComp->Force = Engine::Math::Vector2D (0, forceVal);
					}
					// S
					if (KeyID == 0x0053 && KeyState)
					{
						P_PhysicsComp->Force = Engine::Math::Vector2D (0, -forceVal);
					}

					Engine::Physics::PhysicsComponent::Update(dt);
					Engine::Collision::CollisionComponent::Update(dt);
					Engine::Renderer::RenderComponent::Render();
					


					Engine::Renderer::RenderComponent::EndRendering();
			}
		} while (bQuit == false);


		Engine::JobSystem::RequestShutdown();
		Engine::Application::Shutdown();
	}

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

}

