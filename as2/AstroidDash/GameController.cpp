#include "GameController.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip> // For formatting output

// Simply instantiates the game
GameController::GameController(const std::string &space_grid_file_name,
                               const std::string &celestial_objects_file_name,
                               const std::string &leaderboard_file_name,
                               const std::string &player_file_name,
                               const std::string &player_name) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name, player_name);
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const std::string &commands_file) {
    std::ifstream file(commands_file);
    std::string command;


    while (std::getline(file, command)) {

        if (game->game_over) {
            std::cout << "GAME OVER!" << std::endl;
            game->update_space_grid();
            game->print_space_grid();
            game->leaderboard.print_leaderboard();
            game->leaderboard.write_to_file(game->leaderboard_file_name);
            break;
        }

        game->update_space_grid();

        std::istringstream command_stream(command);
        std::string action;
        command_stream >> action;


        if (action == "PRINT_GRID") {
            game->print_space_grid();
        } else if (action == "MOVE_UP") {
            game->player->move_up();
        } else if (action == "MOVE_DOWN") {
            game->player->move_down(game->space_grid.size());
        } else if (action == "MOVE_LEFT") {
            game->player->move_left();
        } else if (action == "MOVE_RIGHT") {
            game->player->move_right(game->space_grid[0].size());
        } else if (action == "SHOOT") {
            game->shoot();
        }


        game->game_time++;
    }

    file.close();


    if (!game->game_over) {
        std::cout << "GAME FINISHED! No more commands!" << std::endl;
        game->game_over = true;
        game->print_space_grid();
        game->leaderboard.print_leaderboard();
        game->leaderboard.write_to_file(game->leaderboard_file_name);
    }
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    delete game;
}
