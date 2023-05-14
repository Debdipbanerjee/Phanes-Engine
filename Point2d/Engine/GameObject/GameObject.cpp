#include "GameObject.h"

Engine::GameObject::GameObject()
{
}

Engine::GameObject::~GameObject()
{
	Engine::Log::I("GameObject : " + Name + " deleted.");
}
