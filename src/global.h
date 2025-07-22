#pragma once

constexpr int PLAYFIELD_TOP_ROW = 1;
constexpr int PLAYFIELD_HEIGHT = 25;
constexpr int PLAYFIELD_BOTTOM_ROW = PLAYFIELD_HEIGHT;
constexpr int PLAYFIELD_FIRST_COL = 1;
constexpr int PLAYFIELD_WIDTH = 10;
constexpr int PLAYFIELD_LAST_COL = PLAYFIELD_WIDTH;
constexpr int ROWS = 25 + 1 + 1; // 1 invisible at the top, 25 playfield, 1 boundary at the bottom
constexpr int COLS = 10 + 1 + 1; // 1 boundary at the left, 10 playfield, 1 boundary at the right

constexpr float SIZE_TILE = 20.0f;
constexpr int X_NEXT_BLOCK = (COLS * SIZE_TILE) + 50;
constexpr int Y_NEXT_BLOCK = 40;

constexpr int BLOCK_LAYOUT_SIZE = 4;

struct GameState
{
    bool game_over{false};
    int nr_of_full_lines{0};
    bool new_block{false};
};