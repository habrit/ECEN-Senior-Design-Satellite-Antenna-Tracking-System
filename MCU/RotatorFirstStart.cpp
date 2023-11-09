#include "Functions.cpp"

// On first start it checks to make sure that the rotator is set to allow accurate rotation
// Rotate Rotator current AZ degrees to zero stop
// Rotate Rotator full 360 to 360* stop
// Rotate Rotator back to 0* stop

// Remember to remove gpioOut() comments on RPi, only here for testing purposes on PC
//Double check GPIO pins and Az/El wiring

int main()
{
    // Azimuth
    double az = 0;
    // Reading current Azimuth of rotator
    std::string azPath = "AzimuthEx.txt";
    std::ifstream azFile(azPath);
    std::string contents;

    if (azFile.is_open())
    {
        std::string line;
        while (getline(azFile, line))
        {
            contents += line + "\n";
        }
        azFile.close();
    }

    // Convert string to double
    az = std::stod(contents);
    // Find time to rotate to 0 from current az
    double rotateTime = 0;
    rotateTime = az / 6.92;

    // gpioOut(22, rotateTime, false);

    // Rotate from 0 to 360* stop

    // gpioOut(27, 52, false);

    // Rotate from 360* to 0* stop

    // gpioOut(22, 52, false);

    // Elevation
    double el = 0;
    // Reading current Elevation of rotator
    std::string elPath = "ElevationEx.txt";
    std::ifstream elFile(elPath);
    std::string elContents;

    if (elFile.is_open())
    {
        std::string line;
        while (getline(elFile, line))
        {
            contents += line + "\n";
        }
        elFile.close();
    }
    el = std::stod(elContents);
    // Find time to rotate to 0 from current el
    double elRotateTime = 0;
    elRotateTime = el / 6.92;

    // gpioOut(3, rotateTime, false);

    // Rotate from 0 to 180* stop

    // gpioOut(4, 28, false);

    // Rotate from 180* to 0* stop

    // gpioOut(4, 28, false);
}