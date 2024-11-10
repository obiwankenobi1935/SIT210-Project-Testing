#include <WiFiNINA.h>         // Include the WiFiNINA library for Arduino boards with WiFiNINA module
#include <PubSubClient.h>     // Include the PubSubClient library for MQTT
#include <DHT.h>              // Include the DHT library

// Replace these with your network and broker details
const char* ssid = "HAALAND 8082";        // Your Wi-Fi SSID
const char* password = "00000000"; // Your Wi-Fi Password
const char* mqtt_server = "broker.emqx.io"; // EMQX broker address

// DHT Sensor settings
#define DHTPIN 4             // DHT22 data pin
#define DHTTYPE DHT22        // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMessageTime = 0;
const long messageInterval = 2000; // 2 seconds interval for publishing

void setup() {
  Serial.begin(115200);
  dht.begin(); // Initialize DHT sensor
  setup_wifi();
  client.setServer(mqtt_server, 1883);  
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMessageTime > messageInterval) {
    publishDHTData(); // Publish DHT data
    lastMessageTime = now;
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  int status = WiFi.begin(ssid, password);
  
  // Wait for connection to the Wi-Fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ArduinoClient")) {
      Serial.println("connected");
      client.subscribe("SIT210/dht22");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];  
  }
  Serial.println(message);
}

void publishDHTData() {
  float humidity = dht.readHumidity();    // Read humidity
  float temperature = dht.readTemperature(); // Read temperature in Celsius

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Create JSON message to publish
  String message = "{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + "}";
  
  if (client.publish("SIT210/dht22", message.c_str())) {
    Serial.println("Message published: " + message);
  } else {
    Serial.println("Message publish failed");
  }
}
