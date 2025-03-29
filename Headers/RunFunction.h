#ifndef RUNFUNCTION_H 
#define RUNFUNCTION_H
#include "Declare.h"

//Doi toa do 
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

//Loai bo chu so khong dau tien cua points
std::string removeLeadingZeros(std::string& str) {
    size_t firstNonZero = str.find_first_not_of('0'); 
    if (firstNonZero != std::string::npos) {
        return str.substr(firstNonZero); 
    }
    return "0"; 
}

//Loai bo hang duoi cung file player.txt
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

//Doi tu mili giay sang phut giay
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

void PlaySoundEffect(Mix_Chunk* sound, int volume) {
    if (music_check == 0) { // Chỉ phát âm thanh nếu âm thanh đang bật
        Mix_PlayChannel(-1, sound, 0);
        Mix_VolumeChunk(sound, volume);
    }
}

#endif