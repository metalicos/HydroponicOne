#include <PubSubClient.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "RTClib.h"
#include "data.h"

#define MQTT_SERVER "192.168.1.100"
#define MEMORY_KEY (uint8_t)33
#define RELAY_LOGIC LOW

#define KEY_MEMORY_SECTOR_ADDRESS (uint8_t) 0
#define RESTART_COUNTER_MEMORY_SECTOR_ADDRESS (uint8_t) 2
#define DATA_MEMORY_SECTOR_ADDRESS (uint8_t) 6
#define TIME_MEMORY_SECTOR_ADDRESS (uint16_t) 512

int relayPin[4] = {5, 4, 0, 2};
uint64_t lastSendToServer = 0;
uint64_t lastRecheckDosators = 0;

WiFiClient espClient;
PubSubClient mqttClient(espClient);
DeviceData cdd;
RTC_Millis rtc;
