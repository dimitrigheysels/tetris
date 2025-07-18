#include <iostream>

#include "game.h"
#include "global.h"
#include "ui.h"

Game::Game() : is_running_(true), is_paused_(false)
{
    player_ = std::make_shared<Player>();
}

bool Game::is_running()
{
    return is_running_;
}

void Game::update()
{
    if (!is_paused_)
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
    else if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
    {
        is_paused_ ^= true;
    }
    else if (!is_paused_)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
            {
                is_paused_ ^= true;
            }
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

void Game::game_over()
{
    std::cout << "Game::game_over" << std::endl;
    is_running_ = false;
}

void Game::display(const UI &ui) const
{
    ui.clear();
    player_->display(ui);
    ui.display();
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
        player_->update_score(state.nr_of_full_lines);
        return;
    }
}