#include "InitSDL.h"
#include "Declare.h"
#include "RunFunction.h"
#include "Render_board.h"
#include "Render_help.h"
#include "Render_rank.h"
#include "Init_board.h"
#include "Render_Tutorial.h"
#include "Leader_board_control.h"
#include "Write_name.h"
#include "Game_Play.h"
#include "Win_game.h"
#include "Lose_game.h"

const int DISTANCE_H = 180;
const int DISTANCE_W = 100;
const int TIMER_WIDTH = 900;
const int TIMER_HEIGHT = 50;
const int GAME_TIME = 1200000 + 2000;
const int TILE_SIZE = 50;
const int ROWS = 10;
const int COLS = 16;

int main(int argc, char* argv[]) {
    srand(0);
    initSDL(window, renderer);
    TTF_Init();
    Declare_variable();

    Mix_PlayMusic(music_background, -1);
    Mix_VolumeMusic(50);
    SDL_Event events;
    while (isRunning) {
        HandleEvents(events);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, Back_ground_start, NULL, NULL);
        if(music_check == 0) {
            SDL_Rect tile = {950, 0, 50, 50};
            SDL_RenderCopy(renderer, Music_On, NULL, &tile);
        }
        else {
            SDL_Rect tile = {950, 0, 30, 50};
            SDL_RenderCopy(renderer, Music_Off, NULL, &tile);
        }
        SDL_RenderCopy(renderer, Button_Play, NULL, &buttonPlay);
        SDL_RenderCopy(renderer, Button_Help, NULL, &buttonHelp);
        SDL_RenderCopy(renderer, Button_Score, NULL, &buttonScore);
        RenderText(renderer, font_name);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    Mix_PauseMusic();
    while((isRun == 0 && running == 1) || restart == 1){
        Mix_PlayMusic(music, -1);
        Mix_VolumeMusic(5);
        InitBoard();
        restart = 0, next_game = 0;
        bool ktra = 0, needRender = 0, run = 0, run_first = 0;
        SDL_Event event;

        startTime = SDL_GetTicks();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        RenderBoard(save_cnt_wrong);
        SDL_RenderPresent(renderer);
        Uint32 pauseStart = 0; 
        lastUpdateTime = SDL_GetTicks();
        save_cnt_wrong = 5;
        timee = 0;
        while(save_cnt_wrong > -1 && running && next == 0){
            ktra = 0;
            for(int i = 0; i < ROWS; ++i)
            {
                for(int j = 0; j < COLS; j++)
                {
                    if(board[i][j] != -1)
                    {
                        ktra = 1;
                        break;
                    }
                }
                if(ktra == 1) break;
            }
            if(ktra == 0)
            {
                Win();
                if(next_game == 1) break;
            }
            if(save_cnt_wrong == 0 || SDL_GetTicks() - startTime > 1200000 + 1000)
            {
                Lose();
                if(next_game == 1) break;
            }
            if(SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) running = false;
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if(cnt_pr % 2 == 1 && (event.button.x < 920 || event.button.x > 970 || event.button.y < 400 || event.button.y > 450))
                    {
                        Mix_PlayChannel(-1, sound_error_click, 0);
                        Mix_VolumeChunk(sound_error_click, 128);
                    }
                    else if(cnt_pr % 2 == 0 && cnt == 0 && event.button.x >= 100 && event.button.x <= 900 && event.button.y >= 180 && event.button.y <= 680)
                    {
                        Mix_PlayChannel(-1, sound_click, 0);
                        Mix_VolumeChunk(sound_click, 128);
                        FirstClick(event.button.x, event.button.y);
                        if(board[first_click.f][first_click.s] == -1) continue;
                        cnt++;
                        board[first_click.f][first_click.s] = -2;
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderClear(renderer);
                        RenderBoard(save_cnt_wrong);
                        SDL_RenderPresent(renderer);
                    }
                    else if(cnt_pr % 2 == 0 && cnt == 1 && event.button.x >= 10 && event.button.x <= 900 && event.button.y >= 180 && event.button.y <= 680)
                    {
                        Mix_PlayChannel(-1, sound_click, 0);
                        Mix_VolumeChunk(sound_click, 128);
                        bool check_change = 0;
                        pauseStart = SDL_GetTicks();
                        while(check_change == 0){
                            SecondClick(event.button.x, event.button.y);

                            if(board[second_click.f][second_click.s] == -1) continue;
                            board[second_click.f][second_click.s] = -2;

                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderClear(renderer);
                            RenderBoard(save_cnt_wrong);
                            SDL_RenderPresent(renderer);
                            if (SDL_GetTicks() - pauseStart >= 300) 
                            {
                                check_change = 1;

                                board[first_click.f][first_click.s] = save_board[first_click.f][first_click.s];
                                board[second_click.f][second_click.s] = save_board[second_click.f][second_click.s];

                                if(Game_Play(first_click, second_click) == 1)
                                {
                                    Mix_PlayChannel(-1, sound_correct_click, 0);
                                    Mix_VolumeChunk(sound_correct_click, 128);
                                    board[first_click.f][first_click.s] = -1;
                                    board[second_click.f][second_click.s] = -1;
                                }
                                else if(first_click.f != second_click.f || first_click.s != second_click.s)
                                {
                                    Mix_PlayChannel(-1, sound_wrong_click, 0);
                                    Mix_VolumeChunk(sound_wrong_click, 128);
                                    save_cnt_wrong--;
                                }
                                cnt = 0;
                            }
                        }
                    }
                    else if(cnt_pr % 2 == 0 && cnt_help > 0 && event.button.x >= 870 && event.button.x <= 910 && event.button.y >= 700 && event.button.y <= 740)
                    {
                        Mix_PlayChannel(-1, sound_click, 0);
                        Mix_VolumeChunk(sound_click, 128);
                        cnt_help--;
                        bool check_change = 0;
                        pauseStart = SDL_GetTicks();
                        int cnt = 0;
                        std::pair < std::pair < int, int >, std::pair < int, int > > save_help = HELP_HANG_COT();
                        while(check_change == 0){
                            cnt++;
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderClear(renderer);
                            RenderBoard_Help(save_help, save_cnt_wrong);
                            SDL_RenderPresent(renderer);
                            if(SDL_GetTicks() - pauseStart > 1000) check_change = 1;
                        }
                    }
                    else if(event.button.x >= 920 && event.button.x <= 970 && event.button.y >= 400 && event.button.y <= 450)
                    {
                        Mix_PlayChannel(-1, sound_click, 0);
                        Mix_VolumeChunk(sound_click, 128);
                        cnt_pr++;
                        if(cnt_pr % 2 == 1)
                        {
                            Mix_PauseMusic();
                            start_pause = SDL_GetTicks();
                        }
                        else{
                            Mix_ResumeMusic();
                            startTime += (SDL_GetTicks() - start_pause);
                        }

                    }
                    else if(cnt_pr % 2 == 0 && event.button.x >= 25 && event.button.x <= 75 && event.button.y >= 400 && event.button.y <= 450)
                    {
                        Mix_PlayChannel(-1, sound_click, 0);
                        Mix_VolumeChunk(sound_click, 128);
                        cnt_help = 5;
                        restart = 1;
                        break;
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            RenderBoard(save_cnt_wrong);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
    }
    quitSDL(window, renderer);
    TTF_CloseFont(font);
    TTF_Quit();
    Mix_FreeMusic(music);
    Mix_FreeChunks();
    Mix_CloseAudio();

    return 0;
}