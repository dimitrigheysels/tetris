#pragma once

#include <memory>

#include "ui.h"

class PlayerProfile
{
private:
    int highscore_;

public:
    PlayerProfile();

    void update_highscore(int score);

    void display(const UI &ui) const;
};