#ifndef START_GAME_H
#define START_GAME_H

#include "Declare.h"

void Start_game(SDL_Renderer* renderer) {
    start_game += 10;
    SDL_Rect tile = {0, start_game, 1000, 800};
    SDL_RenderCopy(renderer, Start_game_image, NULL, &tile);
}

void Start_game_(SDL_Renderer* renderer) {
    start_game -= 10;
    SDL_Rect tile = {0, start_game, 1000, 800};
    SDL_RenderCopy(renderer, Start_game_image, NULL, &tile);
}

void Start_game__(SDL_Renderer* renderer) {
    SDL_Rect tile = {0, 0, 1000, 800};
    SDL_RenderCopy(renderer, Start_game_image, NULL, &tile);
}

#endif