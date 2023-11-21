#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

int main()
{
    while (true)
    {
        // Execute the terminal command using std::system
        std::system("date");

        // Sleep for one second before the next iteration
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}