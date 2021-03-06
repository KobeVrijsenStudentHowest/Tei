#include "teipch.h"
#include "Keyboard.h"

#include <unordered_map>
#include <ranges>

#include <SDL.h>


using namespace tei::internal::input;

using enum DeviceId;
    
KeyboardInput::Main::Type const KeyboardInput::Main::A{ KEYBOARD, SDL_SCANCODE_A };
KeyboardInput::Main::Type const KeyboardInput::Main::B{ KEYBOARD, SDL_SCANCODE_B };
KeyboardInput::Main::Type const KeyboardInput::Main::C{ KEYBOARD, SDL_SCANCODE_C };
KeyboardInput::Main::Type const KeyboardInput::Main::D{ KEYBOARD, SDL_SCANCODE_D };
KeyboardInput::Main::Type const KeyboardInput::Main::E{ KEYBOARD, SDL_SCANCODE_E };
KeyboardInput::Main::Type const KeyboardInput::Main::F{ KEYBOARD, SDL_SCANCODE_F };
KeyboardInput::Main::Type const KeyboardInput::Main::G{ KEYBOARD, SDL_SCANCODE_G };
KeyboardInput::Main::Type const KeyboardInput::Main::H{ KEYBOARD, SDL_SCANCODE_H };
KeyboardInput::Main::Type const KeyboardInput::Main::I{ KEYBOARD, SDL_SCANCODE_I };
KeyboardInput::Main::Type const KeyboardInput::Main::J{ KEYBOARD, SDL_SCANCODE_J };
KeyboardInput::Main::Type const KeyboardInput::Main::K{ KEYBOARD, SDL_SCANCODE_K };
KeyboardInput::Main::Type const KeyboardInput::Main::L{ KEYBOARD, SDL_SCANCODE_L };
KeyboardInput::Main::Type const KeyboardInput::Main::M{ KEYBOARD, SDL_SCANCODE_M };
KeyboardInput::Main::Type const KeyboardInput::Main::N{ KEYBOARD, SDL_SCANCODE_N };
KeyboardInput::Main::Type const KeyboardInput::Main::O{ KEYBOARD, SDL_SCANCODE_O };
KeyboardInput::Main::Type const KeyboardInput::Main::P{ KEYBOARD, SDL_SCANCODE_P };
KeyboardInput::Main::Type const KeyboardInput::Main::Q{ KEYBOARD, SDL_SCANCODE_Q };
KeyboardInput::Main::Type const KeyboardInput::Main::R{ KEYBOARD, SDL_SCANCODE_R };
KeyboardInput::Main::Type const KeyboardInput::Main::S{ KEYBOARD, SDL_SCANCODE_S };
KeyboardInput::Main::Type const KeyboardInput::Main::T{ KEYBOARD, SDL_SCANCODE_T };
KeyboardInput::Main::Type const KeyboardInput::Main::U{ KEYBOARD, SDL_SCANCODE_U };
KeyboardInput::Main::Type const KeyboardInput::Main::V{ KEYBOARD, SDL_SCANCODE_V };
KeyboardInput::Main::Type const KeyboardInput::Main::W{ KEYBOARD, SDL_SCANCODE_W };
KeyboardInput::Main::Type const KeyboardInput::Main::X{ KEYBOARD, SDL_SCANCODE_X };
KeyboardInput::Main::Type const KeyboardInput::Main::Y{ KEYBOARD, SDL_SCANCODE_Y };
KeyboardInput::Main::Type const KeyboardInput::Main::Z{ KEYBOARD, SDL_SCANCODE_Z };
KeyboardInput::Main::Type const KeyboardInput::Main::NUM_1{ KEYBOARD, SDL_SCANCODE_1 };
KeyboardInput::Main::Type const KeyboardInput::Main::NUM_2{ KEYBOARD, SDL_SCANCODE_2 };
KeyboardInput::Main::Type const KeyboardInput::Main::NUM_3{ KEYBOARD, SDL_SCANCODE_3 };
KeyboardInput::Main::Type const KeyboardInput::Main::NUM_4{ KEYBOARD, SDL_SCANCODE_4 };
KeyboardInput::Main::Type const KeyboardInput::Main::NUM_5{ KEYBOARD, SDL_SCANCODE_5 };
KeyboardInput::Main::Type const KeyboardInput::Main::NUM_6{ KEYBOARD, SDL_SCANCODE_6 };
KeyboardInput::Main::Type const KeyboardInput::Main::NUM_7{ KEYBOARD, SDL_SCANCODE_7 };
KeyboardInput::Main::Type const KeyboardInput::Main::NUM_8{ KEYBOARD, SDL_SCANCODE_8 };
KeyboardInput::Main::Type const KeyboardInput::Main::NUM_9{ KEYBOARD, SDL_SCANCODE_9 };
KeyboardInput::Main::Type const KeyboardInput::Main::NUM_0{ KEYBOARD, SDL_SCANCODE_0 };
KeyboardInput::Main::Type const KeyboardInput::Main::RETURN{ KEYBOARD, SDL_SCANCODE_RETURN };
KeyboardInput::Main::Type const KeyboardInput::Main::ESCAPE{ KEYBOARD, SDL_SCANCODE_ESCAPE };
KeyboardInput::Main::Type const KeyboardInput::Main::BACKSPACE{ KEYBOARD, SDL_SCANCODE_BACKSPACE };
KeyboardInput::Main::Type const KeyboardInput::Main::TAB{ KEYBOARD, SDL_SCANCODE_TAB };
KeyboardInput::Main::Type const KeyboardInput::Main::SPACE{ KEYBOARD, SDL_SCANCODE_SPACE };
KeyboardInput::Main::Type const KeyboardInput::Main::SEMICOLON{ KEYBOARD, SDL_SCANCODE_SEMICOLON };
KeyboardInput::Main::Type const KeyboardInput::Main::APOSTROPHE{ KEYBOARD, SDL_SCANCODE_APOSTROPHE };
KeyboardInput::Main::Type const KeyboardInput::Main::GRAVE{ KEYBOARD, SDL_SCANCODE_GRAVE };
KeyboardInput::Main::Type const KeyboardInput::Main::COMMA{ KEYBOARD, SDL_SCANCODE_COMMA };
KeyboardInput::Main::Type const KeyboardInput::Main::PERIOD{ KEYBOARD, SDL_SCANCODE_PERIOD };
KeyboardInput::Main::Type const KeyboardInput::Main::SLASH{ KEYBOARD, SDL_SCANCODE_SLASH };
KeyboardInput::Main::Type const KeyboardInput::Main::CAPSLOCK{ KEYBOARD, SDL_SCANCODE_CAPSLOCK };
             
