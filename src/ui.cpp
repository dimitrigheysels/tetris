#include <sstream>

#include "ui.h"

UI::UI()
{
    window_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "MyTetris");
    font.loadFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf");
}

void UI::clear()
{
    window_->clear();
}

void UI::display()
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

void UI::display_scoreboard(int score) const
{
    sf::RectangleShape scoreboard(sf::Vector2f(100.0, 100.0));
    scoreboard.setPosition(600, 100);
    scoreboard.setFillColor(sf::Color::White);

    std::stringstream ss;
    ss << "score: " << score << std::endl;

    sf::Text score_text(ss.str(), font, 12);
    score_text.setPosition(scoreboard.getPosition().x + 10, scoreboard.getPosition().y + 10);

    score_text.setOutlineThickness(1);

    window_->draw(scoreboard);
    window_->draw(score_text);
}

void UI::display_field(const Field &field) const
{
    field.display(*window_);
}

void UI::display_next(const Block &block) const
{
    block.display(*window_);
}