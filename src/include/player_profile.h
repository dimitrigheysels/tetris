#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

class PlayerProfile
{
private:
    int highscore_;
    int highlines_;

    void render_highscore(const std::shared_ptr<sf::RenderWindow> &window) const;

public:
    PlayerProfile();

    void update_highscore(int score);
    void update_highlines(int lines);

    void load();
    void save() const;

    void display(const std::shared_ptr<sf::RenderWindow> &window) const;
};