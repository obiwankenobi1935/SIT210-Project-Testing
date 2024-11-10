#include <DHT.h>

// Define the pin where the DHT22 sensor is connected
#define DHTPIN 2     // DHT22 Data Pin connected to Arduino Pin D2

// Define the type of DHT sensor (DHT22)
#define DHTTYPE DHT22

// Create an instance of the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Start the Serial Monitor to display data
  Serial.begin(115200);
  
  // Initialize the DHT sensor
  dht.begin();
  
  Serial.println("DHT22 Sensor Test with Arduino Nano 33 IoT");
}

void loop() {
  // Wait for a while between readings (to avoid overloading the sensor)
  delay(2000);  // Delay of 2 seconds between reads

  // Read the temperature and humidity from the DHT22 sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Read temperature in Celsius

  // Check if the reading is valid
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    // Print the humidity and temperature to the Serial Monitor
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");
  }
}
