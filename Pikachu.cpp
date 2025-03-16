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
#include <sstream>
#include <fstream>
#include <thread>


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

int timee = 0;

std::string point = "";
std::string playerName = "";

void removeLast() {
    std::ifstream inFile("player.txt");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();

    lines.pop_back();

    std::ofstream outFile("player.txt", std::ios::trunc);

    for(int i = 0; i < lines.size(); i++)
    {
        outFile << lines[i] << std::endl;
    }

    outFile.close();
}

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

std::string removeLeadingZeros(std::string& str) {
    size_t firstNonZero = str.find_first_not_of('0'); // Tìm vị trí số đầu tiên khác '0'
    if (firstNonZero != std::string::npos) {
        return str.substr(firstNonZero); // Cắt chuỗi từ vị trí này
    }
    return "0"; // Nếu toàn bộ chuỗi là '0', trả về "0"
}

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
            if(board[i][j] == -1){
                continue;
            }
            else if(board[i][j] == -2)
            {
                SDL_Rect tiles = {DISTANCE_W + j * TILE_SIZE, DISTANCE_H + i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_RenderCopy(renderer, textures_red[save_color[save_board[i][j]]], NULL, &tiles);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &tiles);
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

SDL_Rect buttonPlay = {300, 300, 400, 120};
SDL_Rect buttonHelp = {300, 450, 400, 120};
SDL_Rect buttonScore = {300, 600, 400, 120};

bool enterPressed = false;
bool isRunning = true, isRun = 0;

bool isLeaderboardOpen = false;
bool isTutorial = false;

bool next = 0;

void Rank_present(std::string x, int pos, int pos1)
{
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font_number_rank, x.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(leaderboardRenderer, surface);
    SDL_Rect dstRect = {pos, pos1, surface->w, surface->h};
    SDL_RenderCopy(leaderboardRenderer, texture, NULL, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Present_rank1(std::string num, std::string name, int score, int w)
{
    Rank_present(num, 87 + w, 112);
    Rank_present(name, 138 + w, 112);
    std::string score_s = std::to_string(score);
    if(score_s.size() == 3) w += 14;
    else if(score_s.size() == 2) w += 26;
    else if(score_s.size() == 1) w += 47;
    Rank_present(std::to_string(score), 210 + w, 72);
}

void Present_rank2(std::string num, std::string name, int score, int w)
{
    Rank_present(num, 87 + w, 270);
    Rank_present(name, 138 + w, 270);
    std::string score_s = std::to_string(score);
    if(score_s.size() == 3) w += 14;
    else if(score_s.size() == 2) w += 26;
    else if(score_s.size() == 1) w += 47;
    Rank_present(std::to_string(score), 210 + w, 230);
}

void Present_rank3(std::string num, std::string name, int score, int w)
{
    Rank_present(num, 87 + w, 428);
    Rank_present(name, 138 + w, 428);
    std::string score_s = std::to_string(score);
    if(score_s.size() == 3) w += 14;
    else if(score_s.size() == 2) w += 26;
    else if(score_s.size() == 1) w += 47;
    Rank_present(std::to_string(score), 210 + w, 388);
}

void Present_rank4(std::string num, std::string name, int score, int w)
{
    Rank_present(num, 87 + w, 610);
    Rank_present(name, 138 + w, 610);
    std::string score_s = std::to_string(score);
    if(score_s.size() == 3) w += 14;
    else if(score_s.size() == 2) w += 26;
    else if(score_s.size() == 1) w += 47;
    Rank_present(std::to_string(score), 210 + w, 570);
}

void showLeaderboardWindow(const std::string &filename) {
    if (isLeaderboardOpen) return;

    leaderboardWindow = SDL_CreateWindow("Leaderboard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1500, 750, SDL_WINDOW_SHOWN);
    leaderboardRenderer = SDL_CreateRenderer(leaderboardWindow, -1, SDL_RENDERER_ACCELERATED);

    RANK = LoadTexture("ranking.png", leaderboardRenderer);

    isLeaderboardOpen = true;
    // Đọc dữ liệu từ file
    std::vector<std::pair<std::string, int>> players;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name;
        int score;
        if (std::getline(ss, name, ',') && ss >> score) {
            players.push_back({name, score});
        }
    }
    file.close();
    std::sort(players.begin(), players.end(), [](const auto &a, const auto &b) { return a.second > b.second; });

    while (isLeaderboardOpen) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)) {
                isLeaderboardOpen = false;
            }
        }
        
        SDL_SetRenderDrawColor(leaderboardRenderer, 0, 0, 0, 255);
        SDL_RenderClear(leaderboardRenderer);
        
        SDL_Rect SAVE = {0, 0, 1500, 750};
        SDL_RenderCopy(leaderboardRenderer, RANK, NULL, &SAVE);

        int cnt = 0;

        while(cnt < 12 && cnt < players.size())
        {
            cnt++;
            if(cnt == 4)
            {
                Present_rank1("4", players[cnt - 1].f, players[cnt - 1].s, 0);
            }
            if(cnt == 8)
            {
                Present_rank1("8", players[cnt - 1].f, players[cnt - 1].s, 481);
            }
            if(cnt == 12)
            {
                Present_rank1("12", players[cnt - 1].f, players[cnt - 1].s, 481 * 2);
            }
            else if(cnt == 3)
            {
                Present_rank2("3", players[cnt - 1].f, players[cnt - 1].s, 0);
            }
            else if(cnt == 7)
            {
                Present_rank2("7", players[cnt - 1].f, players[cnt - 1].s, 481);
            }
            else if(cnt == 11)
            {
                Present_rank2("11", players[cnt - 1].f, players[cnt - 1].s, 481 * 2);
            }
            else if(cnt == 2)
            {
                Present_rank3("2", players[cnt - 1].f, players[cnt - 1].s, 0);
            }
            else if(cnt == 6)
            {
                Present_rank3("6", players[cnt - 1].f, players[cnt - 1].s, 481);
            }
            else if(cnt == 10)
            {
                Present_rank3("10", players[cnt - 1].f, players[cnt - 1].s, 481 * 2);
            }
            else if(cnt == 1)
            {
                Present_rank4("1", players[cnt - 1].f, players[cnt - 1].s, 0);
            }
            else if(cnt == 5)
            {
                Present_rank4("5", players[cnt - 1].f, players[cnt - 1].s, 481);
            }
            else if(cnt == 9)
            {
                Present_rank4("9", players[cnt - 1].f, players[cnt - 1].s, 481 * 2);
            }
        }

        SDL_RenderPresent(leaderboardRenderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(leaderboardRenderer);
    SDL_DestroyWindow(leaderboardWindow);
    leaderboardWindow = nullptr;
    leaderboardRenderer = nullptr;
    isLeaderboardOpen = false;

}

void RenderTutorial(TTF_Font* font) {

    SDL_RenderCopy(tutorialboardRenderer, Back_ground_tutorial, NULL, NULL);

    SDL_Rect Rende = {300, 0, 400, 100};
    SDL_RenderCopy(tutorialboardRenderer, Tutorial_Render, NULL, &Rende);

    SDL_Color textColor = {255, 255, 255, 255};
    int x = 50, y = 90;
    int spacing = 20; 
    int iconSize = 50;
    int maxWidth = 1000; 
    int cnt_Tutorial = 0;
    
    struct TextSegment {
        std::string text;
        SDL_Texture* icon;
    };
    
    std::vector<TextSegment> tutorialText = {
        {"This game is exactly like the classic Pokemon Puzzle but with slightly easier gameplay.", nullptr},
        {"The gameplay is simple as follows:", nullptr},
        {"Instead of requiring 1-2 bends like in the classic game, this game ONLY requires a valid path between two matching tiles. Of course, paths along the outer edges of the board are also allowed.", nullptr},
        {"In each level, you have 5 wrong attempts and 5 hints ", Help_Tutorial},
        {"The game duration is 20 minutes. You can pause the game ", Pause_Tutorial},
        {"or restart it if you want", Restart_Tutorial},
        {"As your score is only recorded when you win or lose.", nullptr},
        {"When the game ends, you can replay by pressing ", Restart_Tutorial},
        {"Enjoy the game! Have fun!.", nullptr},
        {"(Bonus: Click the 'X' in the top right corner to close the ranking board and tutorial board.)", nullptr}
    };
    
    for (auto& segment : tutorialText) {
        int currentX = 10;

        if (segment.icon) {
            cnt_Tutorial++;
            if(cnt_Tutorial == 1)
            {
                SDL_Rect iconRect = {720, y - 5, 50, 50};
                SDL_RenderCopy(tutorialboardRenderer, segment.icon, NULL, &iconRect);
            }
            else if(cnt_Tutorial == 2)
            {
                SDL_Rect iconRect = {805, y - 5, 50, 50};
                SDL_RenderCopy(tutorialboardRenderer, segment.icon, NULL, &iconRect);
            }
            else if(cnt_Tutorial == 3)
            {
                SDL_Rect iconRect = {315, y - 5, 50, 50};
                SDL_RenderCopy(tutorialboardRenderer, segment.icon, NULL, &iconRect);
            }
            else if(cnt_Tutorial == 4)
            {
                SDL_Rect iconRect = {680, y - 5, 50, 50};
                SDL_RenderCopy(tutorialboardRenderer, segment.icon, NULL, &iconRect);
            }
        }
        
        if (!segment.text.empty()) {
            SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, segment.text.c_str(), textColor, maxWidth);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(tutorialboardRenderer, textSurface);
            SDL_Rect textRect = {currentX, y, textSurface->w, textSurface->h};
            SDL_RenderCopy(tutorialboardRenderer, textTexture, NULL, &textRect);
            y += textSurface->h + spacing;
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }

    }
    SDL_RenderPresent(tutorialboardRenderer);
}


