#include <iostream>
#include <fstream>
#include <string>

int main() {
    // Open the file for reading
    std::ifstream file("AzimuthEx.txt");

    // Check if the file is open
    if (file.is_open()) {
        // Read the contents of the file into a string
        std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        // Close the file
        file.close();

        // Output the original contents of the file
        std::cout << "Original contents: " << contents << std::endl;

        // Open the file for writing
        std::ofstream outfile("AzimuthEx.txt");

        // Check if the file is open
        if (outfile.is_open()) {
            // Write the new contents to the file
            outfile << "184.6";

            // Close the file
            outfile.close();
        }
    }

    return 0;
}