KeyboardInput::F::Type const KeyboardInput::F::F1{ KEYBOARD, SDL_SCANCODE_F1 };
KeyboardInput::F::Type const KeyboardInput::F::F2{ KEYBOARD, SDL_SCANCODE_F2 };
KeyboardInput::F::Type const KeyboardInput::F::F3{ KEYBOARD, SDL_SCANCODE_F3 };
KeyboardInput::F::Type const KeyboardInput::F::F4{ KEYBOARD, SDL_SCANCODE_F4 };
KeyboardInput::F::Type const KeyboardInput::F::F5{ KEYBOARD, SDL_SCANCODE_F5 };
KeyboardInput::F::Type const KeyboardInput::F::F6{ KEYBOARD, SDL_SCANCODE_F6 };
KeyboardInput::F::Type const KeyboardInput::F::F7{ KEYBOARD, SDL_SCANCODE_F7 };
KeyboardInput::F::Type const KeyboardInput::F::F8{ KEYBOARD, SDL_SCANCODE_F8 };
KeyboardInput::F::Type const KeyboardInput::F::F9{ KEYBOARD, SDL_SCANCODE_F9 };
KeyboardInput::F::Type const KeyboardInput::F::F10{ KEYBOARD, SDL_SCANCODE_F10 };
KeyboardInput::F::Type const KeyboardInput::F::F11{ KEYBOARD, SDL_SCANCODE_F11 };
KeyboardInput::F::Type const KeyboardInput::F::F12{ KEYBOARD, SDL_SCANCODE_F12 };
KeyboardInput::F::Type const KeyboardInput::F::F13{ KEYBOARD, SDL_SCANCODE_F13 };
KeyboardInput::F::Type const KeyboardInput::F::F14{ KEYBOARD, SDL_SCANCODE_F14 };
KeyboardInput::F::Type const KeyboardInput::F::F15{ KEYBOARD, SDL_SCANCODE_F15 };
KeyboardInput::F::Type const KeyboardInput::F::F16{ KEYBOARD, SDL_SCANCODE_F16 };
KeyboardInput::F::Type const KeyboardInput::F::F17{ KEYBOARD, SDL_SCANCODE_F17 };
KeyboardInput::F::Type const KeyboardInput::F::F18{ KEYBOARD, SDL_SCANCODE_F18 };
KeyboardInput::F::Type const KeyboardInput::F::F19{ KEYBOARD, SDL_SCANCODE_F19 };
KeyboardInput::F::Type const KeyboardInput::F::F20{ KEYBOARD, SDL_SCANCODE_F20 };
KeyboardInput::F::Type const KeyboardInput::F::F21{ KEYBOARD, SDL_SCANCODE_F21 };
KeyboardInput::F::Type const KeyboardInput::F::F22{ KEYBOARD, SDL_SCANCODE_F22 };
KeyboardInput::F::Type const KeyboardInput::F::F23{ KEYBOARD, SDL_SCANCODE_F23 };
KeyboardInput::F::Type const KeyboardInput::F::F24{ KEYBOARD, SDL_SCANCODE_F24 };
  
