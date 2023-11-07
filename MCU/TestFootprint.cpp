#include "Celestrak.h"

const char *ObserverLocation = "Cstat"; // Observer name
double myLat = 30.585770;               // Latitude
double myLong = -96.344610;             // Longitude
double myAlt = 386.0;                   // Altitude Above Sea Level

double satelliteLat = 0;  // Satellite latitude
double satelliteLong = 0; // Satellite longitude
double satelliteAz = 0;   // Satellite azimuth
double satelliteEl = 0;   // Satellite elevation

char acBuffer[20]; // Buffer for ASCII time

int main()
{
    // Code to test Satellite Footprint
    const char *tleName = "ISS (ZARYA)";
    const char *tlel1 = "1 25544U 98067A   23108.43397694  .00019322  00000+0  34619-3 0  9991";
    const char *tlel2 = "2 25544  51.6395 264.6579 0006032 207.9800 203.3391 15.49946793392526";
    int year = 2023;
    int month = 10;
    int day = 29;
    int hour = 12;
    int minute = 0;
    int second = 0;

    int numberofpoints = 100;
    int pointsarray[numberofpoints][2];

    DateTime MyTime1(year, month, day, hour, minute, second); // Set start time for the prediction
    Observer MyQTH1(ObserverLocation, myLat, myLong, myAlt);  // Set observer coordinates
    Satellite MySAT1(tleName, tlel1, tlel2);

    MyTime1.ascii(acBuffer);                       // Get time for prediction as ASCII string
    MySAT1.predict(MyTime1);                       // Predict ISS for specific time
    MySAT1.latlon(satelliteLat, satelliteLong);    // Get the rectangular coordinates
    MySAT1.elaz(MyQTH1, satelliteEl, satelliteAz); // Get azimuth and elevation for MyQTH

    std::cout << "        {\n";
    std::cout << "            \"Time\": \"" << acBuffer << "\",\n";
    std::cout << "            \"Co-ordinates\": {\n";
    std::cout << "                \"Latitude\": " << satelliteLat << ",\n";
    std::cout << "                \"Longitude\": " << satelliteLong << ",\n";
    std::cout << "                 \"azimuth\": " << satelliteAz << ",\n";
    std::cout << "                 \"elevation\": " << satelliteEl << "\n";
    std::cout << "            }\n";
}