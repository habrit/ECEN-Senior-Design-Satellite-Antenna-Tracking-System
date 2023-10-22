#include <iostream>
#include <string>
#include <sstream>

int main() {
    std::string line1, line2, line3; // Variables for storing the three lines

    // Read and store the three lines
    if (std::getline(std::cin, line1) && std::getline(std::cin, line2) && std::getline(std::cin, line3)) {
        // Print the collected lines
        std::cout << "Line 1: " << line1 << std::endl;
        std::cout << "Line 2: " << line2 << std::endl;
        std::cout << "Line 3: " << line3 << std::endl;
    } else {
        std::cerr << "Error reading lines from stdin." << std::endl;
        return 1;
    }

    return 0;
}