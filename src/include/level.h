#pragma once

#include <memory>
#include <experimental/random>

// levels:
//  1. normal
//  2. randomly toggle fixed tiles on existing top 2 rows if height of field < 15 rows
//  3. randomly add fixed tiles on max 2 lines above current height if height of filed < 15 rows
//  4. combine 3. and 4.
//  5. speedup
//  6. randomly add fixed tiles (max 5? lines above current height AND height lower than half? of field)
//  7. randomly remove fixed tiles (max 5? lines above current height AND height lower than half? of field)
//  8. combine 6. and 7.
//  9. scramble whole field

class Field;

class Level
{
public:
    Level() = default;
    virtual ~Level() = default;

    virtual inline int get_number() const = 0;
    virtual inline float speed() const = 0;
    virtual inline int event_countdown_in_seconds() const = 0;
    virtual inline std::shared_ptr<Level> next_level() const = 0;

    virtual void do_something_with_field(const std::shared_ptr<Field> field) const = 0;
};

class Level_6 : public Level
{
public:
    inline int get_number() const override { return 6; }
    inline float speed() const override { return 0.5f; };
    inline int event_countdown_in_seconds() const override { return std::experimental::randint(10, 20); }
    inline std::shared_ptr<Level> next_level() const override { return std::make_shared<Level_6>(); }

    void do_something_with_field(const std::shared_ptr<Field> field) const override;
};

class Level_5 : public Level
{
public:
    inline int get_number() const override { return 5; }
    inline float speed() const override { return 0.5f; };
    inline int event_countdown_in_seconds() const override { return std::experimental::randint(15, 25); }
    inline std::shared_ptr<Level> next_level() const override { return std::make_shared<Level_6>(); }

    void do_something_with_field(const std::shared_ptr<Field> field) const override;
};

class Level_4 : public Level
{
public:
    inline int get_number() const override { return 4; }
    inline float speed() const override { return 0.5f; };
    inline int event_countdown_in_seconds() const override { return std::experimental::randint(20, 30); }
    inline std::shared_ptr<Level> next_level() const override { return std::make_shared<Level_5>(); }

    void do_something_with_field(const std::shared_ptr<Field> field) const override;
};

class Level_3 : public Level
{
public:
    inline int get_number() const override { return 3; }
    inline float speed() const override { return 0.15f; }
    inline int event_countdown_in_seconds() const override { return 0; }
    inline std::shared_ptr<Level> next_level() const override { return std::make_shared<Level_4>(); }

    void do_something_with_field(const std::shared_ptr<Field> field) const override;
};

class Level_2 : public Level
{
public:
    inline int get_number() const override { return 2; }
    inline float speed() const override { return 0.3f; };
    inline int event_countdown_in_seconds() const override { return 0; }
    inline std::shared_ptr<Level> next_level() const override { return std::make_shared<Level_3>(); }

    void do_something_with_field(const std::shared_ptr<Field> field) const override;
};

class Level_1 : public Level
{
public:
    inline int get_number() const override { return 1; }
    inline float speed() const override { return 0.5f; };
    inline int event_countdown_in_seconds() const override { return 0; }
    inline std::shared_ptr<Level> next_level() const override { return std::make_shared<Level_2>(); }

    void do_something_with_field(const std::shared_ptr<Field> field) const override;
};
