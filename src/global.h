#pragma once

constexpr int PLAYFIELD_TOP_ROW = 1;
constexpr int PLAYFIELD_HEIGHT = 30; // 25;
constexpr int PLAYFIELD_BOTTOM_ROW = PLAYFIELD_HEIGHT;
constexpr int PLAYFIELD_FIRST_COL = 1;
constexpr int PLAYFIELD_WIDTH = 30; // 10;
constexpr int PLAYFIELD_LAST_COL = PLAYFIELD_WIDTH;
constexpr int MAX_ROWS = 30; // 1 invisible at the top, 25 playfield, 1 boundary at the bottom
constexpr int MAX_COLS = 30; // 1 boundary at the left, 10 playfield, 1 boundary at the right

constexpr float SIZE_TILE = 20.0f;
constexpr int X_NEXT_BLOCK = (MAX_COLS * SIZE_TILE) + 50;
constexpr int Y_NEXT_BLOCK = 40;

constexpr int BLOCK_LAYOUT_SIZE = 4;

// ======== GAMEPLAY SETTINGS ========
constexpr int SCORE_1_LINE = 1;
constexpr int SCORE_2_LINE = 3;
constexpr int SCORE_3_LINE = 6;
constexpr int SCORE_4_LINE = 10;
constexpr int SCORE_NEXT_LEVEL = 100;
constexpr int BONUS_EVERY_LINES = 100;
constexpr int BONUS_LINES = 4;

struct GameState
{
    bool game_over{false};
    int nr_of_full_lines{0};
    bool new_block{false};
};