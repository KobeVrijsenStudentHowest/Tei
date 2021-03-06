#include "teipch.h"
#include "StaticEngineResources.h"

#include <span>

#include <SDL.h>
#include <SDL_image.h>

#include <tei/render.h>
#include <tei/resource.h>

tei::internal::resource::Texture tei::internal::extra::resources::EngineLoaderBackground()
{
	METRICS_TIMEBLOCK;

	static int w{}, h{};
	static std::unique_ptr<SDL_Texture, decltype(
		[] (SDL_Texture* p) 
		{
			SDL_DestroyTexture(p);
		}
	)> texture{};
	if (!texture)
	{
		//auto buf = SDL_RWFromConstMem(std::data(data), int(std::size(data)));
		if (std::filesystem::exists("../tewi_w.png"))
		{
			auto surface = IMG_Load("../tewi_w.png");
			texture.reset(SDL_CreateTextureFromSurface(static_cast<SDL_Renderer*>(render::Renderer->GetRenderTraget().pData), surface));
			SDL_QueryTexture(texture.get(), nullptr, nullptr, &w, &h);
		}
	}
	return resource::Texture{ texture.get(), { w, h }  };
}


