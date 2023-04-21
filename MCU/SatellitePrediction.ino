#include <Wire.h>  // include the Wire library for I2C communication
#include <LiquidCrystal_I2C.h>  // include the LiquidCrystal_I2C library for the LCD display
#include <SoftwareSerial.h> // include the SoftwareSerial library for the GPS module
#include <TinyGPS++.h>  // include the TinyGPS++ library for the GPS module


//https://celestrak.org/NORAD/elements/     (Where to get TLEs)
//https://www.celestrak.com/NORAD/documentation/tle-fmt.php     (TLE format)
//https://www.satflare.com/track.asp?q=25544#TOP        (Tracking the ISS to check versus prediction)
//https://www.amsat.org/articles/g3ruh/111.html         (Plan13, which was used for a lot of these calculations)

//Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 1);  // set the LCD address to 0x27 and the display size to 16x2

//Initialize GPS
// Define the software serial pins used to communicate with the GPS module
SoftwareSerial gpsSerial(10, 11); // RX, TX

// Define a TinyGPS++ object to parse the GPS data
TinyGPSPlus gps;




static long fnday(int y, int m, int d);             // Function to calculate the day of the year
static void fndate(int &y, int &m, int &d, long dt);    // Function to calculate the date from the day of the year
static double getdouble(const char *c, int i0, int i1); // Function to get a double from a string
static long getlong(const char *c, int i0, int i1); // Function to get a long from a string


//Constants needed for calculation

static const double RE = 6378.137;          // World Geodetic Ellipsoid Earth ellipsoid
static const double FL = 1.0 / 298.257224;  //Flattening Ratio
static const double RP = RE * (1.0 - FL);   //Polar Radius

static const double GM = 3.986E5;    // Earth's Gravitational constant km^3/s^2
static const double J2 = 1.08263E-3; // 2nd Zonal coeffecient of the Earth's gravity field

static const double YM = 365.25;        // Mean Year,     days
static const double YT = 365.2421874;   // Tropical year, days
static const double WW = 2.0 * PI / YT; // Earth's rotation rate, radians/whole day
static const double WE = 2.0 * PI + WW; // Earth's rotation rate, radians/day
static const double W0 = WE / 86400.0;  // Earth's rotation rate, radians/sec

// Sidereal and Solar data. Rarely needs changing. Valid to year ~2030
static const double YG = 2014.0;            // GHAA, Year YG, Jan 0.0
static const double G0 = 99.5828;           
static const double MAS0 = 356.4105;        // MA Sun and rate, deg, deg/day
static const double MASD = 0.98560028;      
static const double INS = radians(23.4375); // Sun's inclination
static const double CNS = cos(INS);         
static const double SNS = sin(INS);         
static const double EQC1 = 0.03340;         // Sun's Equation of centre terms
static const double EQC2 = 0.00035;         

static const double AU = 149.597870700E6; // Mean range to the sun, km


typedef double Vec3[3];


class DateTime
{

public:
    long DN;    //Date
    double TN;  //Time

    DateTime();
    DateTime(const DateTime &);
    DateTime(int year, int month, int day, int h, int m, int s);
    ~DateTime();

    void add(double);   //Add seconds to current time
    void settime(int year, int month, int day, int h, int m, int s);    //Set time
    void gettime(int &year, int &mon, int &day, int &h, int &m, int &s);    //Get time and store in variables
    void ascii(char *); //Convert to ASCII
    void roundup(double);   //Rounds to nearest number of seconds
};

//Location of observer
class Observer
{

public:
    const char *name;
    double LA;      //Latitude
    double LO;      //Longitude
    double HT;      //Height above sea level

    Vec3 U, E, N, O, V;     //U = local vertical direction, E = local East direction, N = local North direction, O = Celestial North Direction, V = Observer velocity direction

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
    ~Satellite();       //destructor

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

    double N0, A_0, B_0;        // Mean motion, semi-major axis, and semi-minor axis
    double PC;                  // Period
    double QD, WD, DC;          // QD = perigee distance, WD = perigee velocity, DC = drag coefficient

    double RS; // Radius of satellite orbit
};