KeyboardInput::Home::Type const KeyboardInput::Home::PRINTSCREEN{ KEYBOARD, SDL_SCANCODE_PRINTSCREEN };
KeyboardInput::Home::Type const KeyboardInput::Home::SCROLLLOCK{ KEYBOARD, SDL_SCANCODE_SCROLLLOCK };
KeyboardInput::Home::Type const KeyboardInput::Home::PAUSE{ KEYBOARD, SDL_SCANCODE_PAUSE };
KeyboardInput::Home::Type const KeyboardInput::Home::INSERT{ KEYBOARD, SDL_SCANCODE_INSERT };
KeyboardInput::Home::Type const KeyboardInput::Home::HOME{ KEYBOARD, SDL_SCANCODE_HOME };
KeyboardInput::Home::Type const KeyboardInput::Home::PAGEUP{ KEYBOARD, SDL_SCANCODE_PAGEUP };
KeyboardInput::Home::Type const KeyboardInput::Home::DELETE{ KEYBOARD, SDL_SCANCODE_DELETE };
KeyboardInput::Home::Type const KeyboardInput::Home::END{ KEYBOARD, SDL_SCANCODE_END };
KeyboardInput::Home::Type const KeyboardInput::Home::PAGEDOWN{ KEYBOARD, SDL_SCANCODE_PAGEDOWN };
                                  
KeyboardInput::Arrow::Type const KeyboardInput::Arrow::RIGHT{ KEYBOARD, SDL_SCANCODE_RIGHT };
KeyboardInput::Arrow::Type const KeyboardInput::Arrow::LEFT{ KEYBOARD, SDL_SCANCODE_LEFT };
KeyboardInput::Arrow::Type const KeyboardInput::Arrow::DOWN{ KEYBOARD, SDL_SCANCODE_DOWN };
KeyboardInput::Arrow::Type const KeyboardInput::Arrow::UP{ KEYBOARD, SDL_SCANCODE_UP };
                                  
