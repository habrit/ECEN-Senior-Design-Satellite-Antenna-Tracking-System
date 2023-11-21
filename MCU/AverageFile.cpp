#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

int main()
{
    std::ifstream file("time.txt");
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::vector<double> numbers;
    double num;
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        while (ss >> num)
        {
            numbers.push_back(num);
        }
    }

    if (numbers.empty())
    {
        std::cerr << "No numbers found in the file." << std::endl;
        return 1;
    }

    double sum = 0.0;
    for (double number : numbers)
    {
        sum += number;
    }

    double average = sum / numbers.size();
    std::cout << "Average of numbers in time.txt: " << average << std::endl;
    std::cout << "Number of samples: " << numbers.size() << std::endl;

    return 0;
}