// Convert day into Julian Day Number
static long fnday(int y, int m, int d)
{

    if (m < 3)
    {
        m += 12;
        y--;
    }

    return ((long)((double)y * YM) + (long)((double)(m + 1) * 30.6) + (long)(d - 428));
}

// Convert day-number to date; valid 1900 Mar 01 - 2100 Feb 28

static void fndate(int &y, int &m, int &d, long dt)
{

    dt += 428L;
    y = (int)(((double)dt - 122.1) / YM);
    dt -= (long)((double)y * YM);
    m = (int)((double)dt / 30.61);
    dt -= (long)((double)m * 30.6);
    m--;

    if (m > 12)
    {
        m -= 12;
        y++;
    }

    d = (int)dt;
}

// Cut characters from c with start i0 to i1-1, append string delimiter and convert to double

static double getdouble(const char *c, int i0, int i1)
{

    char buf[20];
    int i;

    for (i = 0; i0 + i < i1; i++)
    {
        buf[i] = c[i0 + i];
    }

    buf[i] = '\0';

    return (strtod(buf, NULL));
}

// Cut characters from c with start i0 to i1-1, append string delimiter and convert to long

static long getlong(const char *c, int i0, int i1)
{

    char buf[20];
    int i;

    for (i = 0; (i0 + i) < i1; i++)
    {
        buf[i] = c[i0 + i];
    }

    buf[i] = '\0';

    return (atol(buf));
}

DateTime::DateTime()
{

    DN = 0L; //All zeros
    TN = 0.0;
}

DateTime::DateTime(const DateTime &dt)
{

    DN = dt.DN;
    TN = dt.TN;
}

DateTime::DateTime(int year, int month, int day, int h, int m, int s)
{

    settime(year, month, day, h, m, s);
}
//destructor
DateTime::~DateTime()
{
}
//Add days to current time
void DateTime::add(double days)
{

    TN += days;
    DN += (long)TN;
    TN -= (long)TN;
}
//Set time
void DateTime::settime(int year, int month, int day, int h, int m, int s)
{

    DN = fnday(year, month, day);
    TN = ((double)h + (double)m / 60.0 + (double)s / 3600.0) / 24.0;
}
//Get current time
void DateTime::gettime(int &year, int &month, int &day, int &h, int &m, int &s)
{

    double t;

    fndate(year, month, day, DN);
    t = TN;
    t *= 24.0;
    h = (int)t;
    t -= (double)h;
    t *= 60.0;
    m = (int)t;
    t -= (double)m;
    t *= 60.0;
    s = (int)(t);
}
//Get current time in ascii format
void DateTime::ascii(char *buf)
{

    int year, mon, day;
    int h, m, s;

    gettime(year, mon, day, h, m, s);
    // 2019-05-11 00:53:13
    sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d", year, mon, day, h, m, s);
}
//Round up to nearest multiple of t
void DateTime::roundup(double t)
{

    double inc;

    inc = t - fmod(TN, t);

    TN += inc;
    DN += (long)TN;
    TN -= (long)TN;
}

// Set the observers parameters from latitude, longitude and altitude
// above sea level (m).

Observer::Observer(const char *nm, double lat, double lon, double asl)
{

    double D, CL, SL, CO, SO;   // Temporary variables
    double Rx;  // Radius of earth in X direction
    double Rz;  // Radius of earth in Z direction

    this->name = nm;    // Copy name
    LA = radians(lat);  // Convert to radians
    LO = radians(lon);  // Convert to radians
    HT = asl / 1000.0;  // Convert to km

    CL = cos(LA);   // Precompute some values
    SL = sin(LA);   // Precompute some values
    CO = cos(LO);   // Precompute some values
    SO = sin(LO);   // Precompute some values

    U[0] = CL * CO; // Compute the unit vectors
    U[1] = CL * SO;
    U[2] = SL;
    E[0] = -SO;     // Compute the local east, north and up
    E[1] = CO;
    E[2] = 0.0;

    N[0] = -SL * CO;
    N[1] = -SL * SO;
    N[2] = CL;

    D = sqrt(RE * RE * CL * CL + RP * RP * SL * SL);    // Compute the radius of the earth in the direction of the observer
    Rx = (RE * RE) / D + HT;
    Rz = (RP * RP) / D + HT;

    O[0] = Rx * U[0];   // Compute the observer position in km
    O[1] = Rx * U[1];
    O[2] = Rz * U[2];

    V[0] = -O[1] * W0;  // Compute the observer velocity in km/s
    V[1] = O[0] * W0;
    V[2] = 0.0;
}
//destructor
Observer::~Observer()
{
}

