#include "global.h"

void toDefaultWiFiSettings() {
  memory.putString("wiSSID", String("CyberDone"));
  memory.putString("wiPASS", String("accesspoint"));
  Serial.println("To Default Wifi Settings");
  ESP.restart();
}
//
// Control
//
void start(int pin) {
  digitalWrite(pin, RELAY_ON);
}

void stop(int pin) {
  digitalWrite(pin, RELAY_OFF);
}

void controlRelay(byte relayNumber) {
  if (cdd.relay[relayNumber] == RELAY_ON) {
    start(relayPin[relayNumber]);
  }
  if (cdd.relay[relayNumber] == RELAY_OFF) {
    stop(relayPin[relayNumber]);
  }
}

void click1(Button2& btn) {
  controlRelay(0);
  cdd.relay[0] = !cdd.relay[0];
}
void longClick1(Button2& btn) {
  toDefaultWiFiSettingsFlag1 = true;
}

void click2(Button2& btn) {
  controlRelay(1);
  cdd.relay[1] = !cdd.relay[1];
}

void click3(Button2& btn) {
  controlRelay(2);
  cdd.relay[2] = !cdd.relay[2];
}

void click4(Button2& btn) {
  controlRelay(3);
  cdd.relay[3] = !cdd.relay[3];
}
void longClick4(Button2& btn) {
  toDefaultWiFiSettingsFlag2 = true;
}

void setupButtons() {
  button1.setClickHandler(click1);
  button1.setLongClickHandler(longClick1);
  button2.setClickHandler(click2);
  button3.setClickHandler(click3);
  button4.setClickHandler(click4);
  button4.setLongClickHandler(longClick4);
}

void buttonloop() {
  button1.loop();
  button2.loop();
  button3.loop();
  button4.loop();
  if (toDefaultWiFiSettingsFlag1 && toDefaultWiFiSettingsFlag2) {
    toDefaultWiFiSettings();
  }
}

void delay1s() {
  for (int i = 0; i < 100; i++) {
    buttonloop();
    delay(10);
  }
}

void saveAllDataToMemory() {
  memory.putULong("reCount", cdd.restartCounter);
  memory.putString("wiSSID", String(cdd.wifiSSID));
  memory.putString("wiPASS", String(cdd.wifiPASS));
  memory.putString("timeZone", String(cdd.timeZone));
  memory.putBool("autoTime", cdd.autoTime);
  memory.putBool("schRel", cdd.scheduledRelays);

  memory.putBool("relay[0]", cdd.relay[0]);
  memory.putBool("relay[1]", cdd.relay[1]);
  memory.putBool("relay[2]", cdd.relay[2]);
  memory.putBool("relay[3]", cdd.relay[3]);
}

void readAllDataFromMemoryToRAM() {
  cdd.restartCounter = memory.getULong("reCou", 0);
  memory.getString("wiSSID", cdd.wifiSSID, 20);
  memory.getString("wiPASS", cdd.wifiPASS, 20);
  memory.getString("timeZone", cdd.timeZone, 35);
  cdd.autoTime = memory.getBool("autoTime", true);
  cdd.scheduledRelays = memory.getBool("schRel", true);

  cdd.relay[0] = memory.getBool("relay[0]", RELAY_OFF);
  cdd.relay[1] = memory.getBool("relay[1]", RELAY_OFF);
  cdd.relay[2] = memory.getBool("relay[2]", RELAY_OFF);
  cdd.relay[3] = memory.getBool("relay[3]", RELAY_OFF);
}

void setupMemory() {
  memory.begin("app-data", false);
  readAllDataFromMemoryToRAM();
  cdd.restartCounter = cdd.restartCounter + 1;
  memory.putULong("reCount", cdd.restartCounter);
}

String currentTime() {
  DateTime now = rtc.now();
  return "[" + String(now.year()) + "," + String(now.month()) + "," + String(now.day()) + "," + String(now.hour()) + "," + String(now.minute()) + "," + String(now.second()) + "]";
}