void showTutorialboardWindow()
{
    if (isTutorial) return;

    tutorialboardWindow = SDL_CreateWindow("Tutorialboard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, SDL_WINDOW_SHOWN);
    tutorialboardRenderer = SDL_CreateRenderer(tutorialboardWindow, -1, SDL_RENDERER_ACCELERATED);
    
    Tutorial_Render = LoadTexture("Tutorial_render.png", tutorialboardRenderer);
    Restart_Tutorial = LoadTexture("restart.png", tutorialboardRenderer);
    Help_Tutorial = LoadTexture("light.png", tutorialboardRenderer);
    Pause_Tutorial = LoadTexture("pause_game.png", tutorialboardRenderer);
    Back_ground_tutorial = LoadTexture("back_ground_tutorial.png", tutorialboardRenderer);

    isTutorial = true;

    while(isTutorial)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)) {
                isTutorial = false;
            }
        }
        RenderTutorial(font_tutorial);
    }

    SDL_DestroyRenderer(tutorialboardRenderer);
    SDL_DestroyWindow(tutorialboardWindow);
    tutorialboardWindow = nullptr;
    tutorialboardRenderer = nullptr;
    isTutorial = false;
    
}

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
                if(playerName.size() != 0 && enterPressed == 1)
                {
                    Mix_PlayChannel(-1, sound_click, 0);
                    Mix_VolumeChunk(sound_click, 128);
                    isRunning = false;  // Nhấn Play -> Dừng vòng lặp
                }
                else
                {
                    Mix_PlayChannel(-1, sound_wrong_click, 0);
                    Mix_VolumeChunk(sound_wrong_click, 128);
                }
            }
            else if (x >= buttonScore.x && x <= buttonScore.x + buttonScore.w && y >= buttonScore.y && y <= buttonScore.y + buttonScore.h)
            {
                Mix_PlayChannel(-1, sound_click, 0);
                Mix_VolumeChunk(sound_click, 128);
                if (!isLeaderboardOpen) {
                    std::thread leaderboardThread(showLeaderboardWindow, "player.txt");
                    leaderboardThread.detach(); // Chạy song song mà không chặn chương trình chính
                }
            }
            else if(x >= buttonHelp.x && x <= buttonHelp.x + buttonHelp.w && y >= buttonHelp.y && y <= buttonHelp.y + buttonHelp.h)
            {
                Mix_PlayChannel(-1, sound_click, 0);
                Mix_VolumeChunk(sound_click, 128);
                if (!isTutorial) {
                    std::thread leaderboardThread(showTutorialboardWindow);
                    leaderboardThread.detach();
                }
            }
        }
    }
}

