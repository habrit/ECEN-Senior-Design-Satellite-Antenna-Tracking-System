#include "Celestrak.h"

void AzIncrement(double angle, int step)
{
    double rotateTime = 0;
    rotateTime = angle / 6.92;
    // Multiply by number of steps
    rotateTime = rotateTime * step;
    angle = angle * step;
    if (angle < 7)
    {
        // Convert seconds to microseconds
        rotateTime = rotateTime * 1000000;
        // gpioOutmicro(26, rotateTime, false);
    }
    else
    {
        // Rotate
        // gpioOut(26, rotateTime, false);
    }
    // Read Angle from AzimuthEx.txt
    double currentAz = 0;
    double newAz = 0;
    std::string azPath = "AzimuthEx.txt";
    std::ifstream azFile(azPath);
    std::string contents;
    if (azFile.is_open())
    {
        std::string line;
        while (getline(azFile, line))
        {
            contents += line + "\n";
            std::cout << contents << std::endl;
        }
        azFile.close();
    }

    // Convert string to double
    currentAz = stringToDouble(contents);
    newAz = currentAz + angle;

    // Set azimuth to newAz in AzimuthEx.txt
    std::ofstream azFileOut(azPath);
    azFileOut << newAz;
    azFileOut.close();
}

void AzDecrement(double angle, int step)
{
    double rotateTime = 0;
    rotateTime = angle / 6.92;
    // Multiply by number of steps
    rotateTime = rotateTime * step;
    angle = angle * step;
    // Rotate
    if (angle < 7)
    {
        // Convert seconds to microseconds
        rotateTime = rotateTime * 1000000;
        // gpioOutmicro(19, rotateTime, false);
    }
    else
    {
        // Rotate
        // gpioOut(19, rotateTime, false);
    }

    // Read Angle from AzimuthEx.txt
    double currentAz = 0;
    double newAz = 0;
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
    currentAz = stringToDouble(contents);
    newAz = currentAz - angle;

    // Set azimuth to newAz in AzimuthEx.txt
    std::ofstream azFileOut(azPath);
    azFileOut << newAz;
    azFileOut.close();
}

void ElIncrement(double angle, int step)
{
    double rotateTime = 0;
    rotateTime = angle / 2.903;
    // Multiply by number of steps
    rotateTime = rotateTime * step;
    angle = angle * step;
    // Rotate
    if (angle < 7)
    {
        // Convert seconds to microseconds
        rotateTime = rotateTime * 1000000;
        // gpioOutmicro(6, rotateTime, false);
    }
    else
    {
        // Rotate
        // gpioOut(6, rotateTime, false);
    }

    // Read Angle from ElevationEx.txt
    double currentEl = 0;
    double newEl = 0;
    std::string elPath = "ElevationEx.txt";
    std::ifstream elFile(elPath);
    std::string contents;
    if (elFile.is_open())
    {
        std::string line;
        while (getline(elFile, line))
        {
            contents += line + "\n";
            // std::cout << contents << std::endl;
        }
        elFile.close();
    }

    // Convert string to double
    currentEl = stringToDouble(contents);
    newEl = currentEl + angle;

    // Set azimuth to newEl in ElevationEx.txt
    std::ofstream ElFileOut(elPath);
    ElFileOut << newEl;
    ElFileOut.close();
}

void ElDecrement(double angle, int step)
{
    double rotateTime = 0;
    rotateTime = angle / 2.903;
    // Multiply by number of steps
    rotateTime = rotateTime * step;
    angle = angle * step;
    // Rotate
    if (angle < 7)
    {
        // Convert seconds to microseconds
        rotateTime = rotateTime * 1000000;
        // gpioOutmicro(13, rotateTime, false);
    }
    else
    {
        // Rotate
        // gpioOut(13, rotateTime, false);
    }

    // Read Angle from ElevationEx.txt
    double currentEl = 0;
    double newEl = 0;
    std::string elPath = "ElevationEx.txt";
    std::ifstream elFile(elPath);
    std::string contents;
    if (elFile.is_open())
    {
        std::string line;
        while (getline(elFile, line))
        {
            contents += line + "\n";
            // std::cout << contents << std::endl;
        }
        elFile.close();
    }

    // Convert string to double
    currentEl = stringToDouble(contents);
    newEl = currentEl - angle;

    // Set azimuth to newAz in AzimuthEx.txt
    std::ofstream elFileOut(elPath);
    elFileOut << newEl;
    elFileOut.close();
}

