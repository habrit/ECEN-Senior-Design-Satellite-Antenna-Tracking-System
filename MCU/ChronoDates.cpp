#include <iostream>
#include <chrono>

int main() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::gmtime(&time);

    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    int hour = tm->tm_hour;
    int minute = tm->tm_min;
    int second = tm->tm_sec;

    std::cout << "Date: " << year << "-" << month << "-" << day << std::endl;
    std::cout << "Time: " << hour << ":" << minute << ":" << second << std::endl;

    return 0;
}
