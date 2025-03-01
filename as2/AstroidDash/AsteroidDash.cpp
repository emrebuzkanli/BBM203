#include "AsteroidDash.h"
#include <fstream>
#include <sstream>
#include <iostream>


// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);       // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    std::ifstream file(input_file);
    space_grid.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream line_stream(line);
        std::vector<int> row;
        int cell;
        while (line_stream >> cell) {
            row.push_back(cell);
        }
        space_grid.push_back(row);
    }
    file.close();
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    std::ifstream infile(player_file_name);
    int start_row, start_col;
    infile >> start_row >> start_col;
    infile.ignore();

    std::vector<std::vector<bool>> shape;
    std::string line;
    while (std::getline(infile, line)) {
        std::vector<bool> row;
        for (char ch : line) {
            if (ch == '1' || ch == '0') {
                row.push_back(ch == '1');
            }
        }
        shape.push_back(row);
    }
    player = new Player(shape, start_row, start_col, player_name);
    infile.close();
}
// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const std::string& input_file) {
    std::ifstream infile(input_file);
    std::string line;
    std::vector<std::vector<bool>> shape;
    int start_row = -1;
    int time_of_appearance = -1;
    ObjectType object_type = ASTEROID;
    std::string effect;

    CelestialObject* prev_object = nullptr;

    while (std::getline(infile, line)) {
        if (line.empty()) continue;  // Skip empty lines

        if (line[0] == '[' || line[0] == '{') {
            if (!shape.empty() && start_row != -1 && time_of_appearance != -1) {
                CelestialObject* new_object = new CelestialObject(shape, object_type, start_row, time_of_appearance);

                if (!celestial_objects_list_head) {
                    celestial_objects_list_head = new_object;
                } else {
                    prev_object->next_celestial_object = new_object;
                }
                prev_object = new_object;



            }


            shape.clear();
            start_row = -1;
            time_of_appearance = -1;
            effect.clear();
            object_type = ASTEROID;

            do {
                std::vector<bool> row;
                for (char c : line) {
                    if (c == '1' || c == '0') row.push_back(c == '1');
                }
                if (!row.empty()) shape.push_back(row);
            } while (std::getline(infile, line) && !line.empty() && (line[0] == '0' || line[0] == '1'));
        }

        if (line[0] == 's') {
            start_row = std::stoi(line.substr(2));

        } else if (line[0] == 't') {
            time_of_appearance = std::stoi(line.substr(2));
        } else if (line[0] == 'e') {
            effect = line.substr(2);
            if (effect == "life") {
                object_type = LIFE_UP;
            } else if (effect == "ammo") {
                object_type = AMMO;
            }


        }
    }
    if (!shape.empty() && start_row != -1 && time_of_appearance != -1) {
        CelestialObject* new_object = new CelestialObject(shape, object_type, start_row, time_of_appearance);

        if (!celestial_objects_list_head) {
            celestial_objects_list_head = new_object;
        } else {
            prev_object->next_celestial_object = new_object;
        }
        prev_object = new_object;


    }

    infile.close();
}

// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    std::cout << "Tick: " << game_time << std::endl;
    std::cout << "Lives: " << player->lives << std::endl;
    std::cout << "Ammo: " << player->current_ammo << std::endl;
    std::cout << "Score: " << current_score << std::endl;
    unsigned long high_score = 0;
    if (leaderboard.head_leaderboard_entry != nullptr) {
        high_score = leaderboard.head_leaderboard_entry->score;
    }
    std::cout << "High Score: " << high_score << std::endl;

    if(game_over) {
        std::cout << "Player: " << player->player_name << std::endl;
    }
    for (const std::vector<int>& row : space_grid) {
        for (int cell : row) {
            if (cell == 0)
                std::cout << unoccupiedCellChar;
            else
                std::cout << occupiedCellChar;
        }

        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    for (std::vector<int>& row : space_grid) {
        std::fill(row.begin(), row.end(), 0);
    }

    if (game_over) {
        const std::vector<std::vector<bool>>& player_shape = player->spacecraft_shape;
        for (int i = 0; i < player_shape.size(); ++i) {
            for (int j = 0; j < player_shape[i].size(); ++j) {
                int player_row = player->position_row + i;
                int player_col = player->position_col + j;
                if (player_shape[i][j] && player_row < space_grid.size() && player_col < space_grid[0].size()) {
                    space_grid[player_row][player_col] = 0;
                }
            }
        }
        return;
    }


    int player_row = player->position_row;
    int player_col = player->position_col;
    current_score = current_score + 1;
    const std::vector<std::vector<bool>>& player_shape = player->spacecraft_shape;

    for (int i = 0; i < player_shape.size(); ++i) {
        for (int j = 0; j < player_shape[i].size(); ++j) {
            if (player_shape[i][j] && player_row + i < space_grid.size() && player_col + j < space_grid[0].size()) {
                space_grid[player_row + i][player_col + j] = 1;
            }
        }
    }


    CelestialObject* current = celestial_objects_list_head;
    CelestialObject* previous = nullptr;

    while (current != nullptr) {
        bool object_collided = false;


        if (current->time_of_appearance <= game_time) {
            int row = current->starting_row;
            int col = space_grid[0].size() - 1 - (game_time - current->time_of_appearance);


            if (col + space_grid[0].size() >= 0) {
                const std::vector<std::vector<bool>>& shape = current->shape;
                for (int i = 0; i < shape.size(); ++i) {
                    int gridRow = row + i;
                    for (int j = 0; j < shape[i].size(); ++j) {
                        int gridCol = col + j;
                        if (shape[i][j] && gridRow >= 0 && gridRow < space_grid.size() &&
                            gridCol >= 0 && gridCol < space_grid[0].size()) {

                            if (space_grid[gridRow][gridCol] == 1) {
                                object_collided = true;

                                if (current->object_type == ObjectType::ASTEROID) {
                                    player->lives -= 1;
                                    current_score = current_score - 1;

                                    if (player->lives <= 0) {
                                        game_over = true;
                                        break;
                                    }
                                } else if (current->object_type == ObjectType::LIFE_UP) {
                                    player->lives += 1;
                                } else if (current->object_type == ObjectType::AMMO) {
                                    player->current_ammo = player->max_ammo;
                                }

                                for (int k = 0; k < shape.size(); ++k) {
                                    for (int l = 0; l < shape[k].size(); ++l) {
                                        if (shape[k][l]) {
                                            int clearRow = row + k;
                                            int clearCol = col + l;
                                            if (clearRow >= 0 && clearRow < space_grid.size() &&
                                                clearCol >= 0 && clearCol < space_grid[0].size()) {
                                                space_grid[clearRow][clearCol] = 0;
                                            }
                                        }
                                    }
                                }
                                break;
                            } else {
                                space_grid[gridRow][gridCol] = 1;
                            }
                        }
                    }
                    if (object_collided) break;
                }
            }
        }

        if (object_collided) {
            if (previous == nullptr) {
                celestial_objects_list_head = current->next_celestial_object;
            } else {
                previous->next_celestial_object = current->next_celestial_object;
            }
            CelestialObject* to_delete = current;
            current = current->next_celestial_object;
            delete to_delete;
        } else {
            previous = current;
            current = current->next_celestial_object;
        }
    }

    for (int i = 0; i < player->spacecraft_shape.size(); ++i) {
        for (int j = 0; j < player->spacecraft_shape[i].size(); ++j) {
            int player_row = player->position_row + i;
            int player_col = player->position_col + j;
            if (player->spacecraft_shape[i][j] && player_row < space_grid.size() &&
                player_col < space_grid[0].size()) {
                space_grid[player_row][player_col] = 1;
            }
        }
    }

    for (auto it = projectiles.begin(); it != projectiles.end();) {
        int proj_row = it->first;
        int proj_col = it->second;

        if (proj_col < space_grid[0].size()) {
            space_grid[proj_row][proj_col] = 1;
            it->second += 1;
            ++it;
        } else {
            it = projectiles.erase(it);
        }
    }
}





// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    if (player->current_ammo <= 0) {
        return;
    }

    int player_row = player->position_row;
    int player_col = player->position_col;
    int player_height = player->spacecraft_shape.size();

    int projectile_row = player_row + player_height / 2;
    int projectile_col = player_col + player->spacecraft_shape[0].size();

    projectiles.push_back(std::make_pair(projectile_row, projectile_col));
    int ammo=player->current_ammo;
    player->current_ammo =ammo-1;
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    if (player != nullptr) {
        delete player;
        player = nullptr;
    }

    CelestialObject* current = celestial_objects_list_head;
    while (current != nullptr) {
        CelestialObject* next = current->next_celestial_object;
        delete current;
        current = next;
    }
}
