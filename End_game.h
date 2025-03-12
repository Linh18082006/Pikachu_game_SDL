#ifndef END_GAME_H  
#define END_GAME_H 

#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>

#include "initSDL.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void End_game()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    TTF_Font* fontt = TTF_OpenFont("arial.ttf", 50);

    SDL_Texture* Background = LoadTexture("Back_ground.png", renderer);
    SDL_RenderCopy(renderer, Background, NULL, NULL);

    SDL_Color textColor = {255, 0, 0}; // Màu đỏ
    SDL_Surface* textSurface = TTF_RenderText_Solid(fontt, "Game Over", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {200, 200, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);

    SDL_RenderPresent(renderer); // Cập nhật cửa sổ

    // Giữ cửa sổ mở
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_RenderCopy(renderer, Background, NULL, NULL);

        SDL_Delay(16); // Chờ 16ms tránh tiêu tốn CPU
    }

    // Dọn dẹp tài nguyên
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#endif  
