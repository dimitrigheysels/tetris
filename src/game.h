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
    bool is_game_over_;

    std::shared_ptr<Player> player_;

    int score_;
    int nr_of_lines_;

    // levels:
    //  1. normal
    //  2. randomly add fixed tiles (max 2? lines above current height AND height lower than half? of field)
    //  3. randomly remove fixed tiles (max 2? lines above current height AND height lower than half? of field)
    //  4. combine 3. and 4.
    //  5. other, more difficult set of blocks (disable 2. 3. 4.)
    //  6. randomly add fixed tiles (max 5? lines above current height AND height lower than half? of field)
    //  7. randomly remove fixed tiles (max 5? lines above current height AND height lower than half? of field)
    //  8. combine 6. and 7.
    //  9. scramble whole field
    int level_;

    void process_game_state(const GameState &state);

public:
    Game();

    bool is_running();

    void update();
    void update(const sf::Event &event);

    void update_score(int nr_of_full_lines);
    void update_level();
    void game_over();

    void display(const UI &ui) const;
};