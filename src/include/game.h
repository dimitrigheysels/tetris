#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "player_profile.h"
#include "field.h"
#include "level.h"
#include "menu.h"
#include "state.h"

class Game : public std::enable_shared_from_this<Game>
{
private:
    bool is_running_;
    bool is_started_;

    std::shared_ptr<State> state_;
    std::shared_ptr<Field> field_;
    std::shared_ptr<PlayerProfile> player_;
    std::shared_ptr<Level> level_;

    sf::Clock block_clock_;
    sf::Clock level_clock_;

    int score_;
    int nr_of_lines_;
    int level_event_countdown_;
    int next_nr_of_lines_bonus_;

    void start_new_game();
    void process_game_evaluation(const Evaluation &evaluation);

    void render_gameover(const std::shared_ptr<sf::RenderWindow> window) const;
    void render_scoreboard(const std::shared_ptr<sf::RenderWindow> window) const;
    void render_level_countdown(const std::shared_ptr<sf::RenderWindow> window) const;

public:
    Game();

    inline bool is_running() const { return is_running_; }
    inline bool is_started() const { return is_started_; }

    void init(const FieldDescription &field_description);

    bool update(const std::optional<sf::Event> &event);

    void set_state(const std::shared_ptr<State> &state);

    void drop_block();
    void down_block();
    void left_block();
    void right_block();
    void up_block();

    void progress_game();
    void check_level_clock();
    void update_score(int nr_of_full_lines);
    void update_level();

    void start();
    void stop();
    void resume();
    void pause();
    void exit();
    void game_over();

    void display(const std::shared_ptr<sf::RenderWindow> window) const;
    void display_playing_state(const std::shared_ptr<sf::RenderWindow> window) const;
    void display_gameover_state(const std::shared_ptr<sf::RenderWindow> window) const;
};