#include "func.h"

void firstCoreLoop(void *parameter) {
  for (;;) {
    mqttLoop();
    buttonloop();
  }
}

void secondCoreLoop(void *parameter) {
  for (;;) {
    if (cdd.scheduledRelays) {
      scheduleRelaysLoop();
    } else {
      stopRelays();
    }
  }
}

void setup() {
  Serial.begin(115200);
  setupMemory();
  setupButtons();
  setupRelays();
  setupTime();

  setupWifi();
  mqttClient.setServer(MQTT_SERVER, MQTT_SERVER_PORT);
  mqttClient.setCallback(callback);

  xTaskCreatePinnedToCore(firstCoreLoop, "firstCoreLoop", 5000, NULL, 2, &core1, 0);
  delay(500);
  xTaskCreatePinnedToCore(secondCoreLoop, "secondCoreLoop", 5000, NULL, 2, &core2, 1);
}

void loop() {
}
