#include "DebugAudio.h"
#include <iostream>

namespace tei::internal::audio
{

	void DebugAudio::OnEnable()
	{
		puts("Debug audio enabled");
	}

	void DebugAudio::OnDisable()
	{
		puts("Debug audio disabled");
	}

	void DebugAudio::OnUpdate(std::span<std::reference_wrapper<resource::Sound const>> log)
	{
		for (resource::Sound const& sound : log)
			std::cout << "[audio] " << sound.name << '\n';
	}

	void DebugAudio::OnMute(bool muted)
	{
		puts(muted ? "Debug audio muted" : "Debug audio unmuted");
	}

}