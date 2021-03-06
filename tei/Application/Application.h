#pragma once

#include <vector>
#include <utility>
#include <string>
#include <span>

#include <tei/utility.h>
#include <tei/unit.h>

namespace tei::internal::application
{

	enum struct WindowProperty
	{
		BORDERED,
		UNBORDERED,

		RESIZABLE,
		FIXED,

		FULLSCREEN,
		FULLSCREEN_FAKE,
		MAXIMISED,
		MINIMISED,
		RESTORED,
		ALWAYS_IN_FRONT,
	};

	enum struct WindowPropertyChangedEvent {};

	class Application
	{

		explicit Application(int, char const* const*);

	public:

		friend Application& StartApplication(int, char const* const*);

		~Application();

		void Update();
		void UpdateProps(bool changed);

		inline auto const& GetWindow() const
		{ return m_Window; }

		void SetWindowTitle(std::string const&);
		void SetWindowProperty(unit::Scale);
		void SetWindowProperty(unit::Position);
		void SetWindowProperty(unit::Transform);
		void SetWindowProperty(WindowProperty);

		inline std::span<std::string_view const> Args() const
		{ return m_Args; }

		void Quit() const;

		struct Window
		{
			void* pData;
			unit::Transform transform;
		};

	private:

		void OpenWindow();
		void CloseWindow();

		std::vector<std::string_view> const m_Args;
		Window m_Window;

	public:

		using Service = utility::Service<Application>;

	};

	Application& StartApplication(int argc = {}, char const* const* argv = {});
	void StopApplication();

	extern Application::Service ApplicationService;

}

namespace tei::external
{
	namespace application
	{
		using tei::internal::application::WindowProperty;
		using tei::internal::application::WindowPropertyChangedEvent;
	}

	static constexpr auto const& Application { tei::internal::application::ApplicationService };

}

