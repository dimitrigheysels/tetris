#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "player_profile.h"
#include "field.h"
#include "level.h"
#include "sound_manager.h"

class Game : public std::enable_shared_from_this<Game>
{
private:
    sf::Clock level_clock_;
    sf::Clock block_clock_;

    bool is_running_;
    bool is_paused_;
    bool is_game_over_;

    std::shared_ptr<Field> field_;
    std::shared_ptr<PlayerProfile> player_;

    int score_;
    int nr_of_lines_;

    std::shared_ptr<Level> level_;
    int level_event_countdown_;

    int next_nr_of_lines_bonus_;

    std::shared_ptr<SoundManager> sm_;

    sf::Font font_;

    FieldDescription field_description_;

    void process_game_state(const GameState &state);

    void render_gameover(const std::shared_ptr<sf::RenderWindow> window) const;
    void render_scoreboard(const std::shared_ptr<sf::RenderWindow> window) const;
    void render_level_countdown(const std::shared_ptr<sf::RenderWindow> window) const;

public:
    Game();

    bool is_running() const;

    void start_new_game();

    void update(std::optional<sf::Event> event);

    void update_score(int nr_of_full_lines);
    void update_level();
    void game_over();

    // void display(const UI &ui) const;
    void display(const std::shared_ptr<sf::RenderWindow> &window) const;
};