void moveToPosition(double az, double el)
{
    // Read Angle from AzimuthEx.txt
    double currentAz = 0;
    double newAz = 0;
    double rotateAngle = 0;
    bool azIncrement = false;
    bool azDecrement = false;

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
    currentAz = stringToDouble(contents);

    // Check if currentAz is greater than az
    if (currentAz > az)
    {
        // Rotate Azimuth to az
        rotateAngle = currentAz - az; // Decrement
        azDecrement = true;
    }
    else
    {
        // Rotate Azimuth to az
        rotateAngle = az - currentAz; // Increment
        azIncrement = true;
    }
    //--------------------------------------------------------------------------------------------

    // Read Angle from ElevationEx.txt
    double currentEl = 0;
    double newEl = 0;
    double rotateAngleEl = 0;
    bool elIncrement = false;
    bool elDecrement = false;

    std::string elPath = "ElevationEx.txt";
    std::ifstream elFile(elPath);
    std::string contentsel;
    if (elFile.is_open())
    {
        std::string line;
        while (getline(elFile, line))
        {
            contentsel += line + "\n";
            // std::cout << contents << std::endl;
        }
        elFile.close();
    }

    // Convert string to double
    currentEl = stringToDouble(contentsel);

    // Check if currentEl is greater than el
    if (currentEl > el)
    {
        // Rotate Elevation to el
        rotateAngleEl = currentEl - el; // Decrement
        elDecrement = true;
    }
    else
    {
        // Rotate Elevation to el
        rotateAngleEl = el - currentEl; // Increment
        elIncrement = true;
    }
    // std::cout << azIncrement << std::endl;
    // std::cout << azDecrement << std::endl;
    // std::cout << elIncrement << std::endl;
    // std::cout << elDecrement << std::endl;



    //Check to see if elevation is going to be greater than 90*, if greater than 90* start rotating back to zero and rotate azimuth 180*
    //Need a way to track if elevation is greater than 90* and then rotate back to zero and rotate azimuth  + 180* so that way it does not start to go back to 90*

    if (el > 90)
    {
        // Rotate Elevation to 0
        rotateAngleEl = currentEl - 0; // Decrement
        elDecrement = true;
        // Rotate Azimuth to az
        rotateAngle = 180 + rotateAngle; // Increment
        azIncrement = true;
    }

    // Rotate Azimuth and Elevations with threads
    if (azIncrement && elIncrement)
    {
        std::thread AzIncrementThread(AzIncrement, rotateAngle, 1);
        std::thread ElIncrementThread(ElIncrement, rotateAngleEl, 1);
        AzIncrementThread.join();
        ElIncrementThread.join();
    }
    else if (azIncrement && elDecrement)
    {
        std::thread AzIncrementThread(AzIncrement, rotateAngle, 1);
        std::thread ElDecrementThread(ElDecrement, rotateAngleEl, 1);
        AzIncrementThread.join();
        ElDecrementThread.join();
    }
    else if (azDecrement && elIncrement)
    {
        std::thread AzDecrementThread(AzDecrement, rotateAngle, 1);
        std::thread ElIncrementThread(ElIncrement, rotateAngleEl, 1);
        AzDecrementThread.join();
        ElIncrementThread.join();
    }
    else if (azDecrement && elDecrement)
    {
        std::thread AzDecrementThread(AzDecrement, rotateAngle, 1);
        std::thread ElDecrementThread(ElDecrement, rotateAngleEl, 1);
        AzDecrementThread.join();
        ElDecrementThread.join();
    }

    // Set Azimuth and Elevation to az and el in AzimuthEx.txt and ElevationEx.txt
    std::ofstream azFileOut(azPath);
    azFileOut << az;
    azFileOut.close();

    std::ofstream elFileOut(elPath);
    elFileOut << el;
    elFileOut.close();
}

