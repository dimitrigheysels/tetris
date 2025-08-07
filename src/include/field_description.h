#pragma once

#include <vector>
#include <filesystem>

// structure of file:
// width:height
// for each row, x,y coordinates of the boundary tiles
// row start at 1, col start at 0
// 0,1 11,1
// 0,2 5,2 11,2
class FieldDescription
{
private:
    int width_{0};
    int height_{0};
    int start_col_{0};
    std::vector<std::pair<int, int>> boundary_coordinates_;

public:
    FieldDescription(const std::filesystem::path &path);

    inline int get_width() const { return width_; }
    inline int get_height() const { return height_; }
    inline int get_start_col() const { return start_col_; }

    bool is_valid() const;

    bool contains_boundary_coordinates(int x, int y) const;
};