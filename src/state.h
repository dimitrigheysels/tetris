#pragma once

#include <SFML/Graphics.hpp>

class Game;
class Menu;
class MenuItem;

class State
{
protected:
    Game &game_;

    State(Game &game);

public:
    virtual ~State() {};

    virtual std::string_view get_name() const = 0;
    virtual void update(Game &game, std::optional<sf::Event> input_event) = 0;
    virtual void display(const std::shared_ptr<sf::RenderWindow> window) const = 0;
};

class StateMenu : public State
{
private:
    static inline std::shared_ptr<StateMenu> instance_ = nullptr;

    std::shared_ptr<Menu> active_menu_;

    StateMenu(Game &game);

public:
    StateMenu(const StateMenu &) = delete;
    void operator=(const StateMenu &) = delete;
    ~StateMenu() {};

    static void init(Game &game);

    static std::shared_ptr<StateMenu> &get_instance();

    std::string_view get_name() const override;
    void update(Game &game, std::optional<sf::Event> input_event) override;
    void display(const std::shared_ptr<sf::RenderWindow> window) const override;
};

class StatePlaying : public State
{
private:
    static inline std::shared_ptr<StatePlaying> instance_ = nullptr;

protected:
    StatePlaying(Game &game);

public:
    StatePlaying(const StatePlaying &) = delete;
    void operator=(const StatePlaying &) = delete;
    ~StatePlaying() {};

    static void init(Game &game);

    static std::shared_ptr<StatePlaying> &get_instance();

    std::string_view get_name() const override;
    void update(Game &game, std::optional<sf::Event> input_event) override;
    void display(const std::shared_ptr<sf::RenderWindow> window) const override;
};

class StatePaused : public StatePlaying
{
private:
    static inline std::shared_ptr<StatePaused> instance_ = nullptr;

    StatePaused(Game &game);

public:
    StatePaused(const StatePaused &) = delete;
    void operator=(const StatePaused &) = delete;
    ~StatePaused() {};

    static void init(Game &game);

    static std::shared_ptr<StatePaused> &get_instance();

    std::string_view get_name() const override;
    void update(Game &game, std::optional<sf::Event> input_event) override;
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
    ~StateGameOver() {};

    static void init(Game &game);

    static std::shared_ptr<StateGameOver> &get_instance();

    std::string_view get_name() const override;
    void update(Game &game, std::optional<sf::Event> input_event) override;
    void display(const std::shared_ptr<sf::RenderWindow> window) const override;
};
