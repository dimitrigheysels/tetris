#pragma once

#include <memory>
#include <filesystem>

const std::filesystem::path player_profile_path = "./tetris.conf";

class UI;

class PlayerProfile
{
private:
    int highscore_;

public:
    PlayerProfile();

    void update_highscore(int score);

    void display(const UI &ui) const;

    void save() const;
    void load();
};