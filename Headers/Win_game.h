#ifndef WIN_GAME_H
#define WIN_GAME_H

#include "Declare.h"
#include "RunFunction.h"
#include "Render_rank.h"

void Win()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const int screenWidth = 1000, screenHeight = 800;
    int planeX = -500, planeY = 300;
    int rainbowX = -1200, rainbowY = 500;
    int gameWinX = 80, gameWinY = -450;
    bool gameWinFalling = false;
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
            gameWinFalling = true;
        }

        bool check = 0;

        if (gameWinFalling) {
            if(cnt_sound == 0)
            {
                cnt_sound = 1;
                Mix_PlayChannel(-1, stone_sound, 0);
                Mix_VolumeChunk(stone_sound, 128);

            }
            if (gameWinY < rainbowY - 420) {
                gameWinY += 50;
            }
            else check = 1;
        }
        SDL_Rect rainbowRect = {rainbowX, rainbowY, 1200, 100};
        SDL_Rect gameWinRect = {gameWinX, gameWinY, 800, 450};

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, Back_ground, NULL, NULL);
        SDL_Rect planeRect = {planeX, planeY, 500, 400};
        SDL_RenderCopy(renderer, plane, NULL, &planeRect);
        if(planeX >= -100) SDL_RenderCopy(renderer, rainbow, NULL, &rainbowRect);
        gameWinRect.y = gameWinY;
        if(gameWinFalling) SDL_RenderCopy(renderer, gameWin, NULL, &gameWinRect);
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
                    file << playerName << "," << point << std::endl; 
                    file.close();
                }
                timee++;
            }
            point = removeLeadingZeros(point);

            SDL_Color textColor = {255, 215, 0};
            SDL_Surface* textSurface = TTF_RenderText_Solid(font_point, point.c_str(), textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            int x = 300 + 40 * (4 - point.size());
            SDL_Rect textRect = {x, 20, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);

            std::string save = "";

            if(point.size() == 1)
            {
                save += "point";
            }
            else save += "points";

            textColor = {255, 215, 0};
            textSurface = TTF_RenderText_Solid(font, save.c_str(), textColor);
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