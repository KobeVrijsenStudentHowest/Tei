#include "Audio.h"

#include <SDL_mixer.h>

using namespace tei::internal::resource;
using namespace tei::internal;
using namespace std::literals;

Resource<Sound> Load(ToLoad<Sound>, std::filesystem::path const& path, bool loop, float volume, [[maybe_unused]] std::string_view name)
{
    struct Audio
    {
        Audio()
        {
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
                throw utility::TeiRuntimeError{ "Could not open audio", SDL_GetError() };
        }
        ~Audio()
        {
            Mix_CloseAudio();
        }
    };
    static std::weak_ptr<Audio> s_Audio{};

    auto audio = s_Audio.lock();
    if (!audio)
        s_Audio = audio = std::make_shared<Audio>();

    auto const DELETER = [audio = std::move(audio)] (Sound* pSound)
    {
        Mix_FreeChunk(static_cast<Mix_Chunk*>(pSound->pData));
        delete pSound;
    };

    Mix_Chunk* pChunk = Mix_LoadWAV(path.string().c_str());
    if (pChunk == nullptr)
        throw utility::TeiRuntimeError{ "Sound chunk could not be loaded", SDL_GetError() };

#if defined(DEBUG) || defined(_DEBUG)
    std::string sname{
        name.empty()
        ? path.filename().string()
        : name
    };
#endif

    return { 
        std::shared_ptr<Sound>{ 
            new Sound{
                pChunk, 
                volume,
                loop
#if defined(DEBUG) || defined(_DEBUG)
                , std::move(sname) 
#endif
            },
            DELETER
        }
    };
}