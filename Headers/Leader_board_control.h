#ifndef LEADER_BOARD_CONTROL_H 
#define LEADER_BOARD_CONTROL_H
#include "Declare.h"
#include "InitSDL.h"

void HandleEvents(SDL_Event &event) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
            isRun = 1;
        }
        else if (playerName.size() <= 11 && event.type == SDL_TEXTINPUT && !enterPressed) {
            Mix_PlayChannel(-1, sound_click, 0);
            Mix_VolumeChunk(sound_click, 30);
            playerName += event.text.text;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (enterPressed == 0 && event.key.keysym.sym == SDLK_BACKSPACE && !playerName.empty()) {
                Mix_PlayChannel(-1, sound_click, 0);
                Mix_VolumeChunk(sound_click, 30);
                playerName.pop_back();
            }
            else if (event.key.keysym.sym == SDLK_RETURN && playerName.size() != 0) {
                enterPressed = true;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (x >= buttonPlay.x && x <= buttonPlay.x + buttonPlay.w && y >= buttonPlay.y && y <= buttonPlay.y + buttonPlay.h) {
                if(playerName.size() != 0 && enterPressed == 1 && click_tutorial)
                {
                    Mix_PlayChannel(-1, sound_click, 0);
                    Mix_VolumeChunk(sound_click, 128);
                    before_menu ++;
                }
                else
                {
                    Mix_PlayChannel(-1, sound_wrong_click, 0);
                    Mix_VolumeChunk(sound_wrong_click, 128);
                }
            }
            else if (x >= 950 && x <= 1000 && y >= 0 && y <= 50) {
                Mix_PlayChannel(-1, sound_click, 0);
                Mix_VolumeChunk(sound_click, 128);
                if (music_check == 0) {
                    Mix_PauseMusic();
                    Mix_Volume(-1, 0); // Tắt toàn bộ hiệu ứng âm thanh
                    music_check = 1;
                }
                else {
                    Mix_ResumeMusic();
                    Mix_Volume(-1, 128); // Bật lại toàn bộ hiệu ứng âm thanh
                    music_check = 0;
                }
            }
            else if (x >= buttonScore.x && x <= buttonScore.x + buttonScore.w && y >= buttonScore.y && y <= buttonScore.y + buttonScore.h)
            {
                Mix_PlayChannel(-1, sound_click, 0);
                Mix_VolumeChunk(sound_click, 128);
                if (!isLeaderboardOpen) {
                    std::thread leaderboardThread(showLeaderboardWindow, "player.txt");
                    leaderboardThread.detach();
                }
            }
            else if(x >= buttonHelp.x && x <= buttonHelp.x + buttonHelp.w && y >= buttonHelp.y && y <= buttonHelp.y + buttonHelp.h)
            {
                Mix_PlayChannel(-1, sound_click, 0);
                Mix_VolumeChunk(sound_click, 128);
                if (!isTutorial) {
                    click_tutorial = 1;
                    std::thread leaderboardThread(showTutorialboardWindow);
                    leaderboardThread.detach();
                }
            }
        }
    }
}

#endif