void RenderText(SDL_Renderer *renderer, TTF_Font *font) {
    int defaultBoxWidth = 400, boxHeight = 50;
    int minBoxWidth = 300;
    int maxBoxWidth = 500;
    int boxX = 500 - defaultBoxWidth / 2, boxY = 200;

    if (playerName.empty()) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect inputBox = {boxX, boxY, defaultBoxWidth, boxHeight};
        SDL_RenderFillRect(renderer, &inputBox);

        SDL_Color hintColor = {100, 100, 100, 255}; // Màu xám
        SDL_Surface *hintSurface = TTF_RenderText_Solid(font, "Name and Enter", hintColor);
        if (hintSurface) {
            SDL_Texture *hintTexture = SDL_CreateTextureFromSurface(renderer, hintSurface);

            int textWidth = hintSurface->w;
            int textHeight = hintSurface->h;
            SDL_Rect hintRect = {boxX + (defaultBoxWidth - textWidth) / 2, boxY + (boxHeight - textHeight) / 2, textWidth, textHeight};
            SDL_RenderCopy(renderer, hintTexture, NULL, &hintRect);
            SDL_FreeSurface(hintSurface);
            SDL_DestroyTexture(hintTexture);
        }

        static int lastBlinkTime = 0;
        static bool showCursor = true;
        int currentTime = SDL_GetTicks();
        if (currentTime > lastBlinkTime + 700) {
            showCursor = !showCursor;
            lastBlinkTime = currentTime;
        }
        if (showCursor) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Rect cursor = {boxX + 8, boxY + 5, 2, 40};
            SDL_RenderFillRect(renderer, &cursor);
        }
        return;
    }

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, playerName.c_str(), textColor);
    if (!textSurface) return;
    int textWidth = textSurface->w, textHeight = textSurface->h;

    int boxWidth = std::max(minBoxWidth, std::min(maxBoxWidth, textWidth + 20));
    boxX = 500 - boxWidth / 2;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect inputBox = {boxX, boxY, boxWidth, boxHeight};
    SDL_RenderFillRect(renderer, &inputBox);

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {boxX + 10, boxY + (boxHeight - textHeight) / 2, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    if(enterPressed == 0)
    {
        static Uint32 lastBlinkTime = 0;
        static bool showCursor = true;
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastBlinkTime + 500) {
            showCursor = !showCursor;
            lastBlinkTime = currentTime;
        }

        if (showCursor) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Rect cursor = {textRect.x + textWidth + 1, textRect.y + 5, 2, textHeight - 12};
            SDL_RenderFillRect(renderer, &cursor);
        }
    }
}

