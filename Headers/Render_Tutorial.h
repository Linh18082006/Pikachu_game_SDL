#ifndef RENDER_TUTORIAL_H 
#define RENDER_TUTORIAL_H
#include "Declare.h"
#include "InitSDL.h"

//Hien thi bang huong dan

void RenderTutorial(TTF_Font* font) {

    Tutorial_Render = LoadTexture("Picture_game/Tutorial_render.png", tutorialboardRenderer);
    Back_ground_tutorial = LoadTexture("Picture_game/back_ground_tutorial.png", tutorialboardRenderer);
    Restart_Tutorial = LoadTexture("Picture_game/restart.png", tutorialboardRenderer);
    Help_Tutorial = LoadTexture("Picture_game/light.png", tutorialboardRenderer);
    Pause_Tutorial = LoadTexture("Picture_game/pause_game.png", tutorialboardRenderer);

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

#endif