//Satellite Tle constructor
Satellite::Satellite(const char *nm, const char *l1, const char *l2)
{

    tle(nm, l1, l2);
}
//destructor
Satellite::~Satellite()
{
}

// Get satellite data from the TLE

void Satellite::tle(const char *nm, const char *l1, const char *l2)
{

    double CI;

    name = nm;

    // Direct quantities from the orbital elements

    N = getlong(l1, 2, 7);    // Get satellite catalog number from tle:l1:2..6
    YE = getlong(l1, 18, 20); // Get epoch year from tle:l1:18..19

    if (YE < 58)
        YE += 2000;
    else
        YE += 1900;

    TE = getdouble(l1, 20, 32);            // Get epoch (day of the year and fractional portion of the day) from tle:l1:20..31
    M2 = 2.0 * PI * getdouble(l1, 33, 43); // Get first time derivative of the mean motion divided by to from tle:l1:33..42

    IN = radians(getdouble(l2, 8, 16));    // Get inclination (degrees) from tle:l2:8..15
    RA = radians(getdouble(l2, 17, 25));   // Get R.A.A.N (degrees) from tle:l2:17..24
    EC = getdouble(l2, 26, 33) / 1.0E7;    // Get eccentricity from tle:l2:26..32
    WP = radians(getdouble(l2, 34, 42));   // Get argument of perigee (degrees) from tle:l2:34..41
    MA = radians(getdouble(l2, 43, 51));   // Get mean anomaly (degrees) from tle:l2:43..50
    MM = 2.0 * PI * getdouble(l2, 52, 63); // Get mean motion from tle:l2:52..62
    RV = getlong(l2, 63, 68);              // Get Revolution number at epoch (revolutions) from tle:l2:63..67

    // Derived quantities from the orbital elements

    // convert TE to DE and TE
    DE = fnday(YE, 1, 0) + (long)TE;

    TE -= (long)TE;

    N0 = MM / 86400.0;
    A_0 = pow(GM / (N0 * N0), 1.0 / 3.0);
    B_0 = A_0 * sqrt(1.0 - EC * EC);

    PC = RE * A_0 / (B_0 * B_0);
    PC = 1.5 * J2 * PC * PC * MM;

    CI = cos(IN);
    QD = -PC * CI;
    WD = PC * (5.0 * CI * CI - 1.0) / 2.0;
    DC = -2.0 * M2 / (3.0 * MM);
}

