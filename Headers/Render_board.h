#ifndef RENDER_BOARD_H 
#define RENDER_BOARD_H
#include "Declare.h"
#include "Prepare_board.h"
#include "RunFunction.h"

//Hien bang
void RenderBoard(int time_wrong) {

    PreparePresentBoard(time_wrong);

    Uint32 elapsedTime;
    if(cnt_pr % 2 == 0)
    {
        elapsedTime = SDL_GetTicks() - startTime;
        lastUpdateTime = elapsedTime;
    }
    else elapsedTime = lastUpdateTime;

    if(before_menu == 1) {
        Time_present = milliseconds_to_mmss(1200000);
        elapsedTime = 0;
    }
    else Time_present = milliseconds_to_mmss(1200000 - elapsedTime);

    float progress = (float)elapsedTime / GAME_TIME; 
    int currentWidth = (int)(progress * TIMER_WIDTH);
    if (currentWidth > TIMER_WIDTH) currentWidth = TIMER_WIDTH; 

    int xPosition = 50 + (TIMER_WIDTH - currentWidth);

    SDL_Rect timeBar = {xPosition, 50, currentWidth, TIMER_HEIGHT}; 
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &timeBar);

    SDL_Rect black = {450, 110, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, Black, NULL, &black);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &black);
    black = {500, 110, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, Black, NULL, &black);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &black);

    point = "";

    for(int i = 1; i <= 4; i++)
    {
        point += Time_present[i - 1];
        const char* cText = Time_present[i - 1].c_str();
        PresentTime(cText, i);
    }

    Time_present.clear();

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if(board[i][j] == -2)
            {
                SDL_Rect tiles = {DISTANCE_W + j * TILE_SIZE, DISTANCE_H + i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_RenderCopy(renderer, textures_red[save_color[save_board[i][j]]], NULL, &tiles);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &tiles);
            }
            else if(board[i][j] == -1){
                continue;
            }
            else
            {
                SDL_Rect tile = {DISTANCE_W + j * TILE_SIZE, DISTANCE_H + i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_RenderCopy(renderer, textures[save_color[board[i][j]]], NULL, &tile);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &tile);
            }
        }
    }
}

#endif