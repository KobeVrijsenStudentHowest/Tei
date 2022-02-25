
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <ImGui.h>

int main(int argc, char*[])
{
	if (argc == 0)
	// Always false, build and link test only
	{
		(void)SDL_getenv(nullptr);
		(void)TTF_CloseFont(nullptr);
		(void)IMG_LoadTGA_RW(nullptr);
		(void)ImGui_ImplSDL2_InitForSDLRenderer(nullptr, nullptr);
	}

	return 0;
}