#include <fstream>

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

void PlayerProfile::update_highlines(int lines)
{
    if (lines > highlines_)
    {
        highlines_ = lines;
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
    out.write(reinterpret_cast<const char *>(&highlines_), sizeof(highlines_));
    out.flush();
    out.close();
}

void PlayerProfile::load()
{
    std::ifstream in(player_profile_path, std::ios::in | std::ios::binary);
    in.read(reinterpret_cast<char *>(&highscore_), sizeof(highscore_));
    in.read(reinterpret_cast<char *>(&highlines_), sizeof(highlines_));
    in.close();
}
