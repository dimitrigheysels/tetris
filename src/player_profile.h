#pragma once

#include <memory>
#include <filesystem>
#include <SFML/Graphics.hpp>

const std::filesystem::path player_profile_path = "./tetris.conf";

class UI;

class PlayerProfile
{
private:
    int highscore_;
    int highlines_;

    sf::Font font_;

    void render_highscore(const std::shared_ptr<sf::RenderWindow> &window) const;

public:
    PlayerProfile();

    void update_highscore(int score);
    void update_highlines(int lines);

    void display(const std::shared_ptr<sf::RenderWindow> &window) const;

    void save() const;
    void load();
};