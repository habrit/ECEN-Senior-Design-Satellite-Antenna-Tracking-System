#include "Functions.cpp"


int main(){
    double az = 0;
    double rotateTime = 0;
    // Reading current Azimuth of rotator
    std::string azPath = "AzimuthEx.txt";
    std::ifstream azFile(azPath);
    std::string contents;

    if (azFile.is_open()) {
        std::string line;
        while (getline(azFile, line)) {
            contents += line + "\n";
        }
        azFile.close();
    }

    // Convert string to double
    az = std::stod(contents);
    std::cout << "Current Azimuth: " << az << std::endl;

    //Current AZ of selected satellite
    std::string filename = "ISSZARYA.json";
    std::string targetString = "azimuth";
    double azSat = 0;
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

    azSat= jsonTimeAZLookup(filename, targetTime);
    std::cout << "az: " << azSat << std::endl;

    double azDiff = az - azSat;
    std::cout << "azDiff: " << azDiff << std::endl;

    //Rotator rotates 6.92 degrees per second
    rotateTime = azDiff / 6.92;
    std::cout << "rotateTime: " << rotateTime << std::endl;


    return 0;
}