#define f first
#define s second
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <random>
#include <utility>
#include <queue>
#include <string>
#include <windows.h>
#include <iostream>
#include <SDL_image.h>
#include <fstream>

#include "initSDL.h"
#include "declare.h"

const int DISTANCE_H = 180;
const int DISTANCE_W = 100;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const int TIMER_WIDTH = 900;
const int TIMER_HEIGHT = 50;
const int GAME_TIME = 1200000 + 2000; 
const std::string WINDOW_TITLE = "Pikachu";

const int TILE_SIZE = 50;
const int ROWS = 10;
const int COLS = 16;

std::vector<std::vector<int>> board, save_board;
std::vector < std::string > Time_present;
SDL_Point firstPick = {-1, -1};
std::pair < int, int > first_click, second_click;
int save_color[1000];
bool check[100][100];
bool check_help = 0;
int cnt_help = 5, save_cnt_wrong = 5;
int cnt = 0, cnt_pr = 0;
int lastUpdateTime, startTime, start_pause;
bool win = 0;

void PresentBox(int w, SDL_Texture* s)
{
    SDL_Rect tile = {w, 702, TILE_SIZE - 10, TILE_SIZE - 10};
    SDL_RenderCopy(renderer, s, NULL, &tile);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &tile);
}

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

    PresentBox(870, HELP);
    
    SDL_Rect tile = {70, 700, 408, TILE_SIZE};

    
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

