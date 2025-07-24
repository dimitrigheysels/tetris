#include "game.h"
#include "global.h"

Game::Game() : is_running_(true), is_paused_(false), is_game_over_(false), score_(0), nr_of_lines_(0), next_nr_of_lines_bonus_(BONUS_EVERY_LINES)
{
    player_ = std::make_shared<PlayerProfile>();
    player_->load();

    start_new_game();
}

bool Game::is_running() const
{
    return is_running_;
}

void Game::start_new_game()
{
    is_game_over_ = false;
    score_ = 0;
    nr_of_lines_ = 0;

    level_ = std::make_shared<Level_1>();
    level_event_countdown_ = level_->event_countdown_in_seconds();
    field_ = std::make_shared<Field>();
    field_->add_new_block();
}

void Game::update(std::optional<sf::Event> event)
{
    if (event)
    {
        if (event->is<sf::Event::Closed>())
        {
            is_running_ = false;
        }
        else
        {
            const auto e = event->getIf<sf::Event::KeyPressed>();
            if (e != nullptr)
            {
                if (e->scancode == sf::Keyboard::Scancode::Escape)
                {
                    is_running_ = false;
                }
                else if (!is_game_over_)
                {
                    if (e->scancode == sf::Keyboard::Scancode::P)
                    {
                        is_paused_ ^= true;
                        block_clock_.isRunning() ? block_clock_.stop() : block_clock_.start();
                        level_clock_.isRunning() ? level_clock_.stop() : level_clock_.start();
                    }
                    else if (!is_paused_)
                    {
                        switch (e->scancode)
                        {
                        case sf::Keyboard::Scancode::Down:
                        {
                            GameState state;
                            if (e->control)
                            {
                                state = field_->drop_block();
                            }
                            else
                            {
                                state = field_->down_block();
                            }

                            process_game_state(state);
                            break;
                        }
                        case sf::Keyboard::Scancode::Left:
                        {
                            field_->left_block();
                            break;
                        }
                        case sf::Keyboard::Scancode::Right:
                        {
                            field_->right_block();
                            break;
                        }
                        case sf::Keyboard::Scancode::Up:
                        {
                            field_->up_block();
                            break;
                        }
                        }
                    }
                }
                else // is game over
                {
                    switch (e->scancode)
                    {
                    case sf::Keyboard::Scancode::Y:
                    {
                        start_new_game();
                        break;
                    }
                    case sf::Keyboard::Scancode::N:
                    {
                        is_running_ = false;
                        player_->save();
                        break;
                    }
                    }
                }
            }
        }
    }
    else
    {
        if (block_clock_.getElapsedTime().asSeconds() >= level_->speed())
        {
            if (!is_game_over_ && !is_paused_)
            {
                GameState state = field_->down_block();
                process_game_state(state);
            }
            block_clock_.restart();
        }
    }
}

void Game::update_score(int nr_of_full_lines)
{
    nr_of_lines_ += nr_of_full_lines;
    switch (nr_of_full_lines)
    {
    case 1:
        score_ += SCORE_1_LINE;
        break;
    case 2:
        score_ += SCORE_2_LINE;
        break;
    case 3:
        score_ += SCORE_3_LINE;
        break;
    case 4:
        score_ += SCORE_4_LINE;
        break;
    }
}

void Game::update_level()
{
    if ((level_->get_number() < 4) && (score_ / SCORE_NEXT_LEVEL == level_->get_number()))
    {
        level_ = level_->next_level();
        level_event_countdown_ = level_->event_countdown_in_seconds();
        level_clock_.restart();
    }
}

void Game::game_over()
{
    player_->update_highscore(score_);
    player_->update_highlines(nr_of_lines_);
    is_game_over_ = true;
}

void Game::display(const UI &ui) const
{
    if (is_game_over_)
    {
        ui.render_gameover();
    }
    else
    {
        field_->display(ui);
        player_->display(ui);
        ui.render_scoreboard(level_->get_number(), score_, nr_of_lines_);
        if (level_event_countdown_ > 0)
        {
            ui.render_level_countdown(level_clock_.getElapsedTime().asSeconds(), level_event_countdown_);
        }
    }
}

void Game::process_game_state(const GameState &state)
{
    if (state.game_over)
    {
        game_over();
        return;
    }

    if (state.new_block)
    {
        // field should only be changed (according to level) when new block is created
        if (level_event_countdown_ > 0 && level_clock_.getElapsedTime().asSeconds() >= level_event_countdown_)
        {
            // calculate next interval and show the clock
            level_event_countdown_ = level_->event_countdown_in_seconds();
            level_->do_something_with_field(field_);
            level_clock_.restart();
        }
    }

    if (state.nr_of_full_lines > 0)
    {
        update_score(state.nr_of_full_lines);

        // clear lines every X lines (but do not clear whole field)
        if (nr_of_lines_ > next_nr_of_lines_bonus_)
        {
            field_->clear_lines(BONUS_LINES);
            next_nr_of_lines_bonus_ += BONUS_EVERY_LINES;
        }

        update_level();
        return;
    }
}