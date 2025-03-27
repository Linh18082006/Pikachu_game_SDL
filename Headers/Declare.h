#ifndef DECLARE_H 
#define DECLARE_H
#define f first
#define s second
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>
#include <queue>
#include <string>
#include <windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <chrono>

extern SDL_Window* tutorialboardWindow;
extern SDL_Renderer* tutorialboardRenderer;
extern SDL_Window* leaderboardWindow;
extern SDL_Renderer* leaderboardRenderer;
extern TTF_Font* font;
extern TTF_Font* font_times;
extern TTF_Font* font_number;
extern TTF_Font* font_point;
extern TTF_Font* font_name;
extern TTF_Font* font_number_rank;
extern TTF_Font* font_tutorial;

extern std::vector<SDL_Texture*> textures, textures_red, textures_green;
extern SDL_Texture* RANK;
extern SDL_Texture* HELP;
extern SDL_Texture* Back_ground;
extern SDL_Texture* Black;
extern SDL_Texture* Heart;
extern SDL_Texture* Time;
extern SDL_Texture* Resume;
extern SDL_Texture* Pause;
extern SDL_Texture* Restart;
extern SDL_Texture* Back_ground_start;
extern SDL_Texture* Button_Play;
extern SDL_Texture* Button_Help;
extern SDL_Texture* Button_Score;
extern SDL_Texture* plane;
extern SDL_Texture* rainbow;
extern SDL_Texture* gameOver;
extern SDL_Texture* gameWin;
extern SDL_Texture* Help_Tutorial;
extern SDL_Texture* Pause_Tutorial;
extern SDL_Texture* Restart_Tutorial;
extern SDL_Texture* Tutorial_Render;
extern SDL_Texture* Back_ground_tutorial;
extern SDL_Texture* Music_On;
extern SDL_Texture* Music_Off;
extern SDL_Texture* Restart_Game;

extern Mix_Music* music;
extern Mix_Music* music_win;
extern Mix_Music* music_background;
extern Mix_Chunk* plane_sound;
extern Mix_Chunk* stone_sound;
extern Mix_Chunk* sound_click;
extern Mix_Chunk* sound_wrong_click;
extern Mix_Chunk* sound_correct_click;
extern Mix_Chunk* sound_error_click;
extern Mix_Chunk* Pikachu_lose;
extern Mix_Chunk* Pikachu_win;

extern const int ROWS;
extern const int COLS;
extern const int DISTANCE_H;
extern const int DISTANCE_W;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int TIMER_WIDTH;
extern const int TIMER_HEIGHT;
extern const int GAME_TIME;
extern const int TILE_SIZE;

extern std::string playerName;

extern int cnt_pr;
extern int timee;
extern int cnt_help, save_cnt_wrong;
extern int cnt;
extern int lastUpdateTime, startTime, start_pause;
extern int save_color[1000];
extern int next_game;
extern int music_check;

extern bool check[100][100];
extern bool check_help;
extern bool win;
extern bool enterPressed;
extern bool isRunning, isRun;
extern bool isLeaderboardOpen;
extern bool isTutorial;
extern bool click_tutorial;
extern bool next;
extern bool running;
extern bool restart;

extern std::pair<int, int> first_click, second_click;
extern std::vector<std::vector<int>> board, save_board;
extern std::vector<std::string> Time_present;
extern std::string point;

extern SDL_Rect buttonPlay;
extern SDL_Rect buttonHelp;
extern SDL_Rect buttonScore;

//Khai bao sound
void Mix_FreeChunks();

//Khai bao bien
void Declare_variable();

#endif 