void trackSatellite(std::string SatelliteName)
{
    std::cout << stopTracking << std::endl;
    while (!stopTracking)
    {
        // std::cout << "In trackSatellite" << std::endl;
        //  std::cout << "----------------------------------------" << std::endl;
        //   Find current time in UTC
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto tm = std::gmtime(&time);

        int year = tm->tm_year + 1900;
        int month = tm->tm_mon + 1;
        int day = tm->tm_mday;
        int hour = tm->tm_hour;
        int minute = tm->tm_min;
        int second = tm->tm_sec;

        // Find time in UTC + 30 seconds
        second = second + 30;
        if (second > 59)
        {
            second = second - 60;
            minute = minute + 1;
        }
        std::string tleNameString = SatelliteName;
        // std::cout << "TLENAMESTRING + " << tleNameString << std::endl;

        std::vector<std::string> tleNameVector = findLineInJson("Celestrak.txt", tleNameString);

        // If segmentation fault, check if tleNameVector and print "Not found" and continue, do not break
        if (tleNameVector.empty())
        {
            std::cout << "Not found" << std::endl;
            return;
        }
        std::string tlel1 = tleNameVector[0];
        // std::cout << "TLEL1 + " << tlel1 << std::endl;

        // Remove spaces, parenthesis
        tlel1.erase(std::remove_if(tlel1.begin(), tlel1.end(), ::isspace), tlel1.end()); // Remove Spaces
        tlel1.erase(std::remove(tlel1.begin(), tlel1.end(), '('), tlel1.end());
        tlel1.erase(std::remove(tlel1.begin(), tlel1.end(), ')'), tlel1.end());

        // std::cout << "TLE CLEANED MINUS SPACES AND PARANTHS: " << tlel1 << std::endl;

        // Convert to "Time": "2023-10-23 00:24:43" format
        std::string timeString = std::to_string(year) + "-" + (month < 10 ? "0" : "") + std::to_string(month) + "-" + (day < 10 ? "0" : "") + std::to_string(day) + " " + (hour < 10 ? "0" : "") + std::to_string(hour) + ":" + (minute < 10 ? "0" : "") + std::to_string(minute) + ":" + (second < 10 ? "0" : "") + std::to_string(second);

        std::cout << timeString << std::endl;
        std::string jsonName = tlel1 + ".json";

        std::vector<std::string> timeJSON = findLineInJson(jsonName, timeString);

        //Get TLE L1 and L2
        std::string tlel2 = tleNameVector[1];
        //std::cout << "TLEL2 + " << tlel2 << std::endl;
        std::string tlel3 = tleNameVector[2];
        //std::cout << "TLEL3 + " << tlel3 << std::endl;

        DateTime dt(year, month, day, hour, minute, second);
        Observer obs(ObserverLocation, myLat, myLong, myAlt);
        Satellite sat(tleNameString.c_str(), tlel2.c_str(), tlel3.c_str());
        sat.doppler(145.8, true);
        //print out doppler
        std::cout << "Doppler is: " << sat.doppler(145.8, true) << std::endl;
        // While timeJSON is empty, keep trying to find the time
        while (timeJSON.empty())
        {
            // timestring + 1 second
            second = second + 1;
            if (second > 59)
            {
                second = second - 60;
                minute = minute + 1;
            }
            // Convert to "Time": "2023-10-23 00:24:43" format
            timeString = std::to_string(year) + "-" + (month < 10 ? "0" : "") + std::to_string(month) + "-" + (day < 10 ? "0" : "") + std::to_string(day) + " " + (hour < 10 ? "0" : "") + std::to_string(hour) + ":" + (minute < 10 ? "0" : "") + std::to_string(minute) + ":" + (second < 10 ? "0" : "") + std::to_string(second);
            std::cout << timeString << std::endl;
            timeJSON = findLineInJson(jsonName, timeString);
        }

        // Set lon to line 3 in timeJSON
        std::string longitude = timeJSON[3];
        // std::cout << longitude << std::endl;
        //  Set lat to line 2 in timeJSON
        std::string latitude = timeJSON[2];
        // std::cout << latitude << std::endl;
        //  Set az to line 4 in timeJSON
        std::string azimuth = timeJSON[4];
        // std::cout << azimuth << std::endl;
        //  Set el to line 5 in timeJSON
        std::string elevation = timeJSON[5];
        // std::cout << elevation << std::endl;

        // Remove extra spaces
        longitude.erase(std::remove_if(longitude.begin(), longitude.end(), ::isspace), longitude.end()); // Remove Spaces
        latitude.erase(std::remove_if(latitude.begin(), latitude.end(), ::isspace), latitude.end());     // Remove Spaces
        azimuth.erase(std::remove_if(azimuth.begin(), azimuth.end(), ::isspace), azimuth.end());         // Remove Spaces
        elevation.erase(std::remove_if(elevation.begin(), elevation.end(), ::isspace), elevation.end()); // Remove Spaces

        // Remove the "Longitude": part
        longitude.erase(0, 12);
        // Remove the "Latitude": part
        latitude.erase(0, 11);
        // Remove the "Azimuth": part
        azimuth.erase(0, 11);
        // Remove the "Elevation": part
        elevation.erase(0, 12);
        /*
        std::cout << longitude << std::endl;
        std::cout << latitude << std::endl;
        std::cout << azimuth << std::endl;
        std::cout << elevation << std::endl;
        */

        //Check to see if el is larger than 90* and if flip is false, if so set flip to true, rotate 180* and then start decrementing el

        if (stringToDouble(elevation) > 90 && !flipOver)
        {
            //Azimuth + 180*
            azimuth = std::to_string(stringToDouble(azimuth) + 180);
            //Elevation is 180 - elevation
            elevation = std::to_string(180 - stringToDouble(elevation));
            flipOver = true;
        }
        if (flipOver){
            //Elevation is 180 - elevation
            elevation = std::to_string(180 - stringToDouble(elevation));
            //azimuth is 180 + azimuth
            azimuth = std::to_string(180 + stringToDouble(azimuth));
        }

        // Move to position
        moveToPosition(stringToDouble(azimuth), stringToDouble(elevation));
        sleep(5);
    }
}