KeyboardInput::Control::Type const KeyboardInput::Control::EXECUTE{ KEYBOARD, SDL_SCANCODE_EXECUTE };
KeyboardInput::Control::Type const KeyboardInput::Control::HELP{ KEYBOARD, SDL_SCANCODE_HELP };
KeyboardInput::Control::Type const KeyboardInput::Control::MENU{ KEYBOARD, SDL_SCANCODE_MENU };
KeyboardInput::Control::Type const KeyboardInput::Control::SELECT{ KEYBOARD, SDL_SCANCODE_SELECT };
KeyboardInput::Control::Type const KeyboardInput::Control::STOP{ KEYBOARD, SDL_SCANCODE_STOP };
KeyboardInput::Control::Type const KeyboardInput::Control::REDO{ KEYBOARD, SDL_SCANCODE_AGAIN };
KeyboardInput::Control::Type const KeyboardInput::Control::UNDO{ KEYBOARD, SDL_SCANCODE_UNDO };
KeyboardInput::Control::Type const KeyboardInput::Control::CUT{ KEYBOARD, SDL_SCANCODE_CUT };
KeyboardInput::Control::Type const KeyboardInput::Control::COPY{ KEYBOARD, SDL_SCANCODE_COPY };
KeyboardInput::Control::Type const KeyboardInput::Control::PASTE{ KEYBOARD, SDL_SCANCODE_PASTE };
KeyboardInput::Control::Type const KeyboardInput::Control::FIND{ KEYBOARD, SDL_SCANCODE_FIND };
KeyboardInput::Control::Type const KeyboardInput::Control::MUTE{ KEYBOARD, SDL_SCANCODE_MUTE };
KeyboardInput::Control::Type const KeyboardInput::Control::MEDIASELECT{ KEYBOARD, SDL_SCANCODE_MEDIASELECT };
KeyboardInput::Control::Type const KeyboardInput::Control::VOLUMEUP{ KEYBOARD, SDL_SCANCODE_VOLUMEUP };
KeyboardInput::Control::Type const KeyboardInput::Control::VOLUMEDOWN{ KEYBOARD, SDL_SCANCODE_VOLUMEDOWN };
                                  
KeyboardInput::Control::Audio::Type const KeyboardInput::Control::Audio::AUDIONEXT{ KEYBOARD, SDL_SCANCODE_AUDIONEXT };
KeyboardInput::Control::Audio::Type const KeyboardInput::Control::Audio::AUDIOPREV{ KEYBOARD, SDL_SCANCODE_AUDIOPREV };
KeyboardInput::Control::Audio::Type const KeyboardInput::Control::Audio::AUDIOSTOP{ KEYBOARD, SDL_SCANCODE_AUDIOSTOP };
KeyboardInput::Control::Audio::Type const KeyboardInput::Control::Audio::AUDIOPLAY{ KEYBOARD, SDL_SCANCODE_AUDIOPLAY };
KeyboardInput::Control::Audio::Type const KeyboardInput::Control::Audio::AUDIOMUTE{ KEYBOARD, SDL_SCANCODE_AUDIOMUTE };
KeyboardInput::Control::Audio::Type const KeyboardInput::Control::Audio::AUDIOREWIND{ KEYBOARD, SDL_SCANCODE_AUDIOREWIND };
KeyboardInput::Control::Audio::Type const KeyboardInput::Control::Audio::AUDIOFASTFORWARD{ KEYBOARD, SDL_SCANCODE_AUDIOFASTFORWARD };
                                  
KeyboardInput::Mod::Type const KeyboardInput::Mod::META{ KEYBOARD, SDL_SCANCODE_LGUI };
KeyboardInput::Mod::Type const KeyboardInput::Mod::MODE{ KEYBOARD, SDL_SCANCODE_MODE };
KeyboardInput::Mod::Type const KeyboardInput::Mod::LCTRL{ KEYBOARD, SDL_SCANCODE_LCTRL };
KeyboardInput::Mod::Type const KeyboardInput::Mod::LSHIFT{ KEYBOARD, SDL_SCANCODE_LSHIFT };
KeyboardInput::Mod::Type const KeyboardInput::Mod::LALT{ KEYBOARD, SDL_SCANCODE_LALT };
KeyboardInput::Mod::Type const KeyboardInput::Mod::LMETA{ KEYBOARD, SDL_SCANCODE_LGUI };
KeyboardInput::Mod::Type const KeyboardInput::Mod::RCTRL{ KEYBOARD, SDL_SCANCODE_RCTRL };
KeyboardInput::Mod::Type const KeyboardInput::Mod::RSHIFT{ KEYBOARD, SDL_SCANCODE_RSHIFT };
KeyboardInput::Mod::Type const KeyboardInput::Mod::RALT{ KEYBOARD, SDL_SCANCODE_RALT };
KeyboardInput::Mod::Type const KeyboardInput::Mod::RMETA{ KEYBOARD, SDL_SCANCODE_RGUI };

