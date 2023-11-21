#include "Celestrak.h"
// Constants needed for calculation

// Radians to degrees

static double degrees(double rad)
{
    return (rad * 180.0 / M_PI);
}

// Degrees to radians

static double radians(double deg)
{
    return (deg * M_PI / 180.0);
}

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


// Get current time
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

DateTime::DateTime()
{

    DN = 0L; // All zeros
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
// destructor
DateTime::~DateTime()
{
}
// Add days to current time
void DateTime::add(double days)
{

    TN += days;
    DN += (long)TN;
    TN -= (long)TN;
}
// Set time
void DateTime::settime(int year, int month, int day, int h, int m, int s)
{

    DN = fnday(year, month, day);
    TN = ((double)h + (double)m / 60.0 + (double)s / 3600.0) / 24.0;
}


// Get current time in ascii format
void DateTime::ascii(char *buf)
{

    int year, mon, day;
    int h, m, s;

    gettime(year, mon, day, h, m, s);
    // 2019-05-11 00:53:13
    sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d", year, mon, day, h, m, s);
}
// Round up to nearest multiple of t
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

    double D, CL, SL, CO, SO; // Temporary variables
    double Rx;                // Radius of earth in X direction
    double Rz;                // Radius of earth in Z direction

    this->name = nm;   // Copy name
    LA = radians(lat); // Convert to radians
    LO = radians(lon); // Convert to radians
    HT = asl / 1000.0; // Convert to km

    CL = cos(LA); // Precompute some values
    SL = sin(LA); // Precompute some values
    CO = cos(LO); // Precompute some values
    SO = sin(LO); // Precompute some values

    U[0] = CL * CO; // Compute the unit vectors
    U[1] = CL * SO;
    U[2] = SL;
    E[0] = -SO; // Compute the local east, north and up
    E[1] = CO;
    E[2] = 0.0;

    N[0] = -SL * CO;
    N[1] = -SL * SO;
    N[2] = CL;

    D = sqrt(RE * RE * CL * CL + RP * RP * SL * SL); // Compute the radius of the earth in the direction of the observer
    Rx = (RE * RE) / D + HT;
    Rz = (RP * RP) / D + HT;

    O[0] = Rx * U[0]; // Compute the observer position in km
    O[1] = Rx * U[1];
    O[2] = Rz * U[2];

    V[0] = -O[1] * W0; // Compute the observer velocity in km/s
    V[1] = O[0] * W0;
    V[2] = 0.0;
}
// destructor
Observer::~Observer()
{
}

// Satellite Tle constructor
Satellite::Satellite(const char *nm, const char *l1, const char *l2)
{

    tle(nm, l1, l2);
}
// destructor
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

    TE = getdouble(l1, 20, 32);              // Get epoch (day of the year and fractional portion of the day) from tle:l1:20..31
    M2 = 2.0 * M_PI * getdouble(l1, 33, 43); // Get first time derivative of the mean motion divided by to from tle:l1:33..42

    IN = radians(getdouble(l2, 8, 16));      // Get inclination (degrees) from tle:l2:8..15
    RA = radians(getdouble(l2, 17, 25));     // Get R.A.A.N (degrees) from tle:l2:17..24
    EC = getdouble(l2, 26, 33) / 1.0E7;      // Get eccentricity from tle:l2:26..32
    WP = radians(getdouble(l2, 34, 42));     // Get argument of perigee (degrees) from tle:l2:34..41
    MA = radians(getdouble(l2, 43, 51));     // Get mean anomaly (degrees) from tle:l2:43..50
    MM = 2.0 * M_PI * getdouble(l2, 52, 63); // Get mean motion from tle:l2:52..62
    RV = getlong(l2, 63, 68);                // Get Revolution number at epoch (revolutions) from tle:l2:63..67

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
    double GHAE, GHAA; // Greenwich Hour Angle at epoch and at time T
    double T, DT, KD, KDP;
    double M, DR, RN, EA;
    double DNOM, C_EA, S_EA;
    double A, B, D;
    double AP, CW, SW;
    double RAAN;
    double CQ, SQ;
    double CI, SI;
    double CG, SG;

    Vec3 CX, CY, CZ; // Position and velocity vectors in the orbital plane

    DN = dt.DN;
    TN = dt.TN;

    GHAE = radians(G0) + ((double)(DE - fnday(YG, 1, 0)) + TE) * WE; // GHA Aries, epoch

    T = (double)(DN - DE) + (TN - TE); // Elapsed T since epoch, days
    DT = DC * T / 2.0;                 // Linear drag terms
    KD = 1.0 + 4.0 * DT;               // -"-
    KDP = 1.0 - 7.0 * DT;              // -"-

    M = MA + MM * T * (1.0 - 3.0 * DT); // Mean anomaly at YR,TN
    DR = (long)(M / (2.0 * M_PI));      // Strip out whole no of revs
    M -= DR * 2.0 * M_PI;               // M now in range 2PI
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
    A = A_0 * KD;  // Semi-major axis
    B = B_0 * KD;  // Semi-minor axis
    RS = A * DNOM; // Distance from center of earth to satellite

    // Calc satellite position & velocity in plane of ellipse
    S[0] = A * (C_EA - EC);
    S[1] = B * S_EA;

    V[0] = -A * S_EA / DNOM * N0;
    V[1] = B * C_EA / DNOM * N0;

    AP = WP + WD * T * KDP;   // Argument of perigee
    CW = cos(AP);             // cos(WP+WD*T*KDP)
    SW = sin(AP);             // sin(WP+WD*T*KDP)
    RAAN = RA + QD * T * KDP; // Ascending Node
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

    // Compute SATellite's position vector and Velocity in
    // Celestial coordinates. (Note: Sz=S[2]=0, Vz=V[2]=0)
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

    //Range Rate calculation
    drr = (V[0] - obs.V[0]) * R[0] + (V[1] - obs.V[1]) * R[1] + V[2] * R[2];

}

