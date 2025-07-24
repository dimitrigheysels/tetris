#include <sstream>

#include "ui.h"

UI::UI()
{
    window_ = std::make_shared<sf::RenderWindow>(sf::VideoMode({1280, 1024}), "MyTetris");
    font_.openFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf");
}

void UI::clear() const
{
    window_->clear();
}

void UI::display() const
{
    window_->display();
}

void UI::close()
{
    window_->close();
}

std::optional<sf::Event> UI::poll_event() const
{
    return window_->pollEvent();
}

void UI::render_highscore(int score) const
{
    sf::RectangleShape scoreboard(sf::Vector2f(200.0, 100.0));
    scoreboard.setPosition({(COLS * SIZE_TILE) + 50, 120 + (2 * SIZE_TILE)});
    scoreboard.setFillColor(sf::Color(64, 64, 64));

    std::stringstream ss;
    ss << "player highscore: " << score << std::endl;

    sf::Text score_text(font_, ss.str(), 12);
    score_text.setPosition({scoreboard.getPosition().x + 10, scoreboard.getPosition().y + 10});

    score_text.setOutlineThickness(1);

    window_->draw(scoreboard);
    window_->draw(score_text);
}

void UI::render_scoreboard(int level, int score, int nr_of_lines) const
{
    sf::RectangleShape scoreboard(sf::Vector2f(200.0, 100.0));
    scoreboard.setPosition({(COLS * SIZE_TILE) + 50, 260 + (2 * SIZE_TILE)});
    scoreboard.setFillColor(sf::Color(64, 64, 64));

    std::stringstream ss;
    ss << "level: " << level << std::endl;
    ss << "# lines: " << nr_of_lines << std::endl;
    ss << "score: " << score << std::endl;

    sf::Text score_text(font_, ss.str(), 12);

    score_text.setPosition({scoreboard.getPosition().x + 10, scoreboard.getPosition().y + 10});

    score_text.setOutlineThickness(1);

    window_->draw(scoreboard);
    window_->draw(score_text);
}

void UI::render_level_countdown(int level_timer_elapsed_seconds, int level_event_countdown) const
{
    sf::RectangleShape countdown_timer(sf::Vector2f(200.0, 100.0));
    countdown_timer.setPosition({(COLS * SIZE_TILE) + 50, 400 + (2 * SIZE_TILE)});
    countdown_timer.setFillColor(sf::Color(64, 64, 64));

    std::stringstream ss;
    ss << std::max(0, level_event_countdown - level_timer_elapsed_seconds);

    sf::Text countdown_text(font_, ss.str(), 32);
    int countdown_textwidth = countdown_text.getLocalBounds().size.x;
    int countdown_textheight = countdown_text.getLocalBounds().size.y;

    // countdown_text.setPosition({countdown_timer.getPosition().x + 10, countdown_timer.getPosition().y + 10});
    countdown_text.setPosition({countdown_timer.getPosition().x + ((countdown_timer.getSize().x - countdown_textwidth) / 2.0f),
                                countdown_timer.getPosition().y + ((countdown_timer.getSize().y - countdown_textheight) / 2.0f)});

    countdown_text.setOutlineThickness(2);

    window_->draw(countdown_timer);
    window_->draw(countdown_text);
}

void UI::render_tiles(const std::shared_ptr<Tile> (&tiles_)[ROWS][COLS], int top_row) const
{
    for (int row = 1; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            render_tile(row, col, tiles_[row][col], top_row);
        }
    }

    // auto r = sf::RectangleShape();
    // r.setSize(sf::Vector2f(810.0, 1000.0));

    // r.move({450, 20});

    // std::stringstream ss;
    // for (int i = 0; i < ROWS; i++)
    // {
    //     for (int j = 0; j < COLS; j += 4)
    //     {
    //         ss << std::right << std::setw(2) << i << "@" << std::left << std::setw(2) << j
    //            << " fixed: " << tiles_[i][j]->is_fixed()
    //            << " block: " << std::right << std::setw(14) << tiles_[i][j]->get_block()
    //            << "\t"
    //            << std::right << std::setw(2) << i << "@" << std::left << std::setw(2) << j + 1
    //            << " fixed: " << tiles_[i][j + 1]->is_fixed()
    //            << " block: " << std::right << std::setw(14) << tiles_[i][j + 1]->get_block()
    //            << "\t"
    //            << std::right << std::setw(2) << i << "@" << std::left << std::setw(2) << j + 2
    //            << " fixed: " << tiles_[i][j + 2]->is_fixed()
    //            << " block: " << std::right << std::setw(14) << tiles_[i][j + 2]->get_block()
    //            << "\t"
    //            << std::right << std::setw(2) << i << "@" << std::left << std::setw(2) << j + 3
    //            << " fixed: " << tiles_[i][j + 3]->is_fixed()
    //            << " block: " << std::right << std::setw(14) << tiles_[i][j + 3]->get_block() << std::endl;
    //     }
    // }

    // sf::Text t(font_, ss.str(), 10);
    // t.setPosition({r.getPosition().x + 1, r.getPosition().y + 5});
    // t.setOutlineThickness(1);

    // window_->draw(r);
    //  window_->draw(t);
}

void UI::render_tile(int r, int c, const std::shared_ptr<Tile> tile, int top_row) const
{
    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(SIZE_TILE, SIZE_TILE));
    shape.setPosition({c * SIZE_TILE, r * SIZE_TILE});
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(-1.0f);

    if (tile->is_boundary())
    {
        if (r == top_row)
        {
            shape.setFillColor(sf::Color(32, 32, 32));
        }
        else
        {
            shape.setFillColor(sf::Color(128, 128, 128));
        }
    }
    else if (auto block = tile->get_block())
    {
        shape.setFillColor(block->get_color());
    }
    else
    {
        shape.setFillColor(sf::Color(64, 64, 64));
    }

    window_->draw(shape);
}

void UI::render_next(const Block &block) const
{
    auto layout = block.get_current_layout();

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (layout[row][col])
            {
                auto shape = sf::RectangleShape(sf::Vector2f(SIZE_TILE, SIZE_TILE));
                shape.setPosition({X_NEXT_BLOCK + (col * SIZE_TILE), Y_NEXT_BLOCK + (row * SIZE_TILE)});
                shape.setOutlineColor(sf::Color::Black);
                shape.setOutlineThickness(-1.0f);
                shape.setFillColor(block.get_color());

                window_->draw(shape);
            }
        }
    }
}

void UI::render_gameover() const
{
    sf::Text gameover_text(font_, "G A M E     O V E R\nnew game (y/n)", 30);

    int gameover_textwidth = gameover_text.getLocalBounds().size.x;
    int gameover_textheight = gameover_text.getLocalBounds().size.y;

    gameover_text.setPosition({(window_->getSize().x - gameover_textwidth) / 2.0f,
                               (window_->getSize().y - gameover_textheight) / 2.0f});
    gameover_text.setOutlineThickness(3);

    window_->draw(gameover_text);
}