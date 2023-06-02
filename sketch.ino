// Bibliotecas
#include <DHTesp.h>
#include <EspMQTTClient.h>
#include <ArduinoJson.h>

// Constantes
const int DHT_PIN = 15;
const char SSIDName[] = "Wokwi-GUEST";
const char SSIDPass[] = "";
const char BrokerURL[] = "broker.hivemq.com";
const char BrokerUserName[] = "";
const char BrokerPassword[] = "";
const char MQTTClientName[] = "SensorDHT22-02";
const int BrokerPort = 1883;
const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(3) + 20;
const char Location[] = "Madrid";

// Variáveis globais
DHTesp dhtSensor;
EspMQTTClient clienteMQTT(SSIDName, SSIDPass, BrokerURL, BrokerUserName, BrokerPassword, MQTTClientName, BrokerPort);
StaticJsonDocument<JSON_BUFFER_SIZE> jsonBuffer;
JsonObject jsonPayload = jsonBuffer.to<JsonObject>();

// Serialize the JSON object to a string
String jsonString;

void onConnectionEstablished() {}

// Setup
void setup() {
  Serial.begin(9600);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  clienteMQTT.enableDebuggingMessages();
}

// Loop
void loop() {
  clienteMQTT.loop();

  if (clienteMQTT.isMqttConnected() == 1) {
    float temp = dhtSensor.getTemperature();

    Serial.println("Temperatura: " + String(temp, 2) + "°C");
    Serial.println("---");

    jsonPayload["temp"] = temp;
    jsonPayload["source"] = MQTTClientName;
    jsonPayload["location"] = Location;

    String jsonString;
    serializeJson(jsonPayload, jsonString);
  
    clienteMQTT.publish("dht/temp", jsonString.c_str());
  }

  delay(7500);
}