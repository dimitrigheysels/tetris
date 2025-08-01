#include <fstream>

#include "player_profile.h"
#include "global.h"

PlayerProfile::PlayerProfile() : highscore_(0)
{
    font_.openFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf");
}

void PlayerProfile::update_highscore(int score)
{
    if (score > highscore_)
    {
        highscore_ = score;
    }
}

void PlayerProfile::update_highlines(int lines)
{
    if (lines > highlines_)
    {
        highlines_ = lines;
    }
}

void PlayerProfile::display(const std::shared_ptr<sf::RenderWindow> &window) const
{
    render_highscore(window);
}

void PlayerProfile::render_highscore(const std::shared_ptr<sf::RenderWindow> &window) const
{
    sf::RectangleShape scoreboard(sf::Vector2f(200.0, 100.0));
    scoreboard.setPosition({(MAX_COLS * SIZE_TILE) + 50, 120 + (2 * SIZE_TILE)});
    scoreboard.setFillColor(sf::Color(64, 64, 64));

    std::stringstream ss;
    ss << "player highscore: " << highscore_ << std::endl;
    ss << "player highlines " << highlines_ << std::endl;

    sf::Text score_text(font_, ss.str(), 12);
    score_text.setPosition({scoreboard.getPosition().x + 10, scoreboard.getPosition().y + 10});

    score_text.setOutlineThickness(1);

    window->draw(scoreboard);
    window->draw(score_text);
}

void PlayerProfile::save() const
{
    std::ofstream out(player_profile_path, std::fstream::out | std::fstream::binary | std::fstream::trunc);
    out.write(reinterpret_cast<const char *>(&highscore_), sizeof(highscore_));
    out.write(reinterpret_cast<const char *>(&highlines_), sizeof(highlines_));
    out.flush();
    out.close();
}

void PlayerProfile::load()
{
    std::ifstream in(player_profile_path, std::ios::in | std::ios::binary);
    in.read(reinterpret_cast<char *>(&highscore_), sizeof(highscore_));
    in.read(reinterpret_cast<char *>(&highlines_), sizeof(highlines_));
    in.close();
}