void breakFunction()
{
    // Used to make sure that GPIO pins drop to 0 if program is stopped
    // gpioOut(26, .1, false);
    // gpioOut(19, .1, false);
    // gpioOut(6, .1, false);
    // gpioOut(13, .1, false);
}

/*
void gpioOut(int pinNumber, int sleepTime, bool debugging)
{

    std::string gpioPath = "/sys/class/gpio/gpio" + std::to_string(pinNumber);

    std::ofstream gpioExport("/sys/class/gpio/export"); // Open file to export GPIO pin
    gpioExport << pinNumber;                            // Write GPIO number to file
    gpioExport.close();                                 // Close file

    std::ofstream gpioDir(gpioPath + "/direction"); // Open file to set GPIO pin direction
    gpioDir << "out";                               // Write "out" to file to set direction as output
    gpioDir.close();                                // Close file

    std::ofstream gpioVal(gpioPath + "/value"); // Open file to set GPIO pin value
    gpioVal << "1";                             // Write "1" to file to set value high
    gpioVal.close();                            // Close file
    if (debugging)
    {
        std::cout << "GPIO " << pinNumber << " is now high" << std::endl; // Output message
    }

    sleep(sleepTime); // Hold the GPIO pin high for specified time

    gpioVal.open(gpioPath + "/value"); // Open file to set GPIO pin value
    gpioVal << "0";                    // Write "0" to file to set value low
    gpioVal.close();                   // Close file
    if (debugging)
    {
        std::cout << "GPIO " << pinNumber << " is now low" << std::endl; // Output message
    }
}



void gpioOutmicro(int pinNumber, int sleepTime, bool debugging)
{

    std::string gpioPath = "/sys/class/gpio/gpio" + std::to_string(pinNumber);

    std::ofstream gpioExport("/sys/class/gpio/export"); // Open file to export GPIO pin
    gpioExport << pinNumber;                            // Write GPIO number to file
    gpioExport.close();                                 // Close file

    std::ofstream gpioDir(gpioPath + "/direction"); // Open file to set GPIO pin direction
    gpioDir << "out";                               // Write "out" to file to set direction as output
    gpioDir.close();                                // Close file

    std::ofstream gpioVal(gpioPath + "/value"); // Open file to set GPIO pin value
    gpioVal << "1";                             // Write "1" to file to set value high
    gpioVal.close();                            // Close file
    if (debugging)
    {
        std::cout << "GPIO " << pinNumber << " is now high" << std::endl; // Output message
    }

    usleep(sleepTime); // Hold the GPIO pin high for specified time

    gpioVal.open(gpioPath + "/value"); // Open file to set GPIO pin value
    gpioVal << "0";                    // Write "0" to file to set value low
    gpioVal.close();                   // Close file
    if (debugging)
    {
        std::cout << "GPIO " << pinNumber << " is now low" << std::endl; // Output message
    }
}
*/