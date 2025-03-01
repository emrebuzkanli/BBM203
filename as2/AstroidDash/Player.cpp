#include "Player.h"

// Constructor to initialize the player's spacecraft, position, and ammo
Player::Player(const vector<vector<bool>> &shape, int row, int col, const string &player_name, int max_ammo, int lives)
        : spacecraft_shape(shape), position_row(row), position_col(col), player_name(player_name), max_ammo(max_ammo),
          current_ammo(max_ammo), lives(lives) {
    // TODO: Your code here, if you want to do further initializations
}

// Move player left within the grid boundaries
void Player::move_left() {
    if (position_col > 0) {
        position_col--;
    }
}

// Move player right within the grid boundaries
void Player::move_right(int grid_width) {
    int player_width = spacecraft_shape[0].size();
    int right_col = position_col + player_width;
    if (right_col < grid_width) {
        position_col++;
    }
}

// Move player up within the grid boundaries
void Player::move_up() {
    if (position_row > 0) {
        position_row--;
    }
}

// Move player down within the grid boundaries
void Player::move_down(int grid_height) {
    int player_height = spacecraft_shape.size();
    int top_row = position_row + player_height;
    if (top_row < grid_height) {
        position_row++;
    }
}