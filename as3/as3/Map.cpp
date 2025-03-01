#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {
    // TODO: Your code here
    // Initialize all distances to a value representing no direct connection
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            distanceMatrix[i][j] = -1;
        }
    }
    // Initialize all provinces as unvisited
    resetVisited();
}


// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    std::string line;
    int row = 0;

    // Read each line in the CSV file
    while (std::getline(file, line) && row < MAX_SIZE) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;

        // Read each cell separated by a comma
        while (std::getline(ss, cell, ',') && col < MAX_SIZE) {
            distanceMatrix[row][col] = std::stoi(cell); // Convert cell to an integer and store in distanceMatrix
            col=col+1;
        }

        row=row+1;
    }
    file.close();
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here
    int distance = distanceMatrix[provinceA][provinceB];

    if (distance <= maxDistance) {
        return true;
    }
    return false;
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    // TODO: Your code here
    visited[province] = true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    // TODO: Your code here
    if (visited[province]) {
        return true;
    }
    return false;
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code
    for (int i = 0; i < MAX_SIZE; ++i) {
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    // TODO: Your code here
    int counter = 0;
    for (int i = 0; i < MAX_SIZE; ++i) {
        if (visited[i]) {
            counter=counter+1;
        }
    }
    return counter;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    // TODO: Your code here
    int distance = distanceMatrix[provinceA][provinceB];
    return distance;
}