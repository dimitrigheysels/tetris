#include <sstream>
#include <iomanip>

#include "ui.h"

UI::UI()
{
    window_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(1280, 1024), "MyTetris");
    font.loadFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf");
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

bool UI::poll_event(sf::Event &event)
{
    return window_->pollEvent(event);
}

void UI::render_scoreboard(int score) const
{
    sf::RectangleShape scoreboard(sf::Vector2f(100.0, 100.0));
    scoreboard.setPosition((COLS * SIZE_TILE) + 50, 200);
    scoreboard.setFillColor(sf::Color::White);

    std::stringstream ss;
    ss << "score: " << score << std::endl;

    sf::Text score_text(ss.str(), font, 12);
    score_text.setPosition(scoreboard.getPosition().x + 10, scoreboard.getPosition().y + 10);

    score_text.setOutlineThickness(1);

    window_->draw(scoreboard);
    window_->draw(score_text);
}

void UI::render_tiles(const std::shared_ptr<Tile> (&tiles_)[ROWS][COLS]) const
{
    for (int row = 1; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            // tiles_[row][col]->display(this);
            render_tile(row, col, tiles_[row][col]);
        }
    }

    auto r = sf::RectangleShape();
    r.setSize(sf::Vector2f(810.0, 1000.0));

    r.setOutlineColor(sf::Color::Blue);
    r.setOutlineThickness(1);
    r.move(450, 20);

    std::stringstream ss;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j += 4)
        {
            ss << std::right << std::setw(2) << i << "@" << std::left << std::setw(2) << j
               << " fixed: " << tiles_[i][j]->is_fixed()
               << " block: " << std::right << std::setw(14) << tiles_[i][j]->get_block()
               << "\t"
               << std::right << std::setw(2) << i << "@" << std::left << std::setw(2) << j + 1
               << " fixed: " << tiles_[i][j + 1]->is_fixed()
               << " block: " << std::right << std::setw(14) << tiles_[i][j + 1]->get_block()
               << "\t"
               << std::right << std::setw(2) << i << "@" << std::left << std::setw(2) << j + 2
               << " fixed: " << tiles_[i][j + 2]->is_fixed()
               << " block: " << std::right << std::setw(14) << tiles_[i][j + 2]->get_block()
               << "\t"
               << std::right << std::setw(2) << i << "@" << std::left << std::setw(2) << j + 3
               << " fixed: " << tiles_[i][j + 3]->is_fixed()
               << " block: " << std::right << std::setw(14) << tiles_[i][j + 3]->get_block() << std::endl;
        }
    }

    sf::Text t(ss.str(), font, 10);
    t.setPosition(r.getPosition().x + 1, r.getPosition().y + 5);

    t.setOutlineThickness(1);

    window_->draw(r);
    window_->draw(t);
}

void UI::render_tile(int r, int c, const std::shared_ptr<Tile> tile) const
{
    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(SIZE_TILE, SIZE_TILE));
    shape.setPosition(c * SIZE_TILE, r * SIZE_TILE);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(-1.0f);

    if (tile->is_boundary())
    {
        shape.setFillColor(sf::Color(128, 128, 128));
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
                shape.setPosition(X_NEXT_BLOCK + (col * SIZE_TILE), Y_NEXT_BLOCK + (row * SIZE_TILE));
                shape.setOutlineColor(sf::Color::Black);
                shape.setOutlineThickness(-1.0f);
                shape.setFillColor(block.get_color());

                window_->draw(shape);
            }
        }
    }
}