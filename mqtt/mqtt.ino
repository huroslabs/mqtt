/*
 * © 2025 HUROS — All Rights Reserved 
 * 
 */


#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid     = "projectiot";
const char* password = "projectiot1234";

// ===== MQTT =====
const char* mqttServer = "10.246.228.135";
const int   mqttPort   = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// ===== MQTT Callback =====
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("]");

  // Convert payload to String
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println("Payload: " + message);

  // Parse JSON
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.println("JSON parse failed");
    return;
  }

  const char* name = doc["name"];
  int status = doc["status"];

  Serial.printf("Control name: %s\n", name);
  Serial.printf("Status: %d\n", status);

  if (strcmp(name, "button1") == 0) {
    if (status == 1) {
      Serial.println("Button 1 ON");
    } else {
      Serial.println("Button 1 OFF");
    }
  }
}

// ===== WiFi Connect =====
void SetupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.println(WiFi.localIP());
}

void Reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");
    if (client.connect("ESP32_SENSOR")) {
      Serial.println("Connected");

      // Subscribe control topic
      client.subscribe("sensor/controls");
    } else {
      Serial.print("Failed rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  SetupWiFi();

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    Reconnect();
  }
  client.loop();

  // ===== Publish sensor data =====
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 5000) {
    lastSend = millis();

    StaticJsonDocument<200> doc;
    doc["temperature"] = 35.5;
    doc["tds"] = 204;
    doc["ph"] = 6.5;

    char buffer[128];
    serializeJson(doc, buffer);

    client.publish("sensor/data", buffer);
    Serial.println("Published: " + String(buffer));
  }
}
