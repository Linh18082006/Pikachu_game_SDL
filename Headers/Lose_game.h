#ifndef LOSE_GAME_H
#define LOSE_GAME_H

#include "Declare.h"
#include "RunFunction.h"
#include "Render_rank.h"

void Lose()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const int screenWidth = 1000, screenHeight = 800;
    int planeX = -500, planeY = 300;
    int rainbowX = -1200, rainbowY = 500;
    int gameOverX = 100, gameOverY = -450;
    bool gameOverFalling = false;
    int a = 0;

    bool runningg = true, cnt_sound = 0;
    Mix_PlayChannel(-1, plane_sound, 0);
    Mix_VolumeChunk(plane_sound, 128);
    while (runningg) {
        SDL_Event event;
        if(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            else if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                if(event.button.x >= 475 && event.button.x <= 525 && event.button.y >= 620 && event.button.y <= 670)
                {
                    Mix_PlayChannel(-1, sound_click, 0);
                    Mix_VolumeChunk(sound_click, 128);
                    runningg = 0;
                    break;
                }
                else if(event.button.x >= 400 && event.button.x <= 600 && event.button.y >= 700 && event.button.y <= 760)
                {
                    Mix_PlayChannel(-1, sound_click, 0);
                    Mix_VolumeChunk(sound_click, 128);
                    if (!isLeaderboardOpen) {
                        std::thread leaderboardThread(showLeaderboardWindow, "player.txt");
                        leaderboardThread.detach(); 
                    }
                }
            }
        }

        if (planeX < screenWidth) {
            planeX += 10;
            if(planeX >= -100) rainbowX += 10;
        } else {
            gameOverFalling = true;
        }

        bool check = 0;

        if (gameOverFalling) {
            if(cnt_sound == 0)
            {
                cnt_sound = 1;
                Mix_PlayChannel(-1, stone_sound, 0);
                Mix_VolumeChunk(stone_sound, 128);

            }
            if (gameOverY < rainbowY - 350) {
                gameOverY += 50;
            }
            else check = 1;
        }
        SDL_Rect rainbowRect = {rainbowX, rainbowY, 1200, 100};
        SDL_Rect gameOverRect = {gameOverX, gameOverY, 800, 450};

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, Back_ground, NULL, NULL);
        SDL_Rect planeRect = {planeX, planeY, 500, 400};
        SDL_RenderCopy(renderer, plane, NULL, &planeRect);
        if(planeX >= -100) SDL_RenderCopy(renderer, rainbow, NULL, &rainbowRect);
        gameOverRect.y = gameOverY;
        if(gameOverFalling) SDL_RenderCopy(renderer, gameOver, NULL, &gameOverRect);
        if(check == 1)
        {
            SDL_Rect tiless = {475, 620, 50, 50};
            SDL_RenderCopy(renderer, Restart, NULL, &tiless);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &tiless);

            tiless = {400, 700, 200, 60};
            SDL_RenderCopy(renderer, Button_Score, NULL, &tiless);
        }
        if(check)
        {
            if(timee == 0)
            {
                std::ofstream file("Player.txt", std::ios::app); 
                if (file.is_open()) {
                    file << playerName << "," << "0" << std::endl; 
                    file.close();
                }
                timee++;
            }

            SDL_Color textColor = {255, 215, 0};
            SDL_Surface* textSurface = TTF_RenderText_Solid(font_point, "0", textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {430, 20, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);

            textColor = {255, 215, 0};
            textSurface = TTF_RenderText_Solid(font, "point", textColor);
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            textRect = {500, 35, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    if(runningg == 0)
    {
        removeLast();
        cnt_help = 5;
        restart = 1;
        next_game = 1;
    }
}

#endif