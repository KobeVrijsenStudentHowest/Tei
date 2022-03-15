
#include <tei.h>

#include <iostream>
#include <sstream>
#include <random>

using namespace tei::literals;

struct Timer {};

//void OnUpdate(Timer)
//{
//	if (tei::Time->frame.now > 5_s)
//		tei::Application->Quit();
//}

//void OnRender(Timer)
//{
//	std::cout << "  " << tei::Time->frame.now << '\r';
//}

//void OnUpdate(tei::components::TextureRenderComponent& t)
//{
//	get<tei::components::ObjectTransform>(t).local.rotation.r = std::sin(tei::Time->frame.now);
//}

struct FpsComponent : tei::components::RefComponent<"fps", tei::components::Observed<std::string>, tei::components::ObjectTransform>
{
	tei::Clock::duration average{ 1_s };
};

void OnUpdate(FpsComponent& fps)
{
	auto& [text, transform] = fps.refs;

	transform.local.position = tei::Application->GetWindow().transform.scale / -2.f + 16.f;

	std::stringstream out{};
	out << "[Frame]\tdelta:\t" << std::chrono::milliseconds{tei::Time->thread->delta}
		<< "\tlag:\t" << std::chrono::microseconds{tei::Time->thread->lag}
		<< std::endl;
	std::cout << out.view();
	std::cout.flush();

	fps.average = (fps.average * 9 + tei::Time->frame.delta * 1) / 10;
	auto ntext = std::to_string(int(1 / fps.average + .25f));
	if (*text != ntext)
		text = std::move(ntext);
}

void OnFixedUpdate(FpsComponent const&)
{
	std::stringstream out{};
	out << "[Fixed]\tdelta:\t" << std::chrono::milliseconds{tei::Time->thread->delta}
		<< "\tlag:\t" << std::chrono::microseconds{tei::Time->thread->lag}
		<< std::endl;
	std::cout << out.view();
	std::cout.flush();
}

struct RootComponent : tei::components::RefComponent<"root", tei::components::ObjectTransform>
{};

void OnFixedUpdate(RootComponent& comp)
{
	auto& [transform] = comp.refs;
	float scale{ 1 + std::sin(tei::Time->thread->now.count()) / 5 };
	transform.local.rotation.r = scale - 1;
	transform.local.scale.x = scale;
	transform.local.scale.y = scale;
}

struct SplashText : tei::components::RefComponent<"splash", tei::components::Observed<std::string>>
{};

void OnUpdate(SplashText& comp)
{
	auto& [text] = comp.refs;
	text.get() = (std::stringstream{} << "0x" << std::hex << tei::RNG()).view();
}

void TeiClientInit()
{
	using namespace tei::components;
	using namespace tei::resource;
	using namespace tei::unit;
	
	//tei::Application->SetWindowProperty(tei::application::WindowProperty::FULLSCREEN_FAKE);

	auto& scene{ tei::Scenes->AddScene(true) };

	scene.AddComponent<Timer>();

// Excersise week 1
#if true
	{

		auto& root = scene.AddChild();

		Scale windowScale{};

		root.AddComponent<ObjectTransform>(Scale{ 2 });
		root.AddComponent<RootComponent>();

		{
			auto& bg = root.AddChild();

			auto& bgTex = bg.AddComponent(tei::Resources->LoadShared<Texture>("resources/background.jpg"));
			
			windowScale = bgTex->size;
			tei::Application->SetWindowProperty(Scale{ windowScale * 2.f });

			bg.AddComponent<ObjectTransform>();
			bg.AddComponent<TextureRenderComponent>();

			{
				auto& bglogo = bg.AddChild();

				bglogo.AddComponent(tei::Resources->LoadShared<Texture>("resources/logo.png"));
				bglogo.AddComponent<ObjectTransform>();
				bglogo.AddComponent<TextureRenderComponent>();
			}
		}

		{
			auto& title = root.AddChild();

			title.AddComponent<tei::components::Observed<std::string>>({ "Splash screen text" });
			title.AddComponent(tei::Resources->LoadUnique<Font>("resources/Lingua.otf", 32));
			title.AddComponent(tei::Resources->LoadUnique<Texture>());
			title.AddComponent<ObjectTransform>(Position{ 0, -windowScale.y / 2 / 5 * 3 });
			title.AddComponent<SplashText>();
			title.AddComponent<TextRenderComponent>();
		}

		{
			auto& fps = scene.AddChild();

			fps.AddComponent<tei::components::Observed<std::string>>();
			fps.AddComponent(tei::Resources->LoadUnique<Font>("resources/Lingua.otf", 11));
			fps.AddComponent(tei::Resources->LoadUnique<Texture>());
			fps.AddComponent<ObjectTransform>(Position{ windowScale / -2.f + 12.f });
			fps.AddComponent<FpsComponent>();
			fps.AddComponent<TextRenderComponent>();
		}

#endif

	}

	tei::Input->AddCommand(
		tei::input::KeyboardInput::Main::ESCAPE,
		[]
		{ 
			tei::Application->Quit(); 
		}
	);

}

void TeiClientCleanup()
{

}