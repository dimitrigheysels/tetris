#include <iostream>

#include "game.h"
#include "global.h"

Game::Game() : is_running_(true), is_paused_(false), is_game_over_(false), score_(0), nr_of_lines_(0)
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
                            // GameState state = player_->down_block();
                            GameState state = field_->down_block();
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

    if (level_clock_.getElapsedTime().asSeconds() >= std::experimental::randint(30, 45))
    {
        level_->do_something_with_field(field_);
        level_clock_.restart();
    }
}

void Game::update_score(int nr_of_full_lines)
{
    nr_of_lines_ += nr_of_full_lines;
    switch (nr_of_full_lines)
    {
    case 1:
        score_ += 1;
        break;
    case 2:
        score_ += 3;
        break;
    case 3:
        score_ += 6;
        break;
    case 4:
        score_ += 10;
        break;
    }
}

void Game::update_level()
{
    if ((level_->get_number() < 4) && (score_ / 100.0f >= level_->get_number()))
    {
        level_ = level_->next_level();
        level_clock_.restart();
    }
}

void Game::game_over()
{
    player_->update_highscore(score_);
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
    }
}

void Game::process_game_state(const GameState &state)
{
    if (state.game_over)
    {
        game_over();
        return;
    }

    if (state.nr_of_full_lines > 0)
    {
        update_score(state.nr_of_full_lines);
        // if score >= every 500 points --> clear 5 lines (but do not clear whole field)
        if (score_ > 9 && score_ % 500 < 9)
        {
            field_->clear_lines(5);
        }
        update_level();
        return;
    }
}