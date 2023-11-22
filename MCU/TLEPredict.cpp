#include "Functions.cpp"

// https://celestrak.org/NORAD/elements/     (Where to get TLEs)
// https://www.celestrak.com/NORAD/documentation/tle-fmt.php     (TLE format)
// https://www.satflare.com/track.asp?q=25544#TOP        (Tracking the ISS to check versus prediction)
// https://www.amsat.org/articles/g3ruh/111.html         (Plan13, which was used for a lot of these calculations)

//const char *ObserverLocation = "Cstat"; // Observer name
// double myLat = 30.585770;               // Latitude
// double myLong = -96.344610;             // Longitude
// double myAlt = 386.0;                   // Altitude Above Sea Level

double satelliteLat = 0;  // Satellite latitude
double satelliteLong = 0; // Satellite longitude
double satelliteAz = 0;   // Satellite azimuth
double satelliteEl = 0;   // Satellite elevation

char acBuffer[20]; // Buffer for ASCII time


int main()
{

    //const char *tleNameChar = "ISS (ZARYA)";
    //const char *tlel1Char = "1 25544U 98067A   23108.43397694  .00019322  00000+0  34619-3 0  9991";
    //const char *tlel2Char = "2 25544  51.6395 264.6579 0006032 207.9800 203.3391 15.49946793392526";

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = std::gmtime(&time);

    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    int hour = tm->tm_hour;
    int minute = tm->tm_min;
    int second = tm->tm_sec;

    int startYear = year;
    int startMonth = month;
    int startDay = day;
    int startHour = hour;
    int startMinute = minute;
    int startSecond = second;

    std::string tleName, tlel1, tlel2;
    
    if (std::getline(std::cin, tleName) && std::getline(std::cin, tlel1) && std::getline(std::cin, tlel2))
    {
        // Print the collected lines
        std::cout << "Line 1: " << tleName << std::endl;
        std::cout << "Line 2: " << tlel1 << std::endl;
        std::cout << "Line 3: " << tlel2 << std::endl;
    }
    else
    {
        std::cerr << "Error reading lines from stdin." << std::endl;
        return 1;
    }

    tleName.erase(std::remove_if(tleName.begin(), tleName.end(), ::isspace), tleName.end()); // Remove Spaces
    //Remove Parenthesis from tleNAME
    tleName.erase(std::remove(tleName.begin(), tleName.end(), '('), tleName.end());
    tleName.erase(std::remove(tleName.begin(), tleName.end(), ')'), tleName.end());
    
    std::cout << tleName << std::endl;
    std::ofstream myfile(tleName + ".json"); // Create json files
    myfile << "{\n";
    myfile << "    \"Satellite Info\": {"
           << "\n";

    myfile << "        \"Name\": \"" << tleName << "\"\n";
    myfile << "    },\n";
    myfile << "    \"Times\": [\n";

    const char *tleNameChar = tleName.c_str();
    const char *tlel1Char = tlel1.c_str();
    const char *tlel2Char = tlel2.c_str();

    DateTime MyTime(year, month, day, hour, minute, second); // Set start time for the prediction
    Observer MyQTH(ObserverLocation, myLat, myLong, myAlt);  // Set observer coordinates
    Satellite MySAT(tleNameChar, tlel1Char, tlel2Char);
    int pointsarray[100][2];
    int numberofpoints = 100;
    MyTime.ascii(acBuffer);                      // Get time for prediction as ASCII string
    MySAT.predict(MyTime);                       // Predict ISS for specific time
    MySAT.latlon(satelliteLat, satelliteLong);   // Get the rectangular coordinates
    MySAT.elaz(MyQTH, satelliteEl, satelliteAz); // Get azimuth and elevation for MyQTH
    //MySAT.footprint(pointsarray, numberofpoints, satelliteLat, satelliteLong);
    //std::cout << pointsarray[0][1] << std::endl;
    //return 0;
    while (year == startYear && month == startMonth && day <= startDay + 2)
    {
        while (hour < 24)
        {
            while (minute < 60)
            {
                while (second < 60)
                {
                    DateTime MyTime1(year, month, day, hour, minute, second); // Set start time for the prediction
                    Observer MyQTH1(ObserverLocation, myLat, myLong, myAlt);     // Set observer coordinates
                    Satellite MySAT1(tleNameChar, tlel1Char, tlel2Char);

                    MyTime1.ascii(acBuffer);                       // Get time for prediction as ASCII string
                    MySAT1.predict(MyTime1);                       // Predict ISS for specific time
                    MySAT1.latlon(satelliteLat, satelliteLong);    // Get the rectangular coordinates
                    MySAT1.elaz(MyQTH1, satelliteEl, satelliteAz); // Get azimuth and elevation for MyQTH
                    myfile << "        {\n";
                    myfile << "            \"Time\": \"" << acBuffer << "\",\n";
                    myfile << "            \"Co-ordinates\": {\n";
                    myfile << "                \"Latitude\": " << satelliteLat << ",\n";
                    myfile << "                \"Longitude\": " << satelliteLong << ",\n";
                    myfile << "                 \"azimuth\": " << satelliteAz << ",\n";
                    myfile << "                 \"elevation\": " << satelliteEl << "\n";
                    myfile << "            }\n";
                    if (((hour == 23) & (minute == 59) & (second == 59)) & (day == startDay + 1))
                    {
                        myfile << "        }\n";
                    }
                    else
                    {
                        myfile << "        },\n";
                    }
                    second++;
                }
                second = 0;
                minute++;
            }
            minute = 0;
            hour++;
            if (hour > 23)
            {
                hour = 0;
                day++;
                if (day > 31)
                {
                    day = 1;
                    month++;
                    if (month > 12)
                    {
                        month = 1;
                        year++;
                    }
                }
            }
            if (hour == 0 && day == startDay + 2)
            {
                break;
            }
        }
        if (hour == 0 && day == startDay + 2)
        {
            break;
        }
    }

    // store times as json

    myfile << "    ]\n";
    myfile << "}\n";
    myfile.close();

    //push file to firebase using pushFiletoFirebase(std::string fileName)
    //Remove Parenthesis from tleNAME
    tleName.erase(std::remove(tleName.begin(), tleName.end(), '('), tleName.end());
    tleName.erase(std::remove(tleName.begin(), tleName.end(), ')'), tleName.end());

    pushFiletoFirebase(tleName + ".json");


    return 0;
}