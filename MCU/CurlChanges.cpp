#include <iostream>
#include <fstream>
#include <string>

int main()
{
    // std::string file1_path, file2_path;
    // std::cout << "Enter path to first file: ";
    // std::cin >> file1_path;
    // std::cout << "Enter path to second file: ";
    // std::cin >> file2_path;

    std::string file1_path = "pretty.json";
    std::string file2_path = "pretty2.json";

    std::ifstream file1(file1_path), file2(file2_path);
    std::string line1, line2;
    int line_num = 1;

    while (std::getline(file1, line1) && std::getline(file2, line2))
    {
        if (line1 != line2)
        {
            std::cout << "Difference found on line " << line_num << ":\n";
            std::cout << "File 1: " << line1 << "\n";
            std::cout << "File 2: " << line2 << "\n";
        }
        line_num++;
    }

    if (std::getline(file1, line1))
    {
        std::cout << "File 1 has additional lines:\n";
        while (std::getline(file1, line1))
        {
            std::cout << line1 << "\n";
        }
    }

    if (std::getline(file2, line2))
    {
        std::cout << "File 2 has additional lines:\n";
        while (std::getline(file2, line2))
        {
            std::cout << line2 << "\n";
        }
    }

    return 0;
}