void Satellite::predict(const DateTime &dt)
{

    long DN;

    double TN;
    double GHAE, GHAA;  //Greenwich Hour Angle at epoch and at time T
    double T, DT, KD, KDP;
    double M, DR, RN, EA;
    double DNOM, C_EA, S_EA;
    double A, B, D;
    double AP, CW, SW;
    double RAAN;
    double CQ, SQ;
    double CI, SI;
    double CG, SG;

    Vec3 CX, CY, CZ;    // Position and velocity vectors in the orbital plane

    DN = dt.DN;
    TN = dt.TN;

    GHAE = radians(G0) + ((double)(DE - fnday(YG, 1, 0)) + TE) * WE; // GHA Aries, epoch

    T = (double)(DN - DE) + (TN - TE); // Elapsed T since epoch, days
    DT = DC * T / 2.0;                 // Linear drag terms
    KD = 1.0 + 4.0 * DT;               // -"-
    KDP = 1.0 - 7.0 * DT;              // -"-

    M = MA + MM * T * (1.0 - 3.0 * DT); // Mean anomaly at YR,TN
    DR = (long)(M / (2.0 * PI));        // Strip out whole no of revs
    M -= DR * 2.0 * PI;                 // M now in range 2PI
    RN = RV + DR;                       // Current Orbit number

    // Solve M = EA - EC*SIN(EA) for EA given M, by Newton's Method
    EA = M; // Initial solution

    do
    {
        C_EA = cos(EA); 
        S_EA = sin(EA);
        DNOM = 1.0 - EC * C_EA;
        D = (EA - EC * S_EA - M) / DNOM;
        EA -= D;                         
    } while (fabs(D) > 1.0E-5);

    // Distances
    A = A_0 * KD;   // Semi-major axis
    B = B_0 * KD;   // Semi-minor axis
    RS = A * DNOM;  // Distance from center of earth to satellite

    // Calc satellite position & velocity in plane of ellipse
    S[0] = A * (C_EA - EC);
    S[1] = B * S_EA;

    V[0] = -A * S_EA / DNOM * N0;
    V[1] = B * C_EA / DNOM * N0;

    AP = WP + WD * T * KDP; // Argument of perigee
    CW = cos(AP);   // cos(WP+WD*T*KDP)
    SW = sin(AP);   // sin(WP+WD*T*KDP)
    RAAN = RA + QD * T * KDP;   //Ascending Node
    CQ = cos(RAAN);
    SQ = sin(RAAN);

    // CX, CY, and CZ form a 3x3 matrix that converts between orbit coordinates, and celestial coordinates.

    // Plane -> celestial coordinate transformation, [C] = [RAAN]*[IN]*[AP]
    CI = cos(IN);
    SI = sin(IN);

    CX[0] = CW * CQ - SW * CI * SQ;
    CX[1] = -SW * CQ - CW * CI * SQ;
    CX[2] = SI * SQ;

    CY[0] = CW * SQ + SW * CI * CQ;
    CY[1] = -SW * SQ + CW * CI * CQ;
    CY[2] = -SI * CQ;

    CZ[0] = SW * SI;
    CZ[1] = CW * SI;
    CZ[2] = CI;

    // Compute SATellite's position vector and VELocity in
    // CELESTIAL coordinates. (Note: Sz=S[2]=0, Vz=V[2]=0)
    SAT[0] = S[0] * CX[0] + S[1] * CX[1];
    SAT[1] = S[0] * CY[0] + S[1] * CY[1];
    SAT[2] = S[0] * CZ[0] + S[1] * CZ[1];

    VEL[0] = V[0] * CX[0] + V[1] * CX[1];
    VEL[1] = V[0] * CY[0] + V[1] * CY[1];
    VEL[2] = V[0] * CZ[0] + V[1] * CZ[1];

    // Also express SAT and VEL in GEOCENTRIC coordinates:
    GHAA = (GHAE + WE * T);
    CG = cos(-GHAA);
    SG = sin(-GHAA);

    S[0] = SAT[0] * CG - SAT[1] * SG;
    S[1] = SAT[0] * SG + SAT[1] * CG;
    S[2] = SAT[2];

    V[0] = VEL[0] * CG - VEL[1] * SG;
    V[1] = VEL[0] * SG + VEL[1] * CG;
    V[2] = VEL[2];
}

void Satellite::latlon(double &lat, double &lon)
{

    lat = degrees(asin(S[2] / RS));
    lon = degrees(atan2(S[1], S[0]));
}

void Satellite::elaz(const Observer &obs, double &el, double &az)
{

    double r, u, e, n;

    Vec3 R; // Rangevec

    // Rangevec = Satvec - Obsvec
    R[0] = S[0] - obs.O[0];
    R[1] = S[1] - obs.O[1];
    R[2] = S[2] - obs.O[2];

    // Range magnitude
    r = sqrt(R[0] * R[0] + R[1] * R[1] + R[2] * R[2]);

    // Normalise Range vector
    R[0] /= r;
    R[1] /= r;
    R[2] /= r;

    // UP Component of unit range
    u = R[0] * obs.U[0] + R[1] * obs.U[1] + R[2] * obs.U[2];
    // EAST
    e = R[0] * obs.E[0] + R[1] * obs.E[1];
    // NORTH
    n = R[0] * obs.N[0] + R[1] * obs.N[1] + R[2] * obs.N[2];

    // Azimuth
    az = degrees(atan2(e, n));

    if (az < 0.0)
        az += 360.0;

    // Elevation
    el = degrees(asin(u));

}




