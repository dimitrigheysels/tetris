#include <spdlog/spdlog.h>
#include <SFML/Audio.hpp>
#include <math.h>
#include <memory>

#include "include/game.h"
#include "include/global.h"
#include "include/state.h"
#include "include/resource_manager.hpp"

Game::Game() : is_running_(true), is_started_(false), score_(0), nr_of_lines_(0), next_nr_of_lines_bonus_(BONUS_EVERY_LINES)
{
    spdlog::debug("Game::Game()");
    field_ = std::make_shared<Field>();
    player_ = std::make_shared<PlayerProfile>();
}

void Game::init(const FieldDescription &field_description)
{
    spdlog::info("Initializing game ...");
    try
    {
        field_->init(field_description);
    }
    catch (const std::exception &e)
    {
        // initialise with 'default' field-description
        spdlog::error(e.what());
        field_->init(field_description);
    }

    player_->load();

    StateMenu::init(*this);
    StatePlaying::init(*this);
    StateGameOver::init(*this);

    state_ = StateMenu::get_instance();
}

void Game::set_state(const std::shared_ptr<State> &state)
{
    spdlog::debug("State change: {} -> {}", state_->get_name(), state->get_name());
    state_ = state;
}

bool Game::is_running() const
{
    return is_running_;
}

void Game::start_new_game()
{
    spdlog::info("Starting new game ...");

    score_ = 0;
    nr_of_lines_ = 0;

    level_ = std::make_shared<Level_4>();
    level_event_countdown_ = level_->event_countdown_in_seconds();

    field_->reset();
    field_->add_new_block();

    is_started_ = true;
}

bool Game::is_started() const
{
    return is_started_;
}

bool Game::update(const std::optional<sf::Event> &event)
{
    return state_->update(event);
}

void Game::drop_block()
{
    Evaluation evaluation = field_->drop_block();

    process_game_evaluation(evaluation);
}

void Game::down_block()
{
    Evaluation evaluation = field_->down_block();

    process_game_evaluation(evaluation);
}

void Game::left_block()
{
    field_->left_block();
}

void Game::right_block()
{
    field_->right_block();
}

void Game::up_block()
{
    field_->up_block();
}

void Game::progress_game()
{
    if (block_clock_.getElapsedTime().asSeconds() >= level_->speed())
    {
        Evaluation evaluation = field_->down_block();

        process_game_evaluation(evaluation);

        block_clock_.restart();
    }
}

void Game::check_level_clock()
{
    if (level_event_countdown_ > 0)
    {
        auto t = level_clock_.getElapsedTime().asSeconds();

        if (std::abs(level_event_countdown_ - t) <= 0.01)
        {
            ResourceManager::get_instance()->get_sound("bell")->play();
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
        ResourceManager::get_instance()->get_sound("destroy_1_line")->play();
        break;
    case 2:
        score_ += SCORE_2_LINE;
        ResourceManager::get_instance()->get_sound("destroy_2_line")->play();
        break;
    case 3:
        score_ += SCORE_3_LINE;
        ResourceManager::get_instance()->get_sound("destroy_3_line")->play();
        break;
    case 4:
        score_ += SCORE_4_LINE;
        ResourceManager::get_instance()->get_sound("destroy_4_line")->play();
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
    set_state(StateGameOver::get_instance());
}

void Game::start()
{
    start_new_game();
    resume();
}

void Game::stop()
{
    is_started_ = false;
    player_->save();
    set_state(StateMenu::get_instance());
}

void Game::resume()
{
    if (!block_clock_.isRunning())
    {
        block_clock_.start();
    }
    if (!level_clock_.isRunning())
    {
        level_clock_.start();
    }

    if (is_started())
    {
        set_state(StatePlaying::get_instance());
    }
}

void Game::pause()
{
    if (block_clock_.isRunning())
    {
        block_clock_.stop();
    }
    if (level_clock_.isRunning())
    {
        level_clock_.stop();
    }

    set_state(StateMenu::get_instance());
}

void Game::exit()
{
    spdlog::info("exit");
    is_running_ = false;
}

void Game::display(const std::shared_ptr<sf::RenderWindow> window) const
{
    state_->display(window);
}

void Game::display_playing_state(const std::shared_ptr<sf::RenderWindow> window) const
{
    field_->display(window);
    player_->display(window);
    render_scoreboard(window);
    if (level_event_countdown_ > 0)
    {
        render_level_countdown(window);
    }
}

void Game::display_gameover_state(const std::shared_ptr<sf::RenderWindow> window) const
{
    sf::Text gameover_text(*ResourceManager::get_instance()->get_font("default_font"), "G A M E     O V E R\nnew game (y/n)", 30);

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

    sf::Text score_text(*ResourceManager::get_instance()->get_font("default_font"), ss.str(), 12);

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

    sf::Text countdown_text(*ResourceManager::get_instance()->get_font("default_font"), ss.str(), 32);
    int countdown_textwidth = countdown_text.getLocalBounds().size.x;
    int countdown_textheight = countdown_text.getLocalBounds().size.y;

    countdown_text.setPosition({countdown_timer.getPosition().x + ((countdown_timer.getSize().x - countdown_textwidth) / 2.0f),
                                countdown_timer.getPosition().y + ((countdown_timer.getSize().y - countdown_textheight) / 2.0f)});

    countdown_text.setOutlineThickness(2);

    window->draw(countdown_timer);
    window->draw(countdown_text);
}

void Game::process_game_evaluation(const Evaluation &evaluation)
{
    if (evaluation.game_over)
    {
        game_over();
        return;
    }

    if (evaluation.new_block)
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

    if (evaluation.nr_of_full_lines > 0)
    {
        update_score(evaluation.nr_of_full_lines);

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