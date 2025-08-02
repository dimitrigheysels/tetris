#include <spdlog/spdlog.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include <math.h>

#include "game.h"
#include "global.h"

Game::Game(const FieldDescription &field_description) : is_running_(true), is_paused_(false), is_game_over_(false), score_(0), nr_of_lines_(0), next_nr_of_lines_bonus_(BONUS_EVERY_LINES)
{
    font_.openFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf");
    sm_ = std::make_shared<SoundManager>();

    field_ = std::make_shared<Field>(field_description);

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
    spdlog::info("Starting new game ...");

    is_game_over_ = false;
    score_ = 0;
    nr_of_lines_ = 0;

    level_ = std::make_shared<Level_1>();
    level_event_countdown_ = level_->event_countdown_in_seconds();

    field_->reset();
    field_->add_new_block();
    // sm_->play_level_music(level_->get_music());
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

    if (level_event_countdown_ > 0)
    {
        auto t = level_clock_.getElapsedTime().asSeconds();

        if (std::abs(t - level_event_countdown_) <= 0.002)
        {
            sm_->play_bell();
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
        sm_->play_destroy_1_line();
        break;
    case 2:
        score_ += SCORE_2_LINE;
        sm_->play_destroy_2_lines();
        break;
    case 3:
        score_ += SCORE_3_LINE;
        sm_->play_destroy_3_lines();
        break;
    case 4:
        score_ += SCORE_4_LINE;
        sm_->play_destroy_4_lines();
        break;
    }
}

void Game::update_level()
{
    if ((level_->get_number() < 6) && (score_ / SCORE_NEXT_LEVEL == level_->get_number()))
    {
        level_ = level_->next_level();
        level_event_countdown_ = level_->event_countdown_in_seconds();
        level_clock_.restart();
        // sm_->play_level_music(level_->get_music());
    }
}

void Game::game_over()
{
    player_->update_highscore(score_);
    player_->update_highlines(nr_of_lines_);
    is_game_over_ = true;
}

void Game::display(const std::shared_ptr<sf::RenderWindow> &window) const
{
    if (is_game_over_)
    {
        render_gameover(window);
    }
    else
    {
        field_->display(window);
        player_->display(window);
        render_scoreboard(window);
        if (level_event_countdown_ > 0)
        {
            render_level_countdown(window);
        }
    }
}

void Game::render_gameover(const std::shared_ptr<sf::RenderWindow> window) const
{
    sf::Text gameover_text(font_, "G A M E     O V E R\nnew game (y/n)", 30);

    int gameover_textwidth = gameover_text.getLocalBounds().size.x;
    int gameover_textheight = gameover_text.getLocalBounds().size.y;

    gameover_text.setPosition({(window->getSize().x - gameover_textwidth) / 2.0f,
                               (window->getSize().y - gameover_textheight) / 2.0f});
    gameover_text.setOutlineThickness(3);

    window->draw(gameover_text);
}

void Game::render_scoreboard(const std::shared_ptr<sf::RenderWindow> window) const
{
    sf::RectangleShape scoreboard(sf::Vector2f(200.0, 100.0));
    scoreboard.setPosition({(MAX_COLS * SIZE_TILE) + 50, 260 + (2 * SIZE_TILE)});
    scoreboard.setFillColor(sf::Color(64, 64, 64));

    std::stringstream ss;
    ss << "level: " << level_->get_number() << std::endl;
    ss << "# lines: " << nr_of_lines_ << std::endl;
    ss << "score: " << score_ << std::endl;

    sf::Text score_text(font_, ss.str(), 12);

    score_text.setPosition({scoreboard.getPosition().x + 10, scoreboard.getPosition().y + 10});

    score_text.setOutlineThickness(1);

    window->draw(scoreboard);
    window->draw(score_text);
}

void Game::render_level_countdown(const std::shared_ptr<sf::RenderWindow> window) const
{
    sf::RectangleShape countdown_timer(sf::Vector2f(200.0, 100.0));
    countdown_timer.setPosition({(MAX_COLS * SIZE_TILE) + 50, 400 + (2 * SIZE_TILE)});
    countdown_timer.setFillColor(sf::Color(64, 64, 64));

    std::stringstream ss;
    ss << std::max(0, level_event_countdown_ - (int)level_clock_.getElapsedTime().asSeconds());

    sf::Text countdown_text(font_, ss.str(), 32);
    int countdown_textwidth = countdown_text.getLocalBounds().size.x;
    int countdown_textheight = countdown_text.getLocalBounds().size.y;

    countdown_text.setPosition({countdown_timer.getPosition().x + ((countdown_timer.getSize().x - countdown_textwidth) / 2.0f),
                                countdown_timer.getPosition().y + ((countdown_timer.getSize().y - countdown_textheight) / 2.0f)});

    countdown_text.setOutlineThickness(2);

    window->draw(countdown_timer);
    window->draw(countdown_text);
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