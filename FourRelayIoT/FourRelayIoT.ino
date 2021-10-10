#include "func.h"

void setup() {
  //Serial.begin(115200);
  setupMemory();
  setupTime();
  setupWifi();
  mqttClient.setServer(MQTT_SERVER, 1883);
  mqttClient.setCallback(callback);
  setupRelays();
}

void loop() {
  mqttLoop();
  if (cdd.deviceEnable == true) {
    relaysLoop();
  }
}