void RenderBoard_Help(std::pair < std::pair < int, int >, std::pair < int, int > > Matran, int time_wrong) {

    int HANG = Matran.f.f;
    int COT = Matran.f.s;

    int HANG_ = Matran.s.f;
    int COT_ = Matran.s.s;

    PreparePresentBoard(time_wrong);
    
    Uint32 elapsedTime;
    if(cnt_pr % 2 == 0)
    {
        elapsedTime = SDL_GetTicks() - startTime; // Tổng thời gian đã trôi qua
        lastUpdateTime = elapsedTime;
    }
    else elapsedTime = lastUpdateTime;

    Time_present = milliseconds_to_mmss(1200000 - elapsedTime);
    
    float progress = (float)elapsedTime / GAME_TIME; // Tăng từ 0 → 1
    int currentWidth = (int)(progress * TIMER_WIDTH);
    if (currentWidth > TIMER_WIDTH) currentWidth = TIMER_WIDTH; // Đảm bảo không vượt quá 900

    // Thanh sẽ tăng dần từ phải sang trái, nên cần tính lại tọa độ x
    int xPosition = 50 + (TIMER_WIDTH - currentWidth);

    // Vẽ thanh thời gian
    SDL_Rect timeBar = {xPosition, 50, currentWidth, TIMER_HEIGHT}; // Di chuyển dần sang trái
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

    for(int i = 1; i <= 4; i++)
    {
        const char* cText = Time_present[i - 1].c_str();
        PresentTime(cText, i);
    }

    Time_present.clear();

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if(board[i][j] == -1){
                continue;
            }
            else if(((j == COT && i == HANG) || (j == COT_ && i == HANG_)))
            {
                SDL_Rect tiles = {DISTANCE_W + j * TILE_SIZE, DISTANCE_H + i * TILE_SIZE, TILE_SIZE, TILE_SIZE};   
                SDL_RenderCopy(renderer, textures_green[save_color[save_board[i][j]]], NULL, &tiles);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &tiles);
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

void RenderBoard(int time_wrong) {

    PreparePresentBoard(time_wrong);

    Uint32 elapsedTime;
    if(cnt_pr % 2 == 0)
    {
        elapsedTime = SDL_GetTicks() - startTime; // Tổng thời gian đã trôi qua
        lastUpdateTime = elapsedTime;
    }
    else elapsedTime = lastUpdateTime;

    Time_present = milliseconds_to_mmss(1200000 - elapsedTime);

    float progress = (float)elapsedTime / GAME_TIME; // Tăng từ 0 → 1
    int currentWidth = (int)(progress * TIMER_WIDTH);
    if (currentWidth > TIMER_WIDTH) currentWidth = TIMER_WIDTH; // Đảm bảo không vượt quá 900

    // Thanh sẽ tăng dần từ phải sang trái, nên cần tính lại tọa độ x
    int xPosition = 50 + (TIMER_WIDTH - currentWidth);

    // Vẽ thanh thời gian
    SDL_Rect timeBar = {xPosition, 50, currentWidth, TIMER_HEIGHT}; // Di chuyển dần sang trái
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

    for(int i = 1; i <= 4; i++)
    {
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


void InitBoard() {
    int pos = 0;
    std::vector<int> tiles;
    for (int i = 0; i < (ROWS * COLS) / 2; ++i) {
        tiles.push_back(i);
        tiles.push_back(i);
        save_color[i] = pos;
        if(i % 2 == 1) pos++;
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(tiles.begin(), tiles.end(), g);
    
    board.resize(ROWS, std::vector<int>(COLS));
    save_board.resize(ROWS, std::vector<int>(COLS));
    for (int i = 0, k = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j, ++k) {
            board[i][j] = tiles[k];
            save_board[i][j] = tiles[k];
        }
    }
}

void FirstClick(int y, int x)
{
    first_click.f = (x - DISTANCE_H) / TILE_SIZE;
    first_click.s = (y - DISTANCE_W) / TILE_SIZE;
}

    void SecondClick(int y, int x)
{
    second_click.f = (x - DISTANCE_H) / TILE_SIZE;
    second_click.s = (y - DISTANCE_W) / TILE_SIZE;
}

bool Game_Play(std::pair < int, int > f_click,std:: pair < int, int > s_click)
{
    if(board[f_click.f][f_click.s] == -1 || board[s_click.f][s_click.s] == -1) return 0;
    if(f_click.f == s_click.f && f_click.s == s_click.s) return 0;
    if(save_color[board[f_click.f][f_click.s]] != save_color[board[s_click.f][s_click.s]]) return 0;
    bool save = 0, block = 0;
    std::queue < std::pair < int, int > > q;
    q.push({f_click.f, f_click.s});
    for(int i = 0; i < 100; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            check[i][j] = 0;
        }
    }
    check[f_click.f][f_click.s] = 1;
    while(q.size() > 0)
    {
        int r = q.front().f;
        int c = q.front().s;
        q.pop();
        if(r == ROWS - 1 || r == 0 || c == COLS - 1 || c == 0) block = 1;
        if(r >= 0 && r < ROWS && c + 1 >= 0 && c + 1 < COLS && check[r][c + 1] == 0)
        {
            if(board[r][c + 1] == -1)
            {
                q.push({r, c + 1});
                check[r][c + 1] = 1;
            }
            else
            {
                if(r == s_click.f && c + 1 == s_click.s)
                {
                    save = 1;
                    break;
                }
            }
        }
        if(r >= 0 && r < ROWS && c - 1 >= 0 && c - 1 < COLS && check[r][c - 1] == 0)
        {
            if(board[r][c - 1] == -1)
            {
                q.push({r, c - 1});
                check[r][c - 1] = 1;
            }
            else
            {
                if(r == s_click.f && c - 1 == s_click.s)
                {
                    save = 1;
                    break;
                }
            }
        }
        if(r + 1 >= 0 && r + 1 < ROWS && c >= 0 && c < COLS && check[r + 1][c] == 0)
        {
            if(board[r + 1][c] == -1)
            {
                q.push({r + 1, c});
                check[r + 1][c] = 1;
            } 
            else
            {
                if(r + 1 == s_click.f && c == s_click.s)
                {
                    save = 1;
                    break;
                }   
            }
        }
        if(r - 1 >= 0 && r - 1 < ROWS && c >= 0 && c < COLS && check[r - 1][c] == 0)
        {
            if(board[r - 1][c] == -1)
            {
                q.push({r - 1, c});
                check[r - 1][c] = 1;
            }
            else
            {
                if(r - 1 == s_click.f && c == s_click.s)
                {
                    save = 1;
                    break;
                }
            }
        }
    }
    while(q.size() > 0) q.pop();
    if(block == 1 && save == 0)
    {
        q.push({s_click.f, s_click.s});
        for(int i = 0; i < 100; i++)
        {
            for(int j = 0; j < 100; j++)
            {
                check[i][j] = 0;
            }
        }
        check[s_click.f][s_click.s] = 1;
        while(q.size() > 0)
        {
            int r = q.front().f;
            int c = q.front().s;
            q.pop();
            if(r == ROWS - 1 || r == 0 || c == COLS - 1 || c == 0)
            {
                save = 1;
                break;
            }
            if(r >= 0 && r < ROWS && c + 1 >= 0 && c + 1 < COLS && check[r][c + 1] == 0)
            {
                if(board[r][c + 1] == -1)
                {
                    q.push({r, c + 1});
                    check[r][c + 1] = 1;
                }
            }
            if(r >= 0 && r < ROWS && c - 1 >= 0 && c - 1 < COLS && check[r][c - 1] == 0)
            {
                if(board[r][c - 1] == -1)
                {
                    q.push({r, c - 1});
                    check[r][c - 1] = 1;
                }
            }
            if(r + 1 >= 0 && r + 1 < ROWS && c >= 0 && c < COLS && check[r + 1][c] == 0)
            {
                if(board[r + 1][c] == -1)
                {
                    q.push({r + 1, c});
                    check[r + 1][c] = 1;
                }

            }
            if(r - 1 >= 0 && r - 1 < ROWS && c >= 0 && c < COLS && check[r - 1][c] == 0)
            {
                if(board[r - 1][c] == -1)
                {
                    q.push({r - 1, c});
                    check[r - 1][c] = 1;
                }
            }
        }
        while(q.size() > 0) q.pop();
    }
    return save;
}

std::pair<std::pair<int, int>, std::pair<int, int>> HELP_HANG_COT()
{
    std::pair<int, int> matrix1 = {-1, -1}, matrix2 = {-1, -1};

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j] == -1) continue; // Bỏ qua ô trống

            for (int h = 0; h < ROWS; h++)
            {
                for (int k = 0; k < COLS; k++)
                {
                    if (i == h && j == k) continue; // Không xét chính nó
                    if (board[h][k] == -1) continue; // Bỏ qua ô trống

                    if (save_color[board[i][j]] == save_color[board[h][k]])
                    {
                        if (Game_Play({i, j}, {h, k}))
                        {
                            return {{i, j}, {h, k}}; // Trả về ngay khi tìm thấy
                        }
                    }
                }
            }
        }
    }
    return {{-1, -1}, {-1, -1}}; // Không tìm thấy cặp hợp lệ
}

