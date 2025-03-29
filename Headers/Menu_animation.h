#ifndef MENU_ANIMATION_H 
#define MENU_ANIMATION_H 

#include "Declare.h"
#include "RunFunction.h"

void menu_animation(SDL_Renderer* renderer) {
    int check = 0;
    plane_x += 5;

    SDL_Rect tile = {plane_x, 20, 200, 200}; 
    SDL_RenderCopy(renderer, plane, NULL, &tile); 
    
    if (plane_x > screen_end + delay_between_planes + 200) {
        plane_x = -200; 
        check = 1;
    }

    if (plane_x > delay_between_planes || (plane_x < 0 && check == 1)) {
        plane_red_x += 5;
        SDL_Rect tiles = {plane_red_x, 43, 200, 125}; 
        SDL_RenderCopy(renderer, plane_red, NULL, &tiles);
        if (plane_red_x > screen_end) {
            plane_red_x = -200; 
        }
    }

    SDL_Delay(16); 
}

#endif