const char *tleName = "ISS (ZARYA)";
const char *tlel1 = "1 25544U 98067A   23108.43397694  .00019322  00000+0  34619-3 0  9991";
const char *tlel2 = "2 25544  51.6395 264.6579 0006032 207.9800 203.3391 15.49946793392526";

const char *tleNameSES = "SES-1";
const char *tlel1SES = "1 36516U 10016A   23108.61923562 -.00000125  00000+0  00000+0 0  9994";
const char *tlel2SES = "2 36516   0.0556  63.1671 0002435 323.6330 301.5774  1.00271773 47452";


const char *ObserverLocation = "Cstat"; // Observer name
double myLat = 30.585770;      // Latitude
double myLong = -96.344610;     // Longitude
double myAlt = 386.0;          // Altitude Above Sea Level

int year = 2023; // Set start year
int month = 4;   // Set start month
int day = 18;     // Set start day
int hour =23;   // Set start hour
int minute = 46; // Set start minute
int second = 30; // Set start second

float longitudeHourMin[24][60];
float longitudeMinSec[60][60];
float latitudeHourMin[24][60];
float latitudeMinSec[60][60];





double satelliteLat = 0; // Satellite latitude
double satelliteLong = 0; // Satellite longitude
double satelliteAz = 0;  // Satellite azimuth
double satelliteEl = 0;  // Satellite elevation


char acBuffer[20]; // Buffer for ASCII time


void setup()
{
    char buf[80];   // Buffer for TLE data

    lcd.init();  // initialize the LCD display
    lcd.backlight();  // turn on the backlight
    Serial.begin(9600);
    delay(10);
    Serial.println("Started");


/*
  for (int i = 0; i < 24; i++) {
    for (int j = 0; j < 60; j++) {
        longitudeHourMin[i][j] = 0;
        latitudeHourMin[i][j] = 0;
    }
}

for (int i = 0; i < 60; i++) {
    for (int j = 0; j < 60; j++) {
        longitudeMinSec[i][j] = 0;
        latitudeMinSec[i][j] = 0;
    }
}

    DateTime MyTime(year, month, day, hour, minute, second); // Set start time for the prediction
    Observer MyQTH(ObserverLocation, myLat, myLong, myAlt);              // Set observer coordinates

    Satellite MySAT(tleName, tlel1, tlel2); // Create ISS data from TLE

    for (int h = 0; h < 24; h++) {
        for (int m = 0; m < 60; m++) {
            for (int s = 0; s < 60; s++) {
                MyTime = DateTime(year, month, day, h, m, s);
                MySAT.predict(MyTime);             // Predict ISS for specific time
                MySAT.latlon(satelliteLat, satelliteLong);    // Get the rectangular coordinates
                MySAT.elaz(MyQTH, satelliteEl, satelliteAz); // Get azimuth and elevation for MyQTH
                longitudeHourMin[h][m] = satelliteLong;
                latitudeHourMin[h][m] = satelliteLat;
                longitudeMinSec[m][s] = satelliteLong;
                latitudeMinSec[m][s] = satelliteLat;


    
  }
}

*/


    DateTime MyTime(year, month, day, hour, minute, second); // Set start time for the prediction
    Observer MyQTH(ObserverLocation, myLat, myLong, myAlt);              // Set observer coordinates
    Satellite MySAT(tleName, tlel1, tlel2);

    MyTime.ascii(acBuffer);            // Get time for prediction as ASCII string
        MySAT.predict(MyTime);             // Predict ISS for specific time
        MySAT.latlon(satelliteLat, satelliteLong);    // Get the rectangular coordinates
        MySAT.elaz(MyQTH, satelliteEl, satelliteAz); // Get azimuth and elevation for MyQTH
        Serial.print(acBuffer);
        Serial.print(" -> Lat: ");
        Serial.print(satelliteLat, 4);
        Serial.print(" Lon: ");
        Serial.print(satelliteLong, 4);
        Serial.print(" Az: ");
        Serial.print(satelliteAz, 2);
        Serial.print(" El: ");
        Serial.println(satelliteEl, 2);
        Serial.println("");
        delay(1000);
        lcd.print(satelliteLat, 4);
        lcd.print(" ");
        lcd.print(satelliteLong, 4);
        lcd.print(" ");
        delay(1000);
        lcd.clear();
        lcd.print(satelliteAz);
        lcd.print(" ");
        lcd.print(satelliteEl);
        lcd.print(" ");
        delay(1000);
        lcd.clear();

}

