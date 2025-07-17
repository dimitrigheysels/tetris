#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "player.h"

class UI;

class Game : public std::enable_shared_from_this<Game>
{
private:
    bool is_running_;
    bool is_paused_;

    std::shared_ptr<Player> player_;

    void process_game_state(const GameState &state);

public:
    Game();

    bool is_running();

    void update();
    void update(const sf::Event &event);

    void game_over();

    void render(const UI &ui) const;
};