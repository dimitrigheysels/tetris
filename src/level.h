#pragma once

#include <memory>

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
    Level() {}
    virtual ~Level() {};

    virtual int get_number() const = 0;
    virtual std::shared_ptr<Level> next_level() const = 0;
    virtual void do_something_with_field(const std::shared_ptr<Field> field) const = 0;
    virtual float speed() const = 0;
};

class Level_1 : public Level
{
public:
    Level_1() : Level() {}
    ~Level_1() {}

    int get_number() const override;
    std::shared_ptr<Level> next_level() const override;
    void do_something_with_field(const std::shared_ptr<Field> field) const override;
    float speed() const override;
};

class Level_2 : public Level
{
public:
    Level_2() : Level() {}
    ~Level_2() {}

    int get_number() const override;
    std::shared_ptr<Level> next_level() const override;
    void do_something_with_field(const std::shared_ptr<Field> field) const override;
    float speed() const override;
};

class Level_3 : public Level
{
public:
    Level_3() : Level() {}
    ~Level_3() {}

    int get_number() const override;
    std::shared_ptr<Level> next_level() const override;
    void do_something_with_field(const std::shared_ptr<Field> field) const override;
    float speed() const override;
};

class Level_4 : public Level
{
public:
    Level_4() : Level() {}
    ~Level_4() {}

    int get_number() const override;
    std::shared_ptr<Level> next_level() const override;
    void do_something_with_field(const std::shared_ptr<Field> field) const override;
    float speed() const override;
};