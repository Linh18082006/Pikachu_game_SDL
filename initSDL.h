#ifndef INITSDL_H  
#define INITSDL_H 

#include <windows.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <string>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const std::string WINDOW_TITLE;

void logSDLError(std::ostream &os, const std::string &msg, bool fatal = false) {
    os << msg << " error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

}

SDL_Texture* LoadTexture(const std::string &file, SDL_Renderer* renderer) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(file.c_str()); // Hoặc SDL_image nếu dùng PNG
    if (loadedSurface == nullptr) {
        std::cout << "Failed to load image " << file << " SDL Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return texture;
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    	SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
}

void mixer()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_AllocateChannels(16);
}

#endif 
