#include "Functions.cpp"
int main()
{
    std::string filename = "ISSZARYA.json";
    std::string targetString = "azimuth";
    double az = 0;
    double el = 0;
    char acBuffer[20]; // Buffer for ASCII time

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::gmtime(&time);

    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    int hour = tm->tm_hour;
    int minute = tm->tm_min;
    int second = tm->tm_sec;

    DateTime MyTime(year, month, day, hour, minute, second); // Set start time for the prediction
    MyTime.ascii(acBuffer);                                  // Get time for prediction as ASCII string
    std::cout << "Time: " << acBuffer << std::endl;

    std::string targetTime = acBuffer;

    az = jsonTimeAZLookup(filename, targetTime);
    std::cout << "az: " << az << std::endl;

    el = jsonTimeELLookup(filename, targetTime);
    std::cout << "el: " << el << std::endl;
    return 0;
}
