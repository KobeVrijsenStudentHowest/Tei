#include "Textures.h"

#include <tei/render.h>
#include <SDL_image.h>
#include "Textures.h"

using namespace tei::internal::resource;
using namespace tei::internal;

//prefab::TextureLoader::TextureLoader()
//{
//	METRICS_TIMEBLOCK;
//
//	int init{
//		bool(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) &
//		bool(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) 
//	};
//	if (!init)
//		throw utility::TeiRuntimeError{ "Could not properly initialize SDL_image", SDL_GetError() };
//}
//
//Texture* prefab::TextureLoader::OnLoad(std::filesystem::path const& path, ResourceManager const& resources, std::optional<Texture> init) const
//{
//	METRICS_TIMEBLOCK;
//
//	auto const file = resources.GetExisting(path);
//
//	auto const pathStr{ file.string() };
//
//	if (!init) init = Texture{};
//
//	SDL_Texture* pTexture = IMG_LoadTexture(
//		static_cast<SDL_Renderer*>(render::Renderer->GetRenderTraget().pData),
//		pathStr.c_str()
//	);
//	init->pData = pTexture;
//
//	if (pTexture == nullptr ||
//		SDL_QueryTexture(
//			pTexture, 
//			nullptr,
//			nullptr, 
//			&init->w, 
//			&init->h
//		) != 0
//	)
//		throw utility::TeiRuntimeError{ "Could not load texture: " + pathStr, SDL_GetError() };
//
//	return new Texture{ *std::move(init) };
//}
//
//void prefab::TextureLoader::OnFree(ResourceManager const&, Texture* texture) const
//{
//	METRICS_TIMEBLOCK;
//
//	SDL_DestroyTexture(static_cast<SDL_Texture*>(texture->pData));
//	delete texture;
//}

Texture LoadTexture(std::filesystem::path const& path)
{
	static bool init{
		bool(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) &&
		bool(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) ||
		(throw utility::TeiRuntimeError{ "Could not properly initialize SDL_image", SDL_GetError() }, false)
	};

	SDL_Texture* pTexture = IMG_LoadTexture(
		static_cast<SDL_Renderer*>(render::Renderer->GetRenderTraget().pData),
		path.string().c_str()
	);

	Texture texture{};
	texture.pData = pTexture;

	if (pTexture == nullptr ||
		SDL_QueryTexture(
			pTexture,
			nullptr, nullptr,
			&texture.w, &texture.h
		) != 0
	)
	{
		throw utility::TeiRuntimeError{ "Could not load texture: " + path.string(), SDL_GetError() };
	}

	return texture;
}

static constexpr auto  DELETER = [] (auto* pTexture) // Texture or Sprite
{
	SDL_DestroyTexture(static_cast<SDL_Texture*>(pTexture->pData));
	delete pTexture;
};

Resource<Texture> Load(ToLoad<Texture>, std::filesystem::path const& path)
{
	return { std::shared_ptr<Texture>{ new Texture{ LoadTexture(path) }, DELETER } };
}

Resource<Sprite> Load(ToLoad<Sprite>, std::filesystem::path const& path, time::Clock::duration frameduration, int cols, int rows, bool loop, time::Clock::time_point origin)
{
	return { std::shared_ptr<Sprite>{ new Sprite{ LoadTexture(path), frameduration, cols, rows, loop, origin }, DELETER } };
}

