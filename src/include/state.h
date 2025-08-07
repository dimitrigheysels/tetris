#pragma once

#include <SFML/Graphics.hpp>

class Game;
class Menu;
class MenuItem;

class State
{
protected:
    State(Game &game);

    Game &game_;

public:
    virtual ~State() {};

    virtual inline std::string_view get_name() const = 0;

    virtual bool update(const std::optional<sf::Event> &input_event) = 0;
    virtual void display(const std::shared_ptr<sf::RenderWindow> window) const = 0;
};

class StateMenu : public State
{
private:
    static inline std::shared_ptr<StateMenu> instance_ = nullptr;

    StateMenu(Game &game);

    std::shared_ptr<Menu> active_menu_;

public:
    StateMenu(const StateMenu &) = delete;
    void operator=(const StateMenu &) = delete;

    static void init(Game &game);
    static std::shared_ptr<StateMenu> &get_instance();

    inline std::string_view get_name() const override { return "MENU"; };

    bool update(const std::optional<sf::Event> &input_event) override;
    void display(const std::shared_ptr<sf::RenderWindow> window) const override;
};

class StatePlaying : public State
{
private:
    static inline std::shared_ptr<StatePlaying> instance_ = nullptr;

    StatePlaying(Game &game);

public:
    StatePlaying(const StatePlaying &) = delete;
    void operator=(const StatePlaying &) = delete;

    static void init(Game &game);
    static std::shared_ptr<StatePlaying> &get_instance();

    inline std::string_view get_name() const override { return "PLAYING"; };

    bool update(const std::optional<sf::Event> &input_event) override;
    void display(const std::shared_ptr<sf::RenderWindow> window) const override;
};

class StateGameOver : public State
{
private:
    static inline std::shared_ptr<StateGameOver> instance_ = nullptr;

    StateGameOver(Game &game);

public:
    StateGameOver(const StateGameOver &) = delete;
    void operator=(const StateGameOver &) = delete;

    static void init(Game &game);
    static std::shared_ptr<StateGameOver> &get_instance();

    inline std::string_view get_name() const override { return "GAME OVER"; };

    bool update(const std::optional<sf::Event> &input_event) override;
    void display(const std::shared_ptr<sf::RenderWindow> window) const override;
};
