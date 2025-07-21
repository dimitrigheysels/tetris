#include <memory>

// levels:
//  1. normal
//  2. randomly add fixed tiles (max 2? lines above current height AND height lower than half? of field)
//  3. randomly remove fixed tiles (max 2? lines above current height AND height lower than half? of field)
//  4. combine 3. and 4.
//  5. other, more difficult set of blocks (disable 2. 3. 4.)
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
    virtual std::shared_ptr<Level> next_level() = 0;
    virtual void do_something_with_field(const std::shared_ptr<Field> field) = 0;
};

class Level_1 : public Level
{
public:
    Level_1() : Level() {}
    ~Level_1() {}

    int get_number() const override;
    std::shared_ptr<Level> next_level() override;
    void do_something_with_field(const std::shared_ptr<Field> field) override;
};

class Level_2 : public Level
{
public:
    Level_2() : Level() {}
    ~Level_2() {}

    int get_number() const override;
    std::shared_ptr<Level> next_level() override;
    void do_something_with_field(const std::shared_ptr<Field> field) override;
};