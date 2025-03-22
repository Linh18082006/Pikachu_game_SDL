#ifndef RENDER_RANK_H 
#define RENDER_RANK_H
#include "Declare.h"
#include "InitSDL.h"

//Hien thi bang rank
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

    RANK = LoadTexture("Picture_game/ranking.png", leaderboardRenderer);
    
    isLeaderboardOpen = true;
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

#endif