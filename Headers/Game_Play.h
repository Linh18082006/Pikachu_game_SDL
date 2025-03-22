#ifndef GAME_PLAY_H 
#define GAME_PLAY_H
#include "Declare.h"

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

#endif