String createJSON() {
  String str = "{\n";
  str += "\"scheduledRelays\":\"" + String(cdd.scheduledRelays ? "true" : "false") + "\",\n";
  str += "\"relay1\":\"" + String(cdd.relay[0]) + "\",\n" ;
  str += "\"relay2\":\"" + String(cdd.relay[0]) + "\",\n" ;
  str += "\"relay3\":\"" + String(cdd.relay[0]) + "\",\n" ;
  str += "\"relay4\":\"" + String(cdd.relay[0]) + "\",\n" ;
  str += "\"wifiSSID\":\"" + String(cdd.wifiSSID) + "\",\n" ;
  str += "\"wifiPASS\":\"" + String(cdd.wifiPASS) + "\",\n" ;
  str += "\"microcontrollerTime\":" + currentTime() + ",\n" ;
  str += "\"timeZone\":\"" + String(cdd.timeZone) + "\",\n" ;
  str += "\"autotime\":\"" + String(cdd.autoTime ? "true" : "false") + "\",\n" ;
  str += "\"restartCounter\":\"" + String((uint32_t)cdd.restartCounter) + "\",\n" ;
  str += "\"UUID\":\""UUID"\"\n" ;
  str += "}";
  return str;
}

void subscribeEndpoints() {
  mqttClient.subscribe("cyberdone/"UUID"/relay1");
  mqttClient.subscribe("cyberdone/"UUID"/relay2");
  mqttClient.subscribe("cyberdone/"UUID"/relay3");
  mqttClient.subscribe("cyberdone/"UUID"/relay4");

  mqttClient.subscribe("cyberdone/"UUID"/updateTime");
  mqttClient.subscribe("cyberdone/"UUID"/autotime");

  mqttClient.subscribe("cyberdone/"UUID"/scheduledRelays");
  mqttClient.subscribe("cyberdone/"UUID"/readAll");
  mqttClient.subscribe("cyberdone/"UUID"/saveAll");
  mqttClient.subscribe("cyberdone/"UUID"/restart");
  mqttClient.subscribe("cyberdone/"UUID"/restartCounter");

  mqttClient.subscribe("cyberdone/"UUID"/wifiSSID");
  mqttClient.subscribe("cyberdone/"UUID"/wifiPASS");
  mqttClient.subscribe("cyberdone/"UUID"/timezone");
}

void callback(char* topic, byte* payload, unsigned int length) {
  const char *data = reinterpret_cast<const char*>(payload);

  int32_t int32_t_Value = 0;
  sscanf(data, "#%d#", &int32_t_Value);

  uint32_t uint32_t_Value = 0;
  sscanf(data, "#%ud#", &uint32_t_Value);

  int64_t int64_t_Value = 0;
  sscanf(data, "#%ld#", &int64_t_Value);

  uint64_t uint64_t_Value = 0;
  sscanf(data, "#%lud#", &uint64_t_Value);

  float floatValue = 0.0f;
  sscanf(data, "#%f#", &floatValue);

  double doubleValue = 0.0;
  sscanf(data, "#%Lf#", &doubleValue);

  if (strcmp(topic, "cyberdone/"UUID"/updateTime") == 0) {
    int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
    sscanf(data, "#%d#%d#%d#%d#%d#%d#", &year, &month, &day, &hour, &minute, &second);
    rtc.adjust(DateTime(year, month, day, hour, minute, second));
    memory.putUChar("daSec", second);
    memory.putUChar("daMin", minute);
    memory.putUChar("daHou", hour);
    memory.putUChar("daDay", day);
    memory.putUChar("daMon", month);
    memory.putUShort("daYea", year);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/wifiSSID") == 0) {
    sscanf(data, " %s ", cdd.wifiSSID);
    memory.putString("wiSSID", String(cdd.wifiSSID));
    memory.putBool("WiFiChg", true);
    Serial.println("WiFiChg");
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/wifiPASS") == 0) {
    sscanf(data, " %s ", cdd.wifiPASS);
    memory.putString("wiPASS", String(cdd.wifiPASS));
    memory.putBool("WiFiChg", true);
    Serial.println("WiFiChg");
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/timezone") == 0) {
    sscanf(data, " %s ", cdd.timeZone);
    memory.putString("timeZone", String(cdd.timeZone));
    Serial.print("timeZone"); Serial.println(cdd.timeZone);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/autotime") == 0) {
    cdd.autoTime = (bool) uint32_t_Value;
    memory.putBool("autoTime", cdd.autoTime);
    Serial.print("autoTime"); Serial.println(cdd.autoTime);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/scheduledRelays") == 0) {
    cdd.scheduledRelays = (bool) int32_t_Value;
    memory.putBool("schRel", cdd.scheduledRelays);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/restartCounter") == 0) {
    cdd.restartCounter = uint64_t_Value;
    memory.putULong("reCount", cdd.restartCounter);
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

  if (strcmp(topic, "cyberdone/"UUID"/relay1") == 0) {
    cdd.relay[0] = (bool) uint32_t_Value;
    controlRelay(0);
  }

  if (strcmp(topic, "cyberdone/"UUID"/relay2") == 0) {
    cdd.relay[1] = (bool) uint32_t_Value;
    controlRelay(1);
  }

  if (strcmp(topic, "cyberdone/"UUID"/relay3") == 0) {
    cdd.relay[2] = (bool) uint32_t_Value;
    controlRelay(2);
  }

  if (strcmp(topic, "cyberdone/"UUID"/relay4") == 0) {
    cdd.relay[3] = (bool) uint32_t_Value;
    controlRelay(3);
  }
}

void actIfConnectionFailedForALongTime() {
  int connectCounter = 0;
  while (WiFi.status() != WL_CONNECTED && connectCounter < 20) {
    connectCounter++;
    delay1s();
  }
  if (WiFi.status() != WL_CONNECTED) {
    if (memory.getBool("wasDiscon", false) == true) {
      memory.putBool("wasDiscon", false);
    } else {
      toDefaultWiFiSettings();
    }
  }
  if (memory.getBool("WiFiChg", false)) {
    memory.putBool("WiFiChg", false);
  }
}

void restartIfConnectionFailedForALongTime() {
  byte connectCounter = 0;
  while (WiFi.status() != WL_CONNECTED && connectCounter < 20) {
    connectCounter++;
    delay1s();
  }
  if (WiFi.status() != WL_CONNECTED) {
    bool wifiSettingsWereChanged = memory.getBool("WiFiChg", false);
    Serial.print("wifiSettingsWereChanged="); Serial.println(wifiSettingsWereChanged);
    memory.putBool("WiFiChg", false);
    memory.putBool("wasDiscon", !wifiSettingsWereChanged);
    ESP.restart();
  }
}

void wifiClear() {
  WiFi.disconnect(false, true);
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);

  WiFi.disconnect(true);
  delay1s();
  WiFi.softAPdisconnect(true);
  delay1s();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay1s();

  Serial.print("WIFI status = "); Serial.println(WiFi.getMode());
  WiFi.disconnect(true);
  delay1s();
  WiFi.mode(WIFI_STA);
  delay1s();
  Serial.print("WIFI status = "); Serial.println(WiFi.getMode());
}

