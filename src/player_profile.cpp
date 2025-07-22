#include <fstream>
#include <iostream>

#include "player_profile.h"
#include "ui.h"

PlayerProfile::PlayerProfile() : highscore_(0)
{
}

void PlayerProfile::update_highscore(int score)
{
    if (score > highscore_)
    {
        highscore_ = score;
    }
}

void PlayerProfile::display(const UI &ui) const
{

    ui.render_highscore(highscore_);
}

void PlayerProfile::save() const
{
    std::ofstream out(player_profile_path, std::fstream::out | std::fstream::binary | std::fstream::trunc);
    out.write(reinterpret_cast<const char *>(&highscore_), sizeof(highscore_));
    out.flush();
    out.close();
}

void PlayerProfile::load()
{
    std::ifstream in(player_profile_path, std::ios::in | std::ios::binary);
    in.read(reinterpret_cast<char *>(&highscore_), sizeof(highscore_));
    in.close();
}
