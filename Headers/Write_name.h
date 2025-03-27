#ifndef WRITE_NAME_H 
#define WRITE_NAME_H
#include "Declare.h"

//Hien thi ten va nhap ten menu
void RenderText(SDL_Renderer *renderer, TTF_Font *font) {
    int defaultBoxWidth = 400, boxHeight = 50;
    int boxX = 500 - defaultBoxWidth / 2, boxY = 200;

    if (playerName.empty()) {

        SDL_Rect inputBox = {boxX, boxY, defaultBoxWidth, boxHeight};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set color to black
        SDL_RenderFillRect(renderer, &inputBox); // Render black box

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set color to white
        SDL_RenderDrawRect(renderer, &inputBox); // Draw white border

        SDL_Color hintColor = {128, 128, 128, 255}; 
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

    SDL_Rect inputBox = {boxX, boxY, defaultBoxWidth, boxHeight};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set color to black
    SDL_RenderFillRect(renderer, &inputBox); // Render black box

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set color to white
    SDL_RenderDrawRect(renderer, &inputBox); // Draw white border

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {boxX + 10, boxY + (boxHeight - textSurface->h) / 2, textSurface->w, textSurface->h};
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
            SDL_Rect cursor = {textRect.x + textSurface->w + 1, textRect.y + 5, 2, textSurface->h - 12};
            SDL_RenderFillRect(renderer, &cursor);
        }
    }
}

#endif