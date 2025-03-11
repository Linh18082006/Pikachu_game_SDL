#ifndef END_GAME_H  
#define END_GAME_H 

#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>

#include "initSDL.h"

void End_game()
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    initSDL(window, renderer);
    mixer();
    TTF_Init();
}

#endif 