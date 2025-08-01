
#include "level.h"
#include "field.h"

// ========= LEVEL 1 =========
// ===========================
int Level_1::get_number() const
{
    return 1;
}

std::shared_ptr<Level> Level_1::next_level() const
{
    return std::make_shared<Level_2>();
}

void Level_1::do_something_with_field(const std::shared_ptr<Field> field) const
{
}

float Level_1::speed() const
{
    return 0.5f;
}

int Level_1::event_countdown_in_seconds() const
{
    return 0;
}

// ========= LEVEL 2 =========
// ===========================
int Level_2::get_number() const
{
    return 2;
}

std::shared_ptr<Level> Level_2::next_level() const
{
    return std::make_shared<Level_3>();
}

void Level_2::do_something_with_field(const std::shared_ptr<Field> field) const
{
}

float Level_2::speed() const
{
    return 0.3f;
}

int Level_2::event_countdown_in_seconds() const
{
    return 0;
}

// ========= LEVEL 3 =========
// ===========================
int Level_3::get_number() const
{
    return 3;
}

std::shared_ptr<Level> Level_3::next_level() const
{
    return std::make_shared<Level_4>();
}

void Level_3::do_something_with_field(const std::shared_ptr<Field> field) const
{
}

float Level_3::speed() const
{

    return 0.15f;
}

int Level_3::event_countdown_in_seconds() const
{
    return 0;
}

// ========= LEVEL 4 =========
// ===========================
int Level_4::get_number() const
{
    return 4;
}

std::shared_ptr<Level> Level_4::next_level() const
{
    return std::make_shared<Level_5>();
}

void Level_4::do_something_with_field(const std::shared_ptr<Field> field) const
{
    int top_row = field->get_top_row();

    // current stack is max 16 high
    if (top_row > 15)
    {
        // scatter any number of existing rows between [toprow, toprow+3]
        field->scatter_rows(top_row, top_row + 2);
    }
}

float Level_4::speed() const
{
    return 0.5f;
}

int Level_4::event_countdown_in_seconds() const
{
    return std::experimental::randint(10, 20);
}

// ========= LEVEL 5 =========
// ===========================
int Level_5::get_number() const
{
    return 5;
}

std::shared_ptr<Level> Level_5::next_level() const
{
    return std::make_shared<Level_6>();
}

void Level_5::do_something_with_field(const std::shared_ptr<Field> field) const
{
    int top_row = field->get_top_row();

    // current stack is max 16 high
    if (top_row > 15)
    {
        // add max 3 new scattered rows
        field->add_scattered_rows(top_row, top_row - 2);
    }
}

float Level_5::speed() const
{
    return 0.5f;
}

int Level_5::event_countdown_in_seconds() const
{
    return std::experimental::randint(15, 25);
}

// ========= LEVEL 6 =========
// ===========================
int Level_6::get_number() const
{
    return 6;
}

std::shared_ptr<Level> Level_6::next_level() const
{
    return std::make_shared<Level_6>();
}

void Level_6::do_something_with_field(const std::shared_ptr<Field> field) const
{
    int top_row = field->get_top_row();

    // current stack is max 16 high
    if (top_row > 15)
    {
        // add max 3 new scattered rows
        field->scatter_rows(top_row, top_row + 3);
        field->add_scattered_rows(top_row, top_row - 3);
    }
}

float Level_6::speed() const
{
    return 0.5f;
}

int Level_6::event_countdown_in_seconds() const
{
    return std::experimental::randint(20, 30);
}