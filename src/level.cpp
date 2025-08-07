
#include "include/level.h"
#include "include/field.h"

// ========= LEVEL 1 =========
// ===========================

void Level_1::do_something_with_field(const std::shared_ptr<Field> field) const
{
}

// ========= LEVEL 2 =========
// ===========================

void Level_2::do_something_with_field(const std::shared_ptr<Field> field) const
{
}

// ========= LEVEL 3 =========
// ===========================

void Level_3::do_something_with_field(const std::shared_ptr<Field> field) const
{
}

// ========= LEVEL 4 =========
// ===========================

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

// ========= LEVEL 5 =========
// ===========================

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

// ========= LEVEL 6 =========
// ===========================

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
