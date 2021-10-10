#include "func.h"

void firstCoreLoop(void *parameter) {
  for (;;) {
    mqttLoop();
  }
}

void secondCoreLoop(void *parameter) {
  for (;;) {
    if (cdd.sensorsEnable) {
      sensorsLoop();
    }
    if (cdd.dosatorsEnable) {
      dosatorsLoop();
    }
  }
}

void setup() {
  Serial.begin(115200);
  setupMemory();
  setupTime();
  setupSensors();
  setupDosators();
  setupWifi();
  mqttClient.setServer(MQTT_SERVER, MQTT_SERVER_PORT);
  mqttClient.setCallback(callback);
  
  xTaskCreatePinnedToCore(firstCoreLoop, "firstCoreLoop", 5000, NULL, 2, &core1, 0);
  delay(500);
  xTaskCreatePinnedToCore(secondCoreLoop, "secondCoreLoop", 5000, NULL, 2, &core2, 1);
}

void loop() {
}
