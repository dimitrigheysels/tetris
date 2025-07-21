#include "player_profile.h"

PlayerProfile::PlayerProfile() : highscore_(0)
{
}

void PlayerProfile::display(const UI &ui) const
{

    ui.render_highscore(highscore_);
}

void PlayerProfile::update_highscore(int score)
{
    if (score > highscore_)
    {
        highscore_ = score;
    }
}