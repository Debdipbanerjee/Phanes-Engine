#pragma once

#include <vector>
#include <DirectXColors.h>

#include "GLib.h"
#include "GameObject/GameObject.h"
#include "nlohmann/json.hpp"
#include "Pointers/Smart_Ptr.h"
#include "Pointers/Weak_Ptr.h"



namespace Engine
{
	namespace Renderer 
	{
		class RenderComponent
		{

		private:

		public:

			Engine::Pointer::WeakPtr<GameObject> Go ;
			GLib::Sprite* Sprite = nullptr;
			bool IsRenderable = true;
			static std::vector<RenderComponent*> RenderList;

			static void AddRenderComponent(Engine::Pointer::SmartPtr<GameObject> gameobject, nlohmann::json& render);
			static RenderComponent* GetRenderComponent(Engine::Pointer::SmartPtr<GameObject> gameobject);
			static  GLib::Sprite* CreateSprite(const char* i_pFilename);
			static void* LoadSprite(const char* i_pFilename, size_t& o_sizeFile);
			static void Render();
			static void BeginRendering();
			static void EndRendering();
		};
	}
	
}