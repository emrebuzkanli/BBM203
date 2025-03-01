#include "RoutePlanner.h"
#include <iostream>
#include <fstream>

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {

    // TO DO:
    // Load map data from file
    map.loadDistanceData(distance_data);
    // Load priority provinces
    loadPriorityProvinces(priority_data);
    // Load restricted provinces
    loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    std::string line;
    for (std::string line; std::getline(file, line); ) {
        if (numPriorityProvinces >= MAX_PRIORITY_PROVINCES) {
            break;
        }
        size_t start = line.find('(');
        size_t end = line.find(')');

        std::string indexString = line.substr(start + 1, end - start - 1);
        int provinceIndex = std::stoi(indexString);
        priorityProvinces[numPriorityProvinces] = provinceIndex;
        numPriorityProvinces=numPriorityProvinces + 1;

    }


    file.close();
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    for (std::string line; std::getline(file, line); ) {
        if (numWeatherRestrictedProvinces >= MAX_WEATHER_RESTRICTED_PROVINCES) {
            break;
        }
        size_t start = line.find('(');
        size_t end = line.find(')');
        std::string indexString = line.substr(start + 1, end - start - 1);
        int provinceIndex = std::stoi(indexString);
        weatherRestrictedProvinces[numWeatherRestrictedProvinces] = provinceIndex;
        numWeatherRestrictedProvinces=numWeatherRestrictedProvinces + 1;
        }
    file.close();
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    // TODO: Your code here
    for (int i = 0; i < numPriorityProvinces; ++i) {
        if (priorityProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    // TODO: Your code here
    for (int i = 0; i < numWeatherRestrictedProvinces; ++i) {
        if (weatherRestrictedProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    // TODO: Your code here
    map.markAsVisited(startingCity);
    stack.push(startingCity);
    route.push_back(startingCity);
    exploreFromProvince(startingCity);
    displayResults();
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    // TODO: Your code here
    enqueueNeighbors(province);
    while (!queue.isEmpty()) {
        int nextProvince = queue.dequeue();
        if (map.isVisited(nextProvince)) {
            continue;
        }
        map.markAsVisited(nextProvince);
        stack.push(nextProvince);
        route.push_back(nextProvince);
        exploreFromProvince(nextProvince);
    }
    backtrack();
}

void RoutePlanner::enqueueNeighbors(int province) {
    // TO DO: Enqueue priority & non-priority neighbors to the queue according to given constraints
    for (int i = 0; i < MAX_SIZE; ++i) {
        if (map.isVisited(i)) {
            continue;
        }
        if (isWeatherRestricted(i)) {
            std::cout << "Province " << cities[i] << " is weather-restricted. Skipping." << std::endl;
            continue;
        }
        if (!map.isWithinRange(province, i, maxDistance)) {
            continue;
        }
        if (isPriorityProvince(i)) {
            queue.enqueuePriority(i);
        } else {
            queue.enqueue(i);
        }
    }
}

void RoutePlanner::backtrack() {
    // If you reach a dead-end province
    // TODO: Your code here
    // Check if backtracking is possible
    if (isExplorationComplete()) {
        return;
    }
    stack.pop();
    if (isExplorationComplete()) {
        return;
    }
    int currentProvince = stack.peek();
    enqueueNeighbors(currentProvince);
}

bool RoutePlanner::isExplorationComplete() const {
    // TODO: Your code here
    if (stack.isEmpty() && queue.isEmpty()) {
        return true;
    }
    return false;
}

void RoutePlanner::displayResults() const {
    // TODO: Your code here
    // Display "Journey Completed!" message
    std::cout << "----------------------------\n";
    std::cout << "Journey Completed!\n";
    std::cout << "----------------------------\n";

    // Display the total number of provinces visited
    int totalVisited = route.size();
    std::cout << "Total Number of Provinces Visited: " << totalVisited << "\n";

    // Calculate the total distance dynamically
    int totalDistanceCovered = 0;
    for (size_t i = 1; i < route.size(); ++i) {
        totalDistanceCovered += map.getDistance(route[i - 1], route[i]);
    }

    // Display the total distance covered
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km\n";

    // Display the route in the order visited
    std::cout << "Route Taken:\n";
    for (size_t i = 0; i < route.size(); ++i) {
        std::cout << cities[route[i]];
        if (i < route.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << "\n\n";

    // Priority Province Summary
    std::cout << "Priority Provinces Status:\n";
    int priorityVisitedCount = 0;
    for (int i = 0; i < numPriorityProvinces; ++i) {
        int province = priorityProvinces[i];
        std::cout << "- " << cities[province] << " (";
        if (map.isVisited(province)) {
            std::cout << "Visited";
            ++priorityVisitedCount;
        } else {
            std::cout << "Not Visited";
        }
        std::cout << ")\n";
    }

    std::cout << "Total Priority Provinces Visited: " << priorityVisitedCount
              << " out of " << numPriorityProvinces << "\n";
    if (priorityVisitedCount == numPriorityProvinces) {
        std::cout << "Success: All priority provinces were visited.\n";
    } else {
        std::cout << "Warning: Not all priority provinces were visited.\n";
    }
}


