#include <TinyGPSPlus.h>

// Create an instance of the TinyGPS++ class
TinyGPSPlus gps;

// Initialize hardware serial communication (using default pins D0 and D1)
void setup() {
  // Start communication with the GPS and Serial Monitor
  Serial.begin(115200);  // Serial Monitor communication
  Serial1.begin(9600);   // GPS module communication (via Hardware Serial1)

  Serial.println("GPS Test with Arduino Nano 33 IoT");
}

void loop() {
  // Read data from the GPS
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());

    // If a new location is available, print it to the Serial Monitor
    if (gps.location.isUpdated()) {
      Serial.print("Latitude= ");
      Serial.print(gps.location.lat(), 6);  // Display latitude with 6 decimal places
      Serial.print(" Longitude= ");
      Serial.println(gps.location.lng(), 6);  // Display longitude with 6 decimal places
    }
  }
}
