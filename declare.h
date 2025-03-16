#ifndef DECLARE_H 
#define DECLARE_H
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

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Window* leaderboardWindow = nullptr;
SDL_Renderer* leaderboardRenderer = nullptr;
SDL_Window* tutorialboardWindow = nullptr;
SDL_Renderer* tutorialboardRenderer = nullptr;
TTF_Font* font = nullptr;
TTF_Font* font_times = nullptr;
TTF_Font* font_number = nullptr;
TTF_Font* font_point = nullptr;
TTF_Font* font_name = nullptr;
TTF_Font* font_number_rank = nullptr;
TTF_Font* font_tutorial = nullptr;

std::vector<SDL_Texture*> textures, textures_red, textures_green;
SDL_Texture* RANK = nullptr;
SDL_Texture* HELP = nullptr;
SDL_Texture* Back_ground = nullptr;
SDL_Texture* Black = nullptr;
SDL_Texture* Heart = nullptr;
SDL_Texture* Time = nullptr;
SDL_Texture* Resume = nullptr;
SDL_Texture* Pause = nullptr;
SDL_Texture* Restart = nullptr;
SDL_Texture* Back_ground_start = nullptr;
SDL_Texture *Button_Play = nullptr;        
SDL_Texture *Button_Help = nullptr;        
SDL_Texture *Button_Score = nullptr;  
SDL_Texture* Help_Tutorial = nullptr;
SDL_Texture* Pause_Tutorial = nullptr;
SDL_Texture* Restart_Tutorial = nullptr;
SDL_Texture* Back_ground_tutorial = nullptr;
SDL_Texture* Tutorial_Render = nullptr;

Mix_Music* music = nullptr;
Mix_Music* music_win = nullptr;
Mix_Music* music_background = nullptr;
Mix_Chunk* plane_sound = nullptr;
Mix_Chunk* stone_sound = nullptr;
Mix_Chunk* sound_click = nullptr;
Mix_Chunk* sound_wrong_click = nullptr;
Mix_Chunk* sound_correct_click = nullptr;
Mix_Chunk* sound_error_click = nullptr;

void Mix_FreeChunks()
{
    Mix_FreeChunk(sound_click);
    Mix_FreeChunk(sound_correct_click);
    Mix_FreeChunk(sound_error_click);
    Mix_FreeChunk(sound_wrong_click);
}

std::vector<std::string> milliseconds_to_mmss(int milliseconds) {
    int total_seconds = milliseconds / 1000;
    int minutes = total_seconds / 60;
    int seconds = total_seconds % 60;
    
    std::vector<std::string> result;
    result.push_back(std::to_string(minutes / 10));
    result.push_back(std::to_string(minutes % 10));
    result.push_back(std::to_string(seconds / 10));
    result.push_back(std::to_string(seconds % 10));
    
    return result;
}

#endif 