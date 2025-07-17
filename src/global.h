#pragma once

constexpr int ROWS = 27; // 1 invisible at the top, 25 playfield, 1 boundary at the bottom
constexpr int COLS = 12; // 1 boundary at the left, 10 playfield, 1 boundary at the right

constexpr float SIZE_TILE = 20.0f;
constexpr int X_NEXT_BLOCK = (COLS * SIZE_TILE) + 50;
constexpr int Y_NEXT_BLOCK = 60;

struct GameState
{
    bool game_over{false};
    int nr_of_full_lines{0};
};