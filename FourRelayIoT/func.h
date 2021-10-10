#include "global.h"

void openRelay(int relayPin) {
  digitalWrite(relayPin, RELAY_LOGIC);
}

void closeRelay(int relayPin) {
  digitalWrite(relayPin, RELAY_LOGIC == HIGH ? LOW : HIGH);
}

void saveAllDataToMemory() {
  EEPROM.put(DATA_MEMORY_SECTOR_ADDRESS, cdd);
}

void readAllDataFromMemoryToRAM() {
  EEPROM.get(DATA_MEMORY_SECTOR_ADDRESS, cdd);
}

void setupMemory() {
  if (EEPROM.read(KEY_MEMORY_SECTOR_ADDRESS) != MEMORY_KEY) {
    EEPROM.write(KEY_MEMORY_SECTOR_ADDRESS, MEMORY_KEY);
    saveAllDataToMemory();
  }
  readAllDataFromMemoryToRAM();
  cdd.restartCounter = EEPROM.read(RESTART_COUNTER_MEMORY_SECTOR_ADDRESS);

  cdd.restartCounter = cdd.restartCounter + 1;
  EEPROM.put(RESTART_COUNTER_MEMORY_SECTOR_ADDRESS, cdd.restartCounter);
}

void subscribeEndpoints() {
  mqttClient.subscribe("cyberdone/"UUID"/updateTime");
  mqttClient.subscribe("cyberdone/"UUID"/relay/1");
  mqttClient.subscribe("cyberdone/"UUID"/relay/2");
  mqttClient.subscribe("cyberdone/"UUID"/relay/3");
  mqttClient.subscribe("cyberdone/"UUID"/relay/4");
  mqttClient.subscribe("cyberdone/"UUID"/deviceEnable");
  mqttClient.subscribe("cyberdone/"UUID"/restartCounter");
  mqttClient.subscribe("cyberdone/"UUID"/readAll");
  mqttClient.subscribe("cyberdone/"UUID"/saveAll");
  mqttClient.subscribe("cyberdone/"UUID"/restart");
  mqttClient.subscribe("cyberdone/"UUID"/wifiSSID");
  mqttClient.subscribe("cyberdone/"UUID"/wifiPASS");
}

String currentTime() {
  DateTime now = rtc.now();
  char date[25] = "YYYY-MM-DD hh:mm:ss";
  return String(now.toString(date));
}

String createJSON() {
  String str = "{\n";
  str += "\"relayStatus\":[" + String(cdd.relay[0]) + "," + String(cdd.relay[1]) + "," + String(cdd.relay[2]) + "," + String(cdd.relay[3]) + "],\n" ;
  str += "\"deviceEnable\":\"" + String(cdd.deviceEnable) + "\",\n" ;
  str += "\"wifiSSID\":\"" + String(cdd.wifiSSID) + "\",\n" ;
  str += "\"wifiPASS\":\"" + String(cdd.wifiPASS) + "\",\n" ;
  str += "\"microcontrollerTime\":\"" + currentTime() + "\",\n" ;
  str += "\"restartCounter\":\"" + String((uint32_t)cdd.restartCounter) + "\",\n" ;
  str += "\"UUID\":\""UUID"\"\n" ;
  str += "}";
  return str;
}

void callback(char* topic, byte* payload, unsigned int length) {
  const char *data = reinterpret_cast<const char*>(payload);

  uint32_t uint32_t_Value = 0;
  sscanf(data, "#%ud#", &uint32_t_Value);

  uint64_t uint64_t_Value = 0;
  sscanf(data, "#%lud#", &uint64_t_Value);

  double doubleValue = 0.0;
  sscanf(data, "#%Lf#", &doubleValue);

  if (strcmp(topic, "cyberdone/"UUID"/updateTime") == 0) {
    int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
    sscanf(data, "#%d#%d#%d#%d#%d#%d#", &year, &month, &day, &hour, &minute, &second);
    rtc.adjust(DateTime(year, month, day, hour, minute, second));
    EEPROM.put(TIME_MEMORY_SECTOR_ADDRESS, rtc);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/deviceEnable") == 0) {
    cdd.deviceEnable = (bool) uint32_t_Value;
    EEPROM.put(DATA_MEMORY_SECTOR_ADDRESS, cdd);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/restartCounter") == 0) {
    cdd.restartCounter = uint64_t_Value;
    EEPROM.put(RESTART_COUNTER_MEMORY_SECTOR_ADDRESS, cdd.restartCounter);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/wifiSSID") == 0) {
    sscanf(data, " %s ", cdd.wifiSSID);
    EEPROM.put(DATA_MEMORY_SECTOR_ADDRESS, cdd);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/wifiPASS") == 0) {
    sscanf(data, " %s ", cdd.wifiPASS);
    EEPROM.put(DATA_MEMORY_SECTOR_ADDRESS, cdd);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/restart") == 0) {
    ESP.restart();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/saveAll") == 0) {
    saveAllDataToMemory();
  }

  if (strcmp(topic, "cyberdone/"UUID"/readAll") == 0) {
    readAllDataFromMemoryToRAM();
  }

  if (strcmp(topic, "cyberdone/"UUID"/relay/1") == 0) {
    digitalWrite(relayPin[0], uint32_t_Value);
    cdd.relay[0] = uint32_t_Value;
  }

  if (strcmp(topic, "cyberdone/"UUID"/relay/2") == 0) {
    digitalWrite(relayPin[1], uint32_t_Value);
    cdd.relay[1] = uint32_t_Value;
  }

  if (strcmp(topic, "cyberdone/"UUID"/relay/3") == 0) {
    digitalWrite(relayPin[2], uint32_t_Value);
    cdd.relay[2] = uint32_t_Value;
  }

  if (strcmp(topic, "cyberdone/"UUID"/relay/4") == 0) {
    digitalWrite(relayPin[3], uint32_t_Value);
    cdd.relay[3] = uint32_t_Value;
  }
}


void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(cdd.wifiSSID, cdd.wifiPASS);
  randomSeed(micros());
}

void reconnect() {
  if (!mqttClient.connected()) {
    if (mqttClient.connect(UUID)) subscribeEndpoints();
  }
}

String timeJSON() {
  String str = "{\n";
  str += "\"microcontrollerTime\":\"" + currentTime() + "\",\n" ;
  str += "\"UUID\":\""UUID"\"\n" ;
  str += "}";
  return str;
}

void mqttLoop() {
  for (int i = 0; i < 50; i++) {
    if (!mqttClient.connected()) {
      reconnect();
    }
    delay(9);
    mqttClient.loop();
    delay(10);
  }
  mqttClient.publish("fourRelayOne", createJSON().c_str());
  mqttClient.publish("time", timeJSON().c_str());
}

void relaysLoop() {

}

void setupRelays() {
  for (byte i = 0; i < 4; i++) {
    pinMode(relayPin[i], OUTPUT);
  }
}

void setupTime() {
  EEPROM.get(TIME_MEMORY_SECTOR_ADDRESS, rtc);
}
