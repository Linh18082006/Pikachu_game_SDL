#ifndef PREPARE_BOARD_H 
#define PREPARE_BOARD_H
#include "Declare.h"

//Ham chuan bi hien bang
extern const int TILE_SIZE;

void PreparePresentBoard(int times_wrong)
{

    SDL_RenderCopy(renderer, Back_ground, NULL, NULL);

    if(cnt_pr % 2 == 0)
    {
        SDL_Rect tiless = {920, 400, 50, 50};
        SDL_RenderCopy(renderer, Pause, NULL, &tiless);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &tiless);
    }
    else
    {
        SDL_Rect tiless = {920, 400, 50, 50};
        SDL_RenderCopy(renderer, Resume, NULL, &tiless);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &tiless);
    }

    SDL_Rect tiless = {25, 400, 50, 50};
    SDL_RenderCopy(renderer, Restart, NULL, &tiless);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &tiless);

    tiless = {50, 50, 900, 50};
    SDL_Rect srcRect = {10, 10, 880, 35};
    SDL_RenderCopy(renderer, Time, &srcRect, &tiless);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &tiless);

    SDL_Rect tile = {870, 702, TILE_SIZE - 10, TILE_SIZE - 10};
    SDL_RenderCopy(renderer, HELP, NULL, &tile);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &tile);

    tile = {70, 700, 408, TILE_SIZE};

    int time = 0;
    for(int i = 1; i <= times_wrong; i++)
    {
        tile = {535 + time * (TILE_SIZE - 5) - 310, 705, TILE_SIZE - 10, TILE_SIZE - 10};
        SDL_RenderCopy(renderer, Heart, NULL, &tile);
        time++;
    }
    SDL_Surface* textSurface = nullptr;

    if (font) {
        SDL_Color textColor = {255, 215, 0};
        switch (cnt_help)
        {
        case 4:
            textSurface = TTF_RenderText_Solid(font, "4", textColor);
            break;
        case 5:
            textSurface = TTF_RenderText_Solid(font, "5", textColor);
            break;
        case 1:
            textSurface = TTF_RenderText_Solid(font, "1", textColor);
            break;
        case 2:
            textSurface = TTF_RenderText_Solid(font, "2", textColor);
            break;
        case 3:
            textSurface = TTF_RenderText_Solid(font, "3", textColor);
            break;
        case 0:
            textSurface = TTF_RenderText_Solid(font, "0", textColor);
            break;
        default:
            break;
        }
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {837, 709, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        textColor = {255, 215, 0};
        textSurface = TTF_RenderText_Solid(font, "Health ", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        textRect = {390 - 310, 709, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    SDL_Color textColor = {255, 215, 0};
    textSurface = TTF_RenderText_Solid(font, "Hint", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {745, 709, textSurface->w, textSurface->h}; // Đặt gần hình light hơn
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    textColor = {255, 215, 0};
    textSurface = TTF_RenderText_Solid(font_times, "x", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect = {826, 714, textSurface->w, textSurface->h}; // Đặt gần hình light hơn
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

//Ham hien thoi gian
void PresentTime(const char* str, int stt)
{
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font_number, str, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect;
    if(stt == 1) textRect = {453, 115, textSurface->w, textSurface->h};
    else if(stt == 2) textRect = {473, 115, textSurface->w, textSurface->h};
    else if(stt == 3) textRect = {505, 115, textSurface->w, textSurface->h};
    else if(stt == 4) textRect = {527, 115, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

#endif