#include "Core.h"

#include <thread>

#include <tei/time.h>
#include <tei/scene.h>
#include <tei/audio.h>
#include <tei/render.h>

#include "../External/Client.h"

#include <SDL.h>

using namespace tei::internal::core;

using namespace tei::internal;
using namespace tei::internal::time;
using namespace tei::internal::time::literals;

utility::Static<CoreFunction> tei::internal::core::Core{};

void CoreFunction::GameLoop()
{
	auto& frame = Time->frame;
	auto current = frame.now = Clock::now();
	while (m_IsRunning)
	{
		METRICS_TIMEBLOCK;

		Time->global.now = current = Clock::now();
		if (frame.fixed)
		{
			frame.lag = current - frame.now;
			frame.now += frame.delta = frame.step;

			FrameUpdate();

			if (m_IsRunning && frame.lag < frame.step)
				std::this_thread::sleep_until(frame.now);
		}
		else
		{
			frame.delta = current - frame.now;
			frame.now = current;
			frame.lag = {};

			FrameUpdate();

			//if (m_IsRunning && frame.synced)
			/* sync */
		}
	}
}

void CoreFunction::FrameUpdate()
{
	METRICS_TIMEBLOCK;

	// Update
	{

		SDL_Event e{};
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				Stop();
		}

		//events::Events->Update();

		if (!m_IsRunning) return;

		scene::Scene->Do(ecs::Message::UPDATE);

		if (!m_IsRunning) return;

		audio::Audio->Update();
		render::Renderer->Update();
	}

	// Render
	{
		render::Renderer->Clear();
		scene::Scene->Do(ecs::Message::RENDER);
		render::Renderer->Present();
	}

}

void CoreFunction::Run()
{
	METRICS_TIMEBLOCK;

	// Services
	audio::Audio.Register(new audio::SDLAudio{});
	//events::Events.Register(new events::EventManager{});
	resource::Resources.Register(new resource::ResourceManager{});
	scene::Scene.Register(new ecs::Object{ nullptr, true });

	// Resource loaders
	resource::Resources->AddLoader(new resource::prefab::TextureLoader{});
	resource::Resources->AddLoader(new resource::prefab::FontsLoader{});
	resource::Resources->AddLoader(new resource::prefab::AudioLoader{});

	// Client init
	TeiClientInit();

	// Game
	GameLoop();

	// Clear scenes
	scene::Scene.Register(nullptr);

	// Client quit
	TeiClientCleanup();

	// Clear services
	resource::Resources.Register(nullptr);
	//events::Events.Register(nullptr);
	audio::Audio.Register(nullptr);

	// End
}

void CoreFunction::Stop()
{
	m_IsRunning = false;
}