void loop()
{
    Serial.begin(9600);


    DateTime MyTime(year, month, day, hour, minute, second); // Set start time for the prediction
    Observer MyQTH(ObserverLocation, myLat, myLong, myAlt);              // Set observer coordinates
    Satellite MySAT(tleName, tlel1, tlel2);

    MyTime.ascii(acBuffer);            // Get time for prediction as ASCII string
        MySAT.predict(MyTime);             // Predict ISS for specific time
        MySAT.latlon(satelliteLat, satelliteLong);    // Get the rectangular coordinates
        MySAT.elaz(MyQTH, satelliteEl, satelliteAz); // Get azimuth and elevation for MyQTH
        Serial.print(acBuffer);
        Serial.print(" -> Lat: ");
        Serial.print(satelliteLat, 4);
        Serial.print(" Lon: ");
        Serial.print(satelliteLong, 4);
        Serial.print(" Az: ");
        Serial.print(satelliteAz, 2);
        Serial.print(" El: ");
        Serial.println(satelliteEl, 2);
        Serial.println("");
        delay(1000);
        lcd.print(satelliteLat, 4);
        lcd.print(" ");
        lcd.print(satelliteLong, 4);
        lcd.print(" ");
        delay(1000);
        lcd.clear();
        lcd.print(satelliteAz);
        lcd.print(" ");
        lcd.print(satelliteEl);
        lcd.print(" ");
        delay(1000);
        lcd.clear();

        if ((minute == 59) & (hour < 23)) {
            minute = 0;
            hour++;
        }
        else if ((minute == 59) & (hour == 23)) {
            minute = 0;
            hour = 0;
            day++;
            for (;;);
        }
        else {
            minute++;
        }
/*     
    Satellite MySATSES(tleNameSES, tlel1SES, tlel2SES);

    MyTime.ascii(acBuffer);            // Get time for prediction as ASCII string
        MySATSES.predict(MyTime);             // Predict ISS for specific time
        MySATSES.latlon(satelliteLat, satelliteLong);    // Get the rectangular coordinates
        MySATSES.elaz(MyQTH, satelliteEl, satelliteAz); // Get azimuth and elevation for MyQTH
        Serial.print(acBuffer);
        Serial.print(" -> Lat (SES): ");
        Serial.print(satelliteLat, 4);
        Serial.print(" Lon (SES): ");
        Serial.print(satelliteLong, 4);
        Serial.print(" Az(SES): ");
        Serial.print(satelliteAz, 2);
        Serial.print(" El(SES): ");
        Serial.println(satelliteEl, 2);
        Serial.println("");
        delay(1000);
        lcd.print(satelliteLat, 4);
        lcd.print(" ");
        lcd.print(satelliteLong, 4);
        lcd.print(" ");
        delay(1000);
        lcd.clear();
        lcd.print(satelliteAz);
        lcd.print(" ");
        lcd.print(satelliteEl);
        lcd.print(" ");
        delay(1000);
        lcd.clear();


      // Read data from the GPS module
        while (gpsSerial.available()) {
            char c = gpsSerial.read();
            gps.encode(c); // Parse the GPS data
        }
  
        // Check if new GPS data is available
        if (gps.location.isUpdated()) {
            // Retrieve the latitude and longitude
            double gpslatitude = gps.location.lat();
            double gpslongitude = gps.location.lng();
    
            // Print the latitude and longitude to the serial monitor
            Serial.print("Latitude: ");
            Serial.print(gpslatitude, 6);
            Serial.print(", Longitude: ");
            Serial.println(gpslongitude, 6);
  }
*/
}
