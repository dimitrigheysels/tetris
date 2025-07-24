#pragma once

#include <memory>
#include <filesystem>

const std::filesystem::path player_profile_path = "./tetris.conf";

class UI;

class PlayerProfile
{
private:
    int highscore_;
    int highlines_;

public:
    PlayerProfile();

    void update_highscore(int score);
    void update_highlines(int lines);

    void display(const UI &ui) const;

    void save() const;
    void load();
};