void Satellite::footprint(int p_aipoints[][2], int p_inumberofpoints, double &p_dsatlat, double &p_dsatlon) {

    int l_ii;
    
    double footprintRadius;
    double l_dcla, l_dsla, l_dclo, l_dslo;
    double fpRadSin, fpRadCos;
    
    double l_da, l_dx, l_dy, l_dz, l_dXfp, l_dYfp, l_dZfp;
    
        
    footprintRadius = acos(RE / RS);  // Radius of footprint circle
    fpRadSin  = sin(footprintRadius);            // Sin/Cos these to save time
    fpRadCos  = cos(footprintRadius);

    l_dcla  = cos(radians(p_dsatlat));
    l_dsla  = sin(radians(p_dsatlat));
    l_dclo  = cos(radians(p_dsatlon));
    l_dslo  = sin(radians(p_dsatlon));
    
    for ( l_ii = 0; l_ii < p_inumberofpoints ; l_ii++)               // "numberofpoints" points to the circle
    {
        l_da = 2.0 * M_PI * (double)l_ii / (double)p_inumberofpoints;  // Angle around the circle
        l_dXfp = fpRadCos;                                             // Circle of points centred on Lat=0, Lon=0
        l_dYfp = fpRadSin * sin(l_da);                                 // assuming Earth's radius = 1
        l_dZfp = fpRadSin * cos(l_da);
        
        l_dx   = l_dXfp * l_dcla - l_dZfp * l_dsla;                  // Rotate point "up" by latitude "satlat"
        l_dy   = l_dYfp;                                             // -"-
        l_dz   = l_dXfp * l_dsla + l_dZfp * l_dcla;                  // -"-
        
        l_dXfp = l_dx * l_dclo - l_dy * l_dslo;                      // Rotate point "around" through longitude "satlon"
        l_dYfp = l_dx * l_dslo + l_dy * l_dclo;                      // -"-
        l_dZfp = l_dz;                                               // -"-
        //Convert to geocentric coordinates
        l_dx   = RS * l_dXfp;
        l_dy   = RS * l_dYfp;
        l_dz   = RS * l_dZfp;
        //Add to array of points
        p_aipoints[l_ii][0] = (int)l_dx;
        p_aipoints[l_ii][1] = (int)l_dy;
        

    }
}
double Satellite::doppler(double freqMHZ, bool direction){
    double dopplershift = dopplerOffset(freqMHZ);
    if (direction){
        freqMHZ = freqMHZ - dopplershift;
    }
    else{
        freqMHZ = freqMHZ + dopplershift;
    }
    return freqMHZ;
}

double Satellite::dopplerOffset(double freqMHZ){
    double dopplershift = 0;
    dopplershift = -freqMHZ * (drr / 299792.0);
    return dopplershift;
}