int main(int argc, char* argv[]) {
    srand(0);
    initSDL(window, renderer);
    mixer();
    TTF_Init();

    font = TTF_OpenFont("orbitron-black.otf", 35);
    font_point = TTF_OpenFont("orbitron-black.otf", 70);
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
    sound_correct_click = Mix_LoadWAV("correct.wav");;
    sound_error_click = Mix_LoadWAV("click-error.wav");;

    font_number_rank = TTF_OpenFont("orbitron-black.otf", 40);

    Back_ground_start = LoadTexture("space_art.png", renderer);

    Button_Play = LoadTexture("button_play.png", renderer);
    Button_Help = LoadTexture("button_help.png", renderer);
    Button_Score = LoadTexture("button_rank.png", renderer);

    music_background = Mix_LoadMUS("start.mp3");
    sound_click = Mix_LoadWAV("select_click.wav");
    sound_wrong_click = Mix_LoadWAV("wrong.wav");

    font_name = TTF_OpenFont("arial.ttf", 35);
    font_tutorial = TTF_OpenFont("arial.ttf", 30);

    Mix_PlayMusic(music_background, -1);
    Mix_VolumeMusic(50);
    SDL_Event events;
    while (isRunning) {
        HandleEvents(events);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, Back_ground_start, NULL, NULL);
        SDL_RenderCopy(renderer, Button_Play, NULL, &buttonPlay);
        SDL_RenderCopy(renderer, Button_Help, NULL, &buttonHelp);
        SDL_RenderCopy(renderer, Button_Score, NULL, &buttonScore);
        RenderText(renderer, font_name);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
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
    Mix_PauseMusic();
    while((isRun == 0 && running == 1) || restart == 1){
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
        timee = 0;
        while(save_cnt_wrong > -1 && running){
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
                            else if(event.button.x >= 400 && event.button.x <= 600 && event.button.y >= 700 && event.button.y <= 760)
                            {
                                Mix_PlayChannel(-1, sound_click, 0);
                                Mix_VolumeChunk(sound_click, 128);
                                if (!isLeaderboardOpen) {
                                    std::thread leaderboardThread(showLeaderboardWindow, "player.txt");
                                    leaderboardThread.detach(); // Chạy song song mà không chặn chương trình chính
                                }
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

                        tiless = {400, 700, 200, 60};
                        SDL_RenderCopy(renderer, Button_Score, NULL, &tiless);
                    }

                    if(check)
                    {

                        if(timee == 0)
                        {
                            std::ofstream file("Player.txt", std::ios::app); // Mở file ở chế độ append
                            if (file.is_open()) {
                                file << playerName << "," << point << std::endl; // Ghi tên vào dòng mới
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
                    break;
                }
            }
            if(save_cnt_wrong == 0 || SDL_GetTicks() - startTime > 1200000 + 1000)
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
                            else if(event.button.x >= 400 && event.button.x <= 600 && event.button.y >= 700 && event.button.y <= 760)
                            {
                                Mix_PlayChannel(-1, sound_click, 0);
                                Mix_VolumeChunk(sound_click, 128);
                                if (!isLeaderboardOpen) {
                                    std::thread leaderboardThread(showLeaderboardWindow, "player.txt");
                                    leaderboardThread.detach(); // Chạy song song mà không chặn chương trình chính
                                }
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

                        tiless = {400, 700, 200, 60};
                        SDL_RenderCopy(renderer, Button_Score, NULL, &tiless);
                    }
                    if(check)
                    {
                        if(timee == 0)
                        {
                            std::ofstream file("Player.txt", std::ios::app); // Mở file ở chế độ append
                            if (file.is_open()) {
                                file << playerName << "," << "0" << std::endl; // Ghi tên vào dòng mới
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