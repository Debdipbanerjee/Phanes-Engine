#include "Rendering.h"

std::vector<Engine::Renderer::RenderComponent*> Engine::Renderer::RenderComponent::RenderList = std::vector<RenderComponent*>();

void Engine::Renderer::RenderComponent::AddRenderComponent(Engine::Pointer::SmartPtr<GameObject> gameobject, nlohmann::json& render)
{
	RenderComponent* RC = new RenderComponent();

	RC->Go = gameobject.operator->();
	
	nlohmann::json Render = render;

	if (Render.contains("sprite"))
	{
		std::string sName;
		Render["sprite"].get_to(sName);
		
		RC->Sprite = RC->CreateSprite(sName.c_str());
	}

	RenderList.push_back(RC);
}

Engine::Renderer::RenderComponent* Engine::Renderer::RenderComponent::GetRenderComponent(Engine::Pointer::SmartPtr<GameObject> gameobject)
{
	for (int i = 0; i < RenderList.size(); i++)
	{
		Engine::Pointer::SmartPtr<GameObject> SmartGo = Engine::Pointer::SmartPtr<GameObject>(RenderList[i]->Go);
		if (SmartGo == gameobject)
		{
			return RenderList[i];
		}
	}
}

GLib::Sprite* Engine::Renderer::RenderComponent::CreateSprite(const char* i_pFilename)
{

	assert(i_pFilename);

	size_t sizeTextureFile = 0;

	// Load the source file (texture data)
	void* pTextureFile = LoadSprite(i_pFilename, sizeTextureFile);

	// Ask GLib to create a texture out of the data (assuming it was loaded successfully)
	GLib::Texture* pTexture = pTextureFile ? GLib::CreateTexture(pTextureFile, sizeTextureFile) : nullptr;

	// exit if something didn't work
	// probably need some debug logging in here!!!!
	if (pTextureFile)
		delete[] pTextureFile;

	if (pTexture == nullptr)
		return nullptr;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int depth = 0;

	// Get the dimensions of the texture. We'll use this to determine how big it is on screen
	bool result = GLib::GetDimensions(*pTexture, width, height, depth);
	assert(result == true);
	assert((width > 0) && (height > 0));

	// Define the sprite edges
	GLib::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
	GLib::SpriteUVs	UVs = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } };
	GLib::RGBA							Color = { 255, 255, 255, 255 };

	// Create the sprite
	GLib::Sprite* pSprite = GLib::CreateSprite(Edges, 0.1f, Color, UVs, pTexture);

	// release our reference on the Texture
	GLib::Release(pTexture);

	return pSprite;
}

void* Engine::Renderer::RenderComponent::LoadSprite(const char* i_pFilename, size_t& o_sizeFile)
{
	assert(i_pFilename != NULL);

	FILE* pFile = NULL;

	errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
	if (fopenError != 0)
		return NULL;

	assert(pFile != NULL);

	int FileIOError = fseek(pFile, 0, SEEK_END);
	assert(FileIOError == 0);

	long FileSize = ftell(pFile);
	assert(FileSize >= 0);

	FileIOError = fseek(pFile, 0, SEEK_SET);
	assert(FileIOError == 0);

	uint8_t* pBuffer = new uint8_t[FileSize];
	assert(pBuffer);

	size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
	assert(FileRead == FileSize);

	fclose(pFile);

	o_sizeFile = FileSize;

	return pBuffer;

	return nullptr;
}

void Engine::Renderer::RenderComponent::Render()
 {
	 // Tell GLib to render this sprite at our calculated location
	 for (int i = 0; i < RenderList.size(); i++)
	 {
		 if (RenderList[i]->Go.IsValid() && RenderList[i]->IsRenderable)
		 {
			 GLib::Render(*RenderList[i]->Sprite, RenderList[i]->Go->Position, 0.0f, 0.0f);
		 }
	 }
 }

 void Engine::Renderer::RenderComponent::BeginRendering()
 {
	 // IMPORTANT: Tell GLib that we want to start rendering
	 GLib::BeginRendering(DirectX::Colors::White);
	 // Tell GLib that we want to render some sprites
	 GLib::Sprites::BeginRendering();
 }

 void Engine::Renderer::RenderComponent::EndRendering()
 {
	 // Tell GLib we're done rendering sprites
	 GLib::Sprites::EndRendering();
	 // IMPORTANT: Tell GLib we're done rendering
	 GLib::EndRendering();
 }


