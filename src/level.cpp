#include "level.h"

#include "field.h"

int Level_1::get_number() const
{
    return 1;
}

std::shared_ptr<Level> Level_1::next_level()
{
    return std::make_shared<Level_2>();
}

void Level_1::do_something_with_field(const std::shared_ptr<Field> field)
{
}

int Level_2::get_number() const
{
    return 2;
}

std::shared_ptr<Level> Level_2::next_level()
{
    return std::make_shared<Level_2>();
}

void Level_2::do_something_with_field(const std::shared_ptr<Field> field)
{
}