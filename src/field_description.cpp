#include <spdlog/spdlog.h>
#include <fstream>
#include <ranges>
#include "include/field_description.h"

FieldDescription::FieldDescription(const std::filesystem::path &path)
{
    spdlog::info("Loading field description '{}' ...", path.filename().c_str());

    std::ifstream in(path, std::fstream::in);
    long maxsize = std::numeric_limits<std::streamsize>::max();
    if (in.is_open())
    {
        std::string line;
        std::getline(in, line);
        std::istringstream iss(line);
        iss >> height_;
        iss.ignore(maxsize, ':');
        iss >> width_;

        std::getline(in, line);
        std::istringstream iss2(line);
        iss2 >> start_col_;

        while (std::getline(in, line))
        {
            std::istringstream iss(line);
            while (!iss.eof())
            {
                std::pair<int, int> coordinates;
                iss >> coordinates.first;
                iss.ignore(maxsize, ',');
                iss >> coordinates.second;
                iss.ignore(maxsize, ' ');
                boundary_coordinates_.emplace_back(coordinates);
            }
        }
        in.close();
    }
    else
    {
        spdlog::error("Unable to open field description file");
    }
}

bool FieldDescription::is_valid() const
{
    // field should at least be able to contain a block (4x4)
    if (width_ < 6)
    {
        spdlog::info("Field description 'width' is too small");
        return false;
    }
    if (height_ < 4)
    {
        spdlog::info("Field description 'height' is too small");
        return false;
    }
    // field should have at least two boundary tiles for each row
    if (boundary_coordinates_.size() < height_ * 2)
    {
        spdlog::info("Field description number of 'boundary coordinates' is too small");
        return false;
    }
    // start position should be between the boundary coordinates of first row
    if (start_col_ <= boundary_coordinates_.at(0).second || start_col_ >= boundary_coordinates_.at(1).second)
    {
        spdlog::info("Field description 'start position' is invalid");
        return false;
    }

    // x-coordinate must be between 0 and width
    // y-coordinate must be between 1 and height
    for (const auto &[row, col] : boundary_coordinates_)
    {
        if (col < 0 || col >= width_)
        {
            spdlog::info("Field description column '{}' of coordinate '{},{}' is invalid", col, row, col);
            return false;
        }
        if (row < 1 || row > height_)
        {
            spdlog::info("Field description row '{}' of coordinate '{},{}' is invalid", row, row, col);
            return false;
        }
    }

    return true;
}

bool FieldDescription::contains_boundary_coordinates(int row, int col) const
{
    return std::ranges::find_if(boundary_coordinates_, [row, col](const std::pair<int, int> &p)
                                { return p.first == row && p.second == col; }) != boundary_coordinates_.end();
}
