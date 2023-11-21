#include "Functions.cpp"

// On first start it checks to make sure that the rotator is set to allow accurate rotation
// Rotate Rotator current AZ degrees to zero stop
// Rotate Rotator full 360 to 360* stop
// Rotate Rotator back to 0* stop

// Remember to remove gpioOut() comments on RPi, only here for testing purposes on PC
// Double check GPIO pins and Az/El wiring

void azHome()
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
            // std::cout << contents << std::endl;
        }
        azFile.close();
    }

    // Convert string to double
    // az = std::stod(contents);

    az = stringToDouble(contents);
    // std::cout << az << std::endl;
    //  Find time to rotate to 0 from current az
    double rotateTime = 0;
    rotateTime = az / 6.92;

    gpioOut(19, rotateTime, true);

    // Rotate from 0 to 360* stop

    gpioOut(26, 52, true);

    // Rotate from 360* to 0* stop

    gpioOut(19, 55, false);

    // Set Azimuth to 0 in AzimuthEx.txt
    std::ofstream azFileOut(azPath);
    azFileOut << "0";
    azFileOut.close();
}

void elHome()
{
std::string contents;
double rotateTime = 0;
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
    el = stringToDouble(elContents);
    // Find time to rotate to 0 from current el
    double elRotateTime = 0;
    elRotateTime = el / 2.903;

    gpioOut(13, rotateTime, true);

    // Rotate from 0 to 180* stop

    gpioOut(6, 65, true);

    // Rotate from 180* to 0* stop

    gpioOut(13, 65, true);

    // Set Elevation to 0 in ElevationEx.txt
    std::ofstream elFileOut(elPath);
    elFileOut << "0";
    elFileOut.close();
}

int main()
{
    //Thread azHome and elHome
    std::thread azHomeThread(azHome);
    std::thread elHomeThread(elHome);
    //join
    azHomeThread.join();
    elHomeThread.join();
    return 0;
}