KeyboardInput::Lang::Type const KeyboardInput::Lang::INTERNATIONAL1{ KEYBOARD, SDL_SCANCODE_INTERNATIONAL1 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::INTERNATIONAL2{ KEYBOARD, SDL_SCANCODE_INTERNATIONAL2 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::INTERNATIONAL3{ KEYBOARD, SDL_SCANCODE_INTERNATIONAL3 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::INTERNATIONAL4{ KEYBOARD, SDL_SCANCODE_INTERNATIONAL4 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::INTERNATIONAL5{ KEYBOARD, SDL_SCANCODE_INTERNATIONAL5 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::INTERNATIONAL6{ KEYBOARD, SDL_SCANCODE_INTERNATIONAL6 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::INTERNATIONAL7{ KEYBOARD, SDL_SCANCODE_INTERNATIONAL7 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::INTERNATIONAL8{ KEYBOARD, SDL_SCANCODE_INTERNATIONAL8 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::INTERNATIONAL9{ KEYBOARD, SDL_SCANCODE_INTERNATIONAL9 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::LANG1{ KEYBOARD, SDL_SCANCODE_LANG1 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::LANG2{ KEYBOARD, SDL_SCANCODE_LANG2 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::LANG3{ KEYBOARD, SDL_SCANCODE_LANG3 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::LANG4{ KEYBOARD, SDL_SCANCODE_LANG4 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::LANG5{ KEYBOARD, SDL_SCANCODE_LANG5 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::LANG6{ KEYBOARD, SDL_SCANCODE_LANG6 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::LANG7{ KEYBOARD, SDL_SCANCODE_LANG7 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::LANG8{ KEYBOARD, SDL_SCANCODE_LANG8 };
KeyboardInput::Lang::Type const KeyboardInput::Lang::LANG9{ KEYBOARD, SDL_SCANCODE_LANG9 };

KeyboardInput::Keypad::Type const KeyboardInput::Keypad::NUMLOCK{ KEYBOARD, SDL_SCANCODE_NUMLOCKCLEAR };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::DIVIDE{ KEYBOARD, SDL_SCANCODE_KP_DIVIDE };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::MULTIPLY{ KEYBOARD, SDL_SCANCODE_KP_MULTIPLY };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::MINUS{ KEYBOARD, SDL_SCANCODE_KP_MINUS };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::PLUS{ KEYBOARD, SDL_SCANCODE_KP_PLUS };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::ENTER{ KEYBOARD, SDL_SCANCODE_KP_ENTER };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::NUM_1{ KEYBOARD, SDL_SCANCODE_KP_1 };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::NUM_2{ KEYBOARD, SDL_SCANCODE_KP_2 };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::NUM_3{ KEYBOARD, SDL_SCANCODE_KP_3 };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::NUM_4{ KEYBOARD, SDL_SCANCODE_KP_4 };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::NUM_5{ KEYBOARD, SDL_SCANCODE_KP_5 };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::NUM_6{ KEYBOARD, SDL_SCANCODE_KP_6 };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::NUM_7{ KEYBOARD, SDL_SCANCODE_KP_7 };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::NUM_8{ KEYBOARD, SDL_SCANCODE_KP_8 };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::NUM_9{ KEYBOARD, SDL_SCANCODE_KP_9 };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::NUM_0{ KEYBOARD, SDL_SCANCODE_KP_0 };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::PERIOD{ KEYBOARD, SDL_SCANCODE_KP_PERIOD };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::COMMA{ KEYBOARD, SDL_SCANCODE_KP_COMMA };
KeyboardInput::Keypad::Type const KeyboardInput::Keypad::EQUALS{ KEYBOARD, SDL_SCANCODE_KP_EQUALS };

SomeCommonInputTypeRef KeyboardInput::InputByName(std::string_view name)
{
    static const std::initializer_list<std::pair<std::string_view, SomeCommonInputTypeRef>> keyList{
{ "A"     , Main::A },
{ "B"     , Main::B },
{ "C"     , Main::C },
{ "D"     , Main::D },
{ "E"     , Main::E },
{ "F"     , Main::F },
{ "G"     , Main::G },
{ "H"     , Main::H },
{ "I"     , Main::I },
{ "J"     , Main::J },
{ "K"     , Main::K },
{ "L"     , Main::L },
{ "M"     , Main::M },
{ "N"     , Main::N },
{ "O"     , Main::O },
{ "P"     , Main::P },
{ "Q"     , Main::Q },
{ "R"     , Main::R },
{ "S"     , Main::S },
{ "T"     , Main::T },
{ "U"     , Main::U },
{ "V"     , Main::V },
{ "W"     , Main::W },
{ "X"     , Main::X },
{ "Y"     , Main::Y },
{ "Z"     , Main::Z },
{ "NUM_1" , Main::NUM_1 },
{ "NUM_2" , Main::NUM_2 },
{ "NUM_3" , Main::NUM_3 },
{ "NUM_4" , Main::NUM_4 },
{ "NUM_5" , Main::NUM_5 },
{ "NUM_6" , Main::NUM_6 },
{ "NUM_7" , Main::NUM_7 },
{ "NUM_8" , Main::NUM_8 },
{ "NUM_9" , Main::NUM_9 },
{ "NUM_0" , Main::NUM_0 },
{ "RETURN", Main::RETURN },
{ "ESCAPE", Main::ESCAPE },
{ "BACKSPACE" , Main::BACKSPACE },
{ "TAB"       , Main::TAB },
{ "SPACE"     , Main::SPACE },
{ "SEMICOLON" , Main::SEMICOLON },
{ "APOSTROPHE", Main::APOSTROPHE },
{ "GRAVE"     , Main::GRAVE },
{ "COMMA"     , Main::COMMA },
{ "PERIOD"    , Main::PERIOD },
{ "SLASH"     , Main::SLASH },
{ "CAPSLOCK"  , Main::CAPSLOCK },
{ "META"      , Mod::META },
{ "MODE"      , Mod::MODE },
{ "LCTRL"     , Mod::LCTRL },
{ "LSHIFT"    , Mod::LSHIFT },
{ "LALT"      , Mod::LALT },
{ "LMETA"     , Mod::LMETA },
{ "RCTRL"     , Mod::RCTRL },
{ "RSHIFT"    , Mod::RSHIFT },
{ "RALT"      , Mod::RALT },
{ "RMETA"     , Mod::RMETA },
{ "INTERNATIONAL1", Lang::INTERNATIONAL1 },
{ "INTERNATIONAL2", Lang::INTERNATIONAL2 },
{ "INTERNATIONAL3", Lang::INTERNATIONAL3 },
{ "INTERNATIONAL4", Lang::INTERNATIONAL4 },
{ "INTERNATIONAL5", Lang::INTERNATIONAL5 },
{ "INTERNATIONAL6", Lang::INTERNATIONAL6 },
{ "INTERNATIONAL7", Lang::INTERNATIONAL7 },
{ "INTERNATIONAL8", Lang::INTERNATIONAL8 },
{ "INTERNATIONAL9", Lang::INTERNATIONAL9 },
{ "LANG1", Lang::LANG1 },
{ "LANG2", Lang::LANG2 },
{ "LANG3", Lang::LANG3 },
{ "LANG4", Lang::LANG4 },
{ "LANG5", Lang::LANG5 },
{ "LANG6", Lang::LANG6 },
{ "LANG7", Lang::LANG7 },
{ "LANG8", Lang::LANG8 },
{ "LANG9", Lang::LANG9 },
{ "NUMLOCK", Keypad::NUMLOCK },
{ "DIVIDE", Keypad::DIVIDE },
{ "MULTIPLY", Keypad::MULTIPLY },
{ "MINUS", Keypad::MINUS },
{ "PLUS", Keypad::PLUS },
{ "ENTER", Keypad::ENTER },
{ "NUM_1", Keypad::NUM_1 },
{ "NUM_2", Keypad::NUM_2 },
{ "NUM_3", Keypad::NUM_3 },
{ "NUM_4", Keypad::NUM_4 },
{ "NUM_5", Keypad::NUM_5 },
{ "NUM_6", Keypad::NUM_6 },
{ "NUM_7", Keypad::NUM_7 },
{ "NUM_8", Keypad::NUM_8 },
{ "NUM_9", Keypad::NUM_9 },
{ "NUM_0", Keypad::NUM_0 },
{ "PERIOD", Keypad::PERIOD },
{ "COMMA", Keypad::COMMA },
{ "EQUALS", Keypad::EQUALS },
{ "AUDIONEXT", Control::Audio::AUDIONEXT },
{ "AUDIOPREV", Control::Audio::AUDIOPREV },
{ "AUDIOSTOP", Control::Audio::AUDIOSTOP },
{ "AUDIOPLAY", Control::Audio::AUDIOPLAY },
{ "AUDIOMUTE", Control::Audio::AUDIOMUTE },
{ "AUDIOREWIND", Control::Audio::AUDIOREWIND },
{ "AUDIOFASTFORWARD", Control::Audio::AUDIOFASTFORWARD },
{ "F1", F::F1 },
{ "F2", F::F2 },
{ "F3", F::F3 },
{ "F4", F::F4 },
{ "F5", F::F5 },
{ "F6", F::F6 },
{ "F7", F::F7 },
{ "F8", F::F8 },
{ "F9", F::F9 },
{ "F10", F::F10 },
{ "F11", F::F11 },
{ "F12", F::F12 },
{ "F13", F::F13 },
{ "F14", F::F14 },
{ "F15", F::F15 },
{ "F16", F::F16 },
{ "F17", F::F17 },
{ "F18", F::F18 },
{ "F19", F::F19 },
{ "F20", F::F20 },
{ "F21", F::F21 },
{ "F22", F::F22 },
{ "F23", F::F23 },
{ "F24", F::F24 },
{ "PRINTSCREEN", Home::PRINTSCREEN },
{ "SCROLLLOCK", Home::SCROLLLOCK },
{ "PAUSE" , Home::PAUSE },
{ "INSERT", Home::INSERT },
{ "HOME"  , Home::HOME },
{ "PAGEUP", Home::PAGEUP },
{ "DELETE", Home::DELETE },
{ "END"   , Home::END },
{ "PAGEDOWN", Home::PAGEDOWN },
{ "RIGHT", Arrow::RIGHT },
{ "LEFT" , Arrow::LEFT },
{ "DOWN" , Arrow::DOWN },
{ "UP"   , Arrow::UP },
{ "EXECUTE", Control::EXECUTE },
{ "HELP", Control::HELP },
{ "MENU", Control::MENU },
{ "SELECT", Control::SELECT },
{ "STOP", Control::STOP },
{ "REDO", Control::REDO },
{ "UNDO", Control::UNDO },
{ "CUT", Control::CUT },
{ "COPY", Control::COPY },
{ "PASTE", Control::PASTE },
{ "FIND", Control::FIND },
{ "MUTE", Control::MUTE },
{ "MEDIASELECT", Control::MEDIASELECT },
{ "VOLUMEUP", Control::VOLUMEUP },
{ "VOLUMEDOWN", Control::VOLUMEDOWN },
    };
    //static const std::unordered_map<std::string_view, SomeCommonInputTypeRef> keyMap{ keyList.begin(), keyList.end() };
    //auto it = keyMap.find(name);
    auto it = utility::IteratorBase(std::ranges::find(std::views::keys(keyList), name));
    if (it == keyList.end())
        throw utility::TeiRuntimeError{ "Could not find a keyboard key with the name " + std::string{ name } };
    else
        return it->second;
}
