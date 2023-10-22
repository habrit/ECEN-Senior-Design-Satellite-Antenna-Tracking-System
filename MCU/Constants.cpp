#include "Celestrak.h"

//Satellite Constants
const double RE = 6378.137;         // World Geodetic Ellipsoid Earth ellipsoid
const double FL = 1.0 / 298.257224; // Flattening Ratio
const double RP = RE * (1.0 - FL);  // Polar Radius

const double GM = 3.986E5;    // Earth's Gravitational constant km^3/s^2
const double J2 = 1.08263E-3; // 2nd Zonal coeffecient of the Earth's gravity field

const double YM = 365.25;          // Mean Year,     days
const double YT = 365.2421874;     // Tropical year, days
const double WW = 2.0 * M_PI / YT; // Earth's rotation rate, radians/whole day
const double WE = 2.0 * M_PI + WW; // Earth's rotation rate, radians/day
const double W0 = WE / 86400.0;    // Earth's rotation rate, radians/sec

// Sidereal and Solar data. Rarely needs changing. Valid to year ~2030
const double YG = 2014.0; // GHAA, Year YG, Jan 0.0
const double G0 = 99.5828;
const double MAS0 = 356.4105; // MA Sun and rate, deg, deg/day
const double MASD = 0.98560028;
const double INS = radians(23.4375); // Sun's inclination
const double CNS = cos(INS);
const double SNS = sin(INS);
const double EQC1 = 0.03340; // Sun's Equation of centre terms
const double EQC2 = 0.00035;

const double AU = 149.597870700E6; // Mean range to the sun, km




//Constants
const std::string CONNECT = "Connect";
const std::string DISCONNECT = "DISCONNECT";

