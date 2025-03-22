#ifndef INITSDL_H  
#define INITSDL_H 

#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const std::string WINDOW_TITLE;

//Bao loi
void logSDLError(std::ostream &os, const std::string &msg, bool fatal = false);

//Mo cua so Window va Audio
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);

//Tai anh len
SDL_Texture* LoadTexture(const std::string &file, SDL_Renderer* renderer);

//Thoat va dong window
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

#endif 