void setupWifi() {
  Serial.print(cdd.wifiSSID); Serial.print(" "); Serial.println(cdd.wifiPASS);
  delay(10);
  wifiClear();
  WiFi.setAutoReconnect(true);
  WiFi.begin(cdd.wifiSSID, cdd.wifiPASS);
  randomSeed(micros());
  actIfConnectionFailedForALongTime();
}

void reconnect() {
  if (!mqttClient.connected()) {
    if (mqttClient.connect(UUID, MQTT_USER, MQTT_PASSWORD)) subscribeEndpoints();
  }
}

String timeJSON() {
  String str = "{\n";
  str += "\"microcontrollerTime\":" + currentTime() + ",\n" ;
  str += "\"microcontrollerTimeZone\":\"" + String(cdd.timeZone) + "\",\n" ;
  str += "\"UUID\":\""UUID"\"\n" ;
  str += "}";
  return str;
}

void setupRelays() {
  for (byte i = 0; i < 4; i++) {
    pinMode(relayPin[i], OUTPUT);
  }
}

void stopRelays() {
  for (byte i = 0; i < 4; i++) {
    stop(relayPin[i]);
  }
}

void scheduleRelaysLoop() {

}

void setupTime() {
  int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
  second = memory.getUChar("daSec", 0);
  minute = memory.getUChar("daMin", 0);
  hour = memory.getUChar("daHou", 0);
  day = memory.getUChar("daDay", 0);
  month = memory.getUChar("daMon", 0);
  year = memory.getUShort("daYea", 0);
  rtc.begin(DateTime(year, month, day, hour, minute, second));
}





void mqttLoop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!mqttClient.connected()) {
      reconnect();
      byte state = mqttClient.state();
      Serial.print("Mqtt connection state="); Serial.println(state == 0 ? "CONNECTED" : String(state));
    }
    mqttClient.loop();
    delay1s();
    mqttClient.publish("cyberdone-relay-one-4", createJSON().c_str());
    if (cdd.autoTime) {
      mqttClient.publish("time", timeJSON().c_str());
    }
  }
  else {
    restartIfConnectionFailedForALongTime();
  }
}