int main(int argc, char* argv[]) {
    srand(0);
    initSDL(window, renderer);
    mixer();
    TTF_Init();
    font = TTF_OpenFont("orbitron-black.otf", 35);
    font_times = TTF_OpenFont("orbitron-black.otf", 22);
    font_number = TTF_OpenFont("arial.ttf", 35);
    
    Restart = LoadTexture("restart.png", renderer);
    Resume = LoadTexture("resume_game.png", renderer);
    Pause = LoadTexture("pause_game.png", renderer);
    Heart = LoadTexture("heart.png", renderer);
    Black = LoadTexture("black.png", renderer);
    HELP = LoadTexture("light.png", renderer);
    Back_ground = LoadTexture("Back_ground.png", renderer);
    Time = LoadTexture("Time.png", renderer);

    music = Mix_LoadMUS("background.mp3");
    plane_sound = Mix_LoadWAV("plane_sound.wav");
    stone_sound = Mix_LoadWAV("stone_sound.wav");
    sound_click = Mix_LoadWAV("select_click.wav");
    sound_wrong_click = Mix_LoadWAV("wrong.wav");;
    sound_correct_click = Mix_LoadWAV("correct.wav");;
    sound_error_click = Mix_LoadWAV("click-error.wav");;
    // sound = Mix_LoadWAV()

    for (int i = 1; i <= 40; i++) {
        std::string filename = "object_" + std::to_string(i) + ".png"; 
        textures.push_back(LoadTexture(filename, renderer));
    }

    for (int i = 1; i <= 40; i++) {
        std::string filename = "object_" + std::to_string(i) + "_red_tint_50x50.png"; 
        textures_red.push_back(LoadTexture(filename, renderer));
    }

    for (int i = 1; i <= 40; i++) {
        std::string filename = "object_" + std::to_string(i) + "_green_overlay.png"; 
        textures_green.push_back(LoadTexture(filename, renderer));
    }
    
    bool running = true;
    bool restart = 0;

    while(running == 1 || restart == 1){
        Mix_PlayMusic(music, -1);
        Mix_VolumeMusic(5);
        InitBoard();
        restart = 0;
        bool ktra = 0, needRender = 0, run = 0, run_first = 0;
        SDL_Event event;

        startTime = SDL_GetTicks();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        RenderBoard(save_cnt_wrong);
        SDL_RenderPresent(renderer);
        Uint32 pauseStart = 0; // Biến lưu thời gian bắt đầu tạm dừng
        lastUpdateTime = SDL_GetTicks();
        save_cnt_wrong = 5;
        while(save_cnt_wrong > -1 && running){
            ktra = 0;
            for(int i = 0; i < ROWS; ++i)
            {
                for(int j = 0; j < COLS; j++)
                {
                    if(board[i][j] == -1)
                    {
                        ktra = 1;
                        break;
                    }
                }
                if(ktra == 1) break;
            }
            if(ktra == 1)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                SDL_Texture* plane = nullptr;
                SDL_Texture* rainbow = nullptr;
                SDL_Texture* gameWin = nullptr;

                plane = LoadTexture("plane.png", renderer);
                rainbow = LoadTexture("rainbow.png", renderer);
                gameWin = LoadTexture("game_win.png", renderer);


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
                        if (event.type == SDL_QUIT) return 0;
                        else if(event.type == SDL_MOUSEBUTTONDOWN)
                        {
                            if(event.button.x >= 475 && event.button.x <= 525 && event.button.y >= 620 && event.button.y <= 670)
                            {
                                Mix_PlayChannel(-1, sound_click, 0);
                                Mix_VolumeChunk(sound_click, 128);
                                runningg = 0;
                                break;
                            }
                        }
                    }
                    
                    // Di chuyển máy bay
                    if (planeX < screenWidth) {
                        planeX += 10;
                        if(planeX >= -100) rainbowX += 10;
                    } else {
                        gameWinFalling = true;
                    }

                    bool check = 0;
                    
                    // Hiệu ứng chữ "Game Over" rơi xuống
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
                        else
                        {
                            check = 1;
                            // Mix_PlayChannel(-1, sound_click, 0);
                            // Mix_VolumeChunk(sound_click, 128);
                        }
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
                    }
                    SDL_RenderPresent(renderer);
                    SDL_Delay(16);
                }
                if(runningg == 0)
                {
                    cnt_help = 5;
                    restart = 1;
                    break;
                }
            }
            if(save_cnt_wrong == 0)
            {

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                SDL_Texture* plane = nullptr;
                SDL_Texture* rainbow = nullptr;
                SDL_Texture* gameOver = nullptr;

                plane = LoadTexture("plane.png", renderer);
                rainbow = LoadTexture("rainbow.png", renderer);
                gameOver = LoadTexture("game_over.png", renderer);


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
                        if (event.type == SDL_QUIT) return 0;
                        else if(event.type == SDL_MOUSEBUTTONDOWN)
                        {
                            if(event.button.x >= 475 && event.button.x <= 525 && event.button.y >= 620 && event.button.y <= 670)
                            {
                                Mix_PlayChannel(-1, sound_click, 0);
                                Mix_VolumeChunk(sound_click, 128);
                                runningg = 0;
                                break;
                            }
                        }
                    }
                    
                    // Di chuyển máy bay
                    if (planeX < screenWidth) {
                        planeX += 10;
                        if(planeX >= -100) rainbowX += 10;
                    } else {
                        gameOverFalling = true;
                    }

                    bool check = 0;
                    
                    // Hiệu ứng chữ "Game Over" rơi xuống
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
                        else
                        {
                            check = 1;
                            // Mix_PlayChannel(-1, sound_click, 0);
                            // Mix_VolumeChunk(sound_click, 128);
                        }
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
                    }
                    SDL_RenderPresent(renderer);
                    SDL_Delay(16);
                }
                if(runningg == 0)
                {
                    cnt_help = 5;
                    restart = 1;
                    break;
                }
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
                            if (SDL_GetTicks() - pauseStart >= 300) // Đã qua 300ms?
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
                            // std::ofstream logFile("debug.log", std::ios::app);
                            // logFile << cnt << std::endl;

                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderClear(renderer);
                            RenderBoard_Help(save_help, save_cnt_wrong);
                            SDL_RenderPresent(renderer);
                            if(SDL_GetTicks() - pauseStart > 1000) check_change = 1;
                            // logFile.close();
                        }

                        // check_help = 0;
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