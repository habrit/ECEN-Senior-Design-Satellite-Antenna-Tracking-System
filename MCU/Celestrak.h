#ifndef CELESTRAK_H
#define CELESTRAK_H

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iterator>
#include <algorithm>
#include <vector>
#include <cctype>
#include <cmath>
#include <unistd.h> // for sleep()


static double degrees(double rad); // Radians to degrees
static double radians(double deg); // Degrees to radians

//List of constants

extern const double RE;         // World Geodetic Ellipsoid Earth ellipsoid
extern const double FL; // Flattening Ratio
extern const double RP;  // Polar Radius

extern const double GM;    // Earth's Gravitational constant km^3/s^2
extern const double J2; // 2nd Zonal coeffecient of the Earth's gravity field

extern const double YM;          // Mean Year,     days
extern const double YT;     // Tropical year, days
extern const double WW; // Earth's rotation rate, radians/whole day
extern const double WE; // Earth's rotation rate, radians/day
extern const double W0;    // Earth's rotation rate, radians/sec

// Sidereal and Solar data. Rarely needs changing. Valid to year ~2030
extern const double YG; // GHAA, Year YG, Jan 0.0
extern const double G0;
extern const double MAS0; // MA Sun and rate, deg, deg/day
extern const double MASD;
extern const double INS; // Sun's inclination
extern const double CNS;
extern const double SNS;
extern const double EQC1; // Sun's Equation of centre terms
extern const double EQC2;

extern const double AU; // Mean range to the sun, km

typedef double Vec3[3];

// List of commands
extern const std::string CONNECT;
extern const std::string DISCONNECT;
extern const std::string DECREMENT;
extern const std::string ENDAUTO;
extern const std::string ENDMANUAL;
extern const std::string FETCHSATELLITE;
extern const std::string INCREMENT;
extern const std::string GETPOSITION;
extern const std::string PARKSATELLITE;
extern const std::string SETAZIOFFSET;
extern const std::string SETELEOFFSET;
extern const std::string SETPOSITION;
extern const std::string STARTAUTO;
extern const std::string STARTMANUAL;
extern const std::string STOPSATELLITE;
extern const std::string TOGGLEMODE;

// Your code goes here




static long fnday(int y, int m, int d);                 // Function to calculate the day of the year
static void fndate(int &y, int &m, int &d, long dt);    // Function to calculate the date from the day of the year
static double getdouble(const char *c, int i0, int i1); // Function to get a double from a string
static long getlong(const char *c, int i0, int i1);     // Function to get a long from a string


class DateTime
{

public:
    long DN;   // Date
    double TN; // Time

    DateTime();
    DateTime(const DateTime &);
    DateTime(int year, int month, int day, int h, int m, int s);
    ~DateTime();

    void add(double);                                                    // Add seconds to current time
    void settime(int year, int month, int day, int h, int m, int s);     // Set time
    void gettime(int &year, int &mon, int &day, int &h, int &m, int &s); // Get time and store in variables
    void ascii(char *);                                                  // Convert to ASCII
    void roundup(double);                                                // Rounds to nearest number of seconds
};


// Location of observer
class Observer
{

public:
    const char *name;
    double LA; // Latitude
    double LO; // Longitude
    double HT; // Height above sea level

    Vec3 U, E, N, O, V; // U = local vertical direction, E = local East direction, N = local North direction, O = Celestial North Direction, V = Observer velocity direction

    Observer(const char *nm, double lat, double lon, double asl);
    ~Observer();
};


class Satellite
{

public:
    const char *name;

    Vec3 SAT, VEL; // Celestial coordinates
    Vec3 S, V;     // Geocentric coordinates

    Satellite(const char *name, const char *l1, const char *l2);
    ~Satellite(); // destructor

    void tle(const char *name, const char *l1, const char *l2);
    void predict(const DateTime &dt);
    void latlon(double &lat, double &lon);
    void elaz(const Observer &obs, double &el, double &az);

private:
    long N;      // Satellite calaog number
    long YE;     // Epoch Year    			year
    double TE;   // Epoch time    			days
    double IN;   // Inclination   			deg
    double RA;   // R.A.A.N.      			deg
    double EC;   // Eccentricity  			 -
    double WP;   // Arg perigee   			deg
    double MA;   // Mean anomaly  			deg
    double MM;   // Mean motion   			rev/d
    double M2;   // Decay Rate    			rev/d/d
    double RV;   // Orbit number  			 -
    double ALON; // Sat attitude				deg
    double ALAT; // Sat attitude				deg
    long DE;     // Epoch Fraction of day

    // These values are stored, but could be calculated on the fly during calls to predict()

    double N0, A_0, B_0; // Mean motion, semi-major axis, and semi-minor axis
    double PC;           // Period
    double QD, WD, DC;   // QD = perigee distance, WD = perigee velocity, DC = drag coefficient

    double RS; // Radius of satellite orbit
};


static long fnday(int y, int m, int d);

static void fndate(int &y, int &m, int &d, long dt);

static double getdouble(const char *c, int i0, int i1);

static long getlong(const char *c, int i0, int i1);



bool checkConnectInJSON(const std::string& filename);   //Check if Connected
bool checkDisconnect(const std::string& filename);    //Returns true if disconnected


void collectCelestrakData(); // Collects the Celestrak TLE's and outputs it to Celestrak.txt. NEEDS TO RUN DAILY

bool findSatelliteData(const std::string &satName); // Returns true if the satellite data is found, false otherwise
void exampleTLELookupCase();                        // Example case for findSatelliteData

std::string prettifyJson(std::string json); // Prettifies ugly JSON pulled from Firebase

void downloadAndPrettifyJSON(const std::string &url, const std::string &outputPath, const std::string &prettyOutputPath); // Downloads JSON from Firebase and prettifies it

std::vector<std::string> compareFiles(const std::string &file1_path, const std::string &file2_path, bool debugging); // Compares two files and returns the differences

void gpioOut(int pinNumber, int sleepTime, bool debugging);


#endif // CELESTRAK_H
