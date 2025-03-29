#include "Declare.h"
#include "InitSDL.h"

SDL_Window* tutorialboardWindow = nullptr;
SDL_Renderer* tutorialboardRenderer = nullptr;
SDL_Window* leaderboardWindow = nullptr;
SDL_Renderer* leaderboardRenderer = nullptr;
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
SDL_Texture* plane = nullptr;
SDL_Texture* rainbow = nullptr;
SDL_Texture* gameOver = nullptr;
SDL_Texture* gameWin = nullptr;
SDL_Texture* Help_Tutorial = nullptr;
SDL_Texture* Pause_Tutorial = nullptr;
SDL_Texture* Restart_Tutorial = nullptr;
SDL_Texture* Tutorial_Render = nullptr;
SDL_Texture* Back_ground_tutorial = nullptr;
SDL_Texture* Music_On = nullptr;
SDL_Texture* Music_Off = nullptr;
SDL_Texture* Restart_Game = nullptr;
SDL_Texture* plane_red = nullptr;
SDL_Texture* Start_game_image = nullptr;

Mix_Music* music = nullptr;
Mix_Music* music_win = nullptr;
Mix_Music* music_background = nullptr;
Mix_Chunk* Door_reverse = nullptr;
Mix_Chunk* plane_sound = nullptr;
Mix_Chunk* stone_sound = nullptr;
Mix_Chunk* sound_click = nullptr;
Mix_Chunk* sound_wrong_click = nullptr;
Mix_Chunk* sound_correct_click = nullptr;
Mix_Chunk* sound_error_click = nullptr;
Mix_Chunk* Pikachu_lose = nullptr;
Mix_Chunk* Pikachu_win = nullptr;
Mix_Chunk* Door_sound = nullptr;

std::string playerName = "";

int cnt_pr = 0;
int timee = 0;
int cnt_help = 5, save_cnt_wrong = 5;
int cnt = 0;
int lastUpdateTime, startTime, start_pause;
int save_color[1000];
int next_game = 0;
int music_check = 0;
int plane_x = -200;
int plane_red_x = -200; 
const int screen_end = 1000; 
const int delay_between_planes = 70; 
int before_menu = 0;
int start_game = -800;

bool check[100][100];
bool check_help = 0;
bool win = 0;
bool enterPressed = false;
bool isRunning = true, isRun = 0;
bool isLeaderboardOpen = false;
bool isTutorial = false;
bool click_tutorial = false;
bool next = 0;
bool running = true;
bool restart = 0;

std::pair < int, int > first_click, second_click;
std::vector< std::vector<int> > board, save_board;
std::vector < std::string > Time_present;
std::string point = "";

SDL_Rect buttonPlay = {244, 300, 512, 115};
SDL_Rect buttonHelp = {244, 445, 512, 121};
SDL_Rect buttonScore = {244, 600, 512, 138};

//Khai bao sound
void Mix_FreeChunks()
{
    Mix_FreeChunk(sound_click);
    Mix_FreeChunk(sound_correct_click);
    Mix_FreeChunk(sound_error_click);
    Mix_FreeChunk(sound_wrong_click);
}

//Khai bao bien
void Declare_variable()
{
    font = TTF_OpenFont("Font/orbitron-black.otf", 35);
    font_point = TTF_OpenFont("Font/orbitron-black.otf", 70);
    font_times = TTF_OpenFont("Font/orbitron-black.otf", 22);
    font_number = TTF_OpenFont("Font/arial.ttf", 35);
    font_number_rank = TTF_OpenFont("Font/orbitron-black.otf", 40);
    font_name = TTF_OpenFont("Font/arial.ttf", 35);
    font_tutorial = TTF_OpenFont("Font/arial.ttf", 30);

    Restart = LoadTexture("Picture_game/restart.png", renderer);
    Resume = LoadTexture("Picture_game/resume_game.png", renderer);
    Pause = LoadTexture("Picture_game/pause_game.png", renderer);
    Heart = LoadTexture("Picture_game/heart.png", renderer);
    Black = LoadTexture("Picture_game/black.png", renderer);
    HELP = LoadTexture("Picture_game/light.png", renderer);
    Back_ground = LoadTexture("Picture_game/Back_ground.png", renderer);
    Time = LoadTexture("Picture_game/Time.png", renderer);
    Back_ground_start = LoadTexture("Picture_game/space_art.png", renderer);
    Button_Play = LoadTexture("Picture_game/button_play.png", renderer);
    Button_Help = LoadTexture("Picture_game/button_help.png", renderer);
    Button_Score = LoadTexture("Picture_game/button_rank.png", renderer);
    plane = LoadTexture("Picture_game/plane.png", renderer);
    rainbow = LoadTexture("Picture_game/rainbow.png", renderer);
    gameWin = LoadTexture("Picture_game/game_win.png", renderer);
    gameOver = LoadTexture("Picture_game/game_over.png", renderer);
    RANK = LoadTexture("Picture_game/ranking.png", leaderboardRenderer);
    Music_On = LoadTexture("Picture_game/music_on.png", renderer);
    Music_Off = LoadTexture("Picture_game/music_off.png", renderer);
    Restart_Game = LoadTexture("Picture_game/restart_game.png", renderer);
    plane_red = LoadTexture("Picture_game/plane_red.png", renderer);
    Start_game_image = LoadTexture("Picture_game/start_game.png", renderer);

    music = Mix_LoadMUS("Music/background.mp3");
    music_background = Mix_LoadMUS("Music/start.mp3");
    Door_reverse = Mix_LoadWAV("Sound/door_sound_reverse.wav");
    plane_sound = Mix_LoadWAV("Sound/plane_sound.wav");
    stone_sound = Mix_LoadWAV("Sound/stone_sound.wav");
    sound_correct_click = Mix_LoadWAV("Sound/correct.wav");;
    sound_error_click = Mix_LoadWAV("Sound/click-error.wav");
    sound_click = Mix_LoadWAV("Sound/select_click.wav");
    sound_wrong_click = Mix_LoadWAV("Sound/wrong.wav");
    Pikachu_lose = Mix_LoadWAV("Sound/sound_lose_pikachu.wav");
    Pikachu_win = Mix_LoadWAV("Sound/sound_win_pikachu.wav");
    Door_sound = Mix_LoadWAV("Sound/door_sound.wav");

    for (int i = 1; i <= 40; i++) {
        std::string filename = "Picture_game/object_" + std::to_string(i) + ".png";
        textures.push_back(LoadTexture(filename, renderer));
    }

    for (int i = 1; i <= 40; i++) {
        std::string filename = "Picture_game/object_" + std::to_string(i) + "_red_tint_50x50.png";
        textures_red.push_back(LoadTexture(filename, renderer));
    }

    for (int i = 1; i <= 40; i++) {
        std::string filename = "Picture_game/object_" + std::to_string(i) + "_green_overlay.png";
        textures_green.push_back(LoadTexture(filename, renderer));
    }
}