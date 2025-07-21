#include <iostream>

#include "game.h"
#include "global.h"

Game::Game() : is_running_(true), is_paused_(false), is_game_over_(false), score_(0), nr_of_lines_(0)
{
    level_ = std::make_shared<Level_1>();
    player_ = std::make_shared<PlayerProfile>();
    field_ = std::make_shared<Field>();
    field_->add_new_block();
}

bool Game::is_running()
{
    return is_running_;
}

void Game::start_new_game()
{
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
                        is_game_over_ = false;
                        score_ = 0;
                        nr_of_lines_ = 0;

                        start_new_game();
                        break;
                    }
                    case sf::Keyboard::Scancode::N:
                    {
                        is_running_ = false;
                        break;
                        // save player data in profile
                        // ...
                    }
                    }
                }
            }
        }
    }
    else
    {
        if (block_clock_.getElapsedTime().asSeconds() >= 0.5f)
        {
            if (!is_game_over_ && !is_paused_)
            {
                GameState state = field_->down_block();
                process_game_state(state);
            }
            block_clock_.restart();
        }
    }

    if (level_clock_.getElapsedTime().asSeconds() >= 30.0f)
    {
        // level->do_something_with_field(field_)
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
    // if (level_ < 9)
    // {
    if (score_ % 100 == 0)
    {
        // level_++;
        level_ = level_->next_level();
        std::cout << "level up ! " << level_ << std::endl;
        level_clock_.restart();
    }
    // }
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
        update_level();
        return;
    }
}