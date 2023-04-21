#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Define the software serial pins used to communicate with the GPS module
SoftwareSerial gpsSerial(10, 11); // RX, TX

// Define a TinyGPS++ object to parse the GPS data
TinyGPSPlus gps;

void setup() {
  // Initialize serial communication with the computer
  Serial.begin(9600);
  Serial.print("Initialized");
  
  // Initialize serial communication with the GPS module
  gpsSerial.begin(9600);
}

void loop() {
  // Read data from the GPS module
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    gps.encode(c); // Parse the GPS data
  }
  
  // Check if new GPS data is available
  if (gps.location.isUpdated()) {
    // Retrieve the latitude and longitude
    double latitude = gps.location.lat();
    double longitude = gps.location.lng();
    
    // Print the latitude and longitude to the serial monitor
    Serial.print("Latitude: ");
    Serial.print(latitude, 6);
    Serial.print(", Longitude: ");
    Serial.println(longitude, 6);
  }
}
