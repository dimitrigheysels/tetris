#include <iostream>

#include "game.h"
#include "global.h"
#include "ui.h"

Game::Game() : is_running_(true), is_paused_(false), is_game_over_(false), score_(0), nr_of_lines_(0), level_(1)
{
    player_ = std::make_shared<Player>();
}

bool Game::is_running()
{
    return is_running_;
}

void Game::update()
{
    if (!is_game_over_ && !is_paused_)
    {
        GameState state = player_->down_block();
        process_game_state(state);
    }
}

void Game::update(const sf::Event &event)
{
    if (event.type == sf::Event::Closed ||
        (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)))
    {
        is_running_ = false;
    }
    else
    {
        if (!is_game_over_)
        {
            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
            {
                is_paused_ ^= true;
            }
            else if (!is_paused_)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ||
                        event.key.code == sf::Keyboard::Key::Down)
                    {
                        GameState state = player_->down_block();
                        process_game_state(state);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                    {
                        player_->left_block();
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                    {
                        player_->right_block();
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                    {
                        player_->up_block();
                    }
                }
            }
        }
        else // is game over
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y))
                {
                    is_game_over_ = false;
                    score_ = 0;
                    nr_of_lines_ = 0;
                    level_ = 1;
                    player_->start_new_game();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N))
                {
                    is_running_ = false;

                    // save player data
                    // ...
                }
            }
        }
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

void Game::game_over()
{
    player_->update_highscore(score_);
    is_game_over_ = true;

    // play again window ?
    // is_running_ = false;
}

void Game::display(const UI &ui) const
{
    if (is_game_over_)
    {
        ui.render_gameover();
        // display big "GAME OVER" text
        // as to play again ?
    }
    else // if not gameover
    {
        player_->display(ui);
        ui.render_scoreboard(level_, score_, nr_of_lines_);
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
        // update_level();
        return;
    }
}