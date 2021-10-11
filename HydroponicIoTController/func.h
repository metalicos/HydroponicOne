#include "global.h"

void startDosing(int pinA, int pinB, bool direction) {
  digitalWrite(pinA, direction == true ? 1 : 0);
  digitalWrite(pinB, direction == true ? 0 : 1);
}

void stopDosing(int pinA, int pinB) {
  digitalWrite(pinA, 0);
  digitalWrite(pinB, 0);
}

uint64_t doseTimeMs(double doseMl) {
  return lround(doseMl / cdd.mlPerMilisecond);
}

void saveAllDataToMemory() {
  memory.putString("wiSSID", String(cdd.wifiSSID));
  memory.putString("wiPASS", String(cdd.wifiPASS));
  memory.putString("timeZone", String(cdd.timeZone));
  memory.putBool("autoTime", cdd.autoTime);
  memory.putDouble("mlPerMil", cdd.mlPerMilisecond);
  memory.putDouble("reErPhUp", cdd.regulateErrorPhUp);
  memory.putDouble("reErPhDo", cdd.regulateErrorPhDown);
  memory.putDouble("reErFer", cdd.regulateErrorFertilizer);
  memory.putDouble("phUpDoMl", cdd.phUpDoseMl);
  memory.putDouble("phDoDoMl", cdd.phDownDoseMl);
  memory.putDouble("feDoMl", cdd.fertilizerDoseMl);
  memory.putULong("reDoAfMs", cdd.recheckDosatorsAfterMs);
  memory.putDouble("sePhVal", cdd.setupPhValue);
  memory.putUInt("seTdsVal", cdd.setupTdsValue);

  memory.putBool("devEn", cdd.deviceEnable);
  memory.putBool("dosEn", cdd.dosatorsEnable);
  memory.putBool("senEn", cdd.sensorsEnable);
  memory.putULong("reCount", cdd.restartCounter);
}

void readAllDataFromMemoryToRAM() {
  cdd.restartCounter = memory.getULong("reCou", 0);
  memory.getString("wiSSID", cdd.wifiSSID, 20);
  memory.getString("wiPASS", cdd.wifiPASS, 20);
  memory.getString("timeZone", cdd.timeZone, 35);
  cdd.autoTime = memory.getBool("autoTime", true);
  cdd.mlPerMilisecond = memory.getDouble("mlPerMil", 0.00106666666);
  cdd.regulateErrorPhUp = memory.getDouble("reErPhUp", 0.5);
  cdd.regulateErrorPhDown = memory.getDouble("reErPhDo", 0.5);
  cdd.regulateErrorFertilizer = memory.getDouble("reErFer", 20);
  cdd.phUpDoseMl = memory.getDouble("phUpDoMl", 2.0);
  cdd.phDownDoseMl = memory.getDouble("phDoDoMl", 2.0);
  cdd.fertilizerDoseMl = memory.getDouble("feDoMl", 2.0);
  cdd.recheckDosatorsAfterMs = memory.getULong("reDoAfMs", 10000);
  cdd.setupPhValue = memory.getDouble("sePhVal", 6.2);
  cdd.setupTdsValue = memory.getUInt("seTdsVal", 600);
  cdd.deviceEnable = memory.getBool("devEn", true);
  cdd.dosatorsEnable = memory.getBool("dosEn", true);
  cdd.sensorsEnable = memory.getBool("senEn", true);
  cdd.restartCounter = memory.getULong("reCount", 0);
}

struct PHCalibrationValue getPhStruct() {
  struct PHCalibrationValue calValue = phSensor.getCalibrationValue();
  calValue.point = memory.getChar("point", '1');
  calValue.value[0] = memory.getDouble("value[0]", 4.0);
  calValue.value[1] = memory.getDouble("value[1]", 7.0);
  calValue.adc[0] = memory.getLong("adc[0]", 1500);
  calValue.adc[1] = memory.getLong("adc[1]", 2000);
  calValue.slope = memory.getDouble("slope", 0.01);
  calValue.adcOffset = memory.getLong("adcOffset", 700);
  return calValue;
}

void savePhStruct() {
  struct PHCalibrationValue calValue = phSensor.getCalibrationValue();
  memory.putChar("point", calValue.point);
  memory.putDouble("value[0]", calValue.value[0]);
  memory.putDouble("value[1]", calValue.value[1]);
  memory.putLong("adc[0]", calValue.adc[0]);
  memory.putLong("adc[1]", calValue.adc[1]);
  memory.putDouble("slope", calValue.slope);
  memory.putLong("adcOffset", calValue.adcOffset);
}

void setupMemory() {
  memory.begin("app-data", false);
  if (memory.getUChar("first", 2) == 34) {
    memory.putUChar("first", 34);
    memory.putDouble("tdskValue", 1.0);
    memory.putUChar("daSec", 0);
    memory.putUChar("daMin", 0);
    memory.putUChar("daHou", 0);
    memory.putUChar("daDay", 0);
    memory.putUChar("daMon", 0);
    memory.putUShort("daYea", 0);
    savePhStruct();
    saveAllDataToMemory();
  }
  tdsSensor.setKvalue(memory.getDouble("tdskValue", 1.0));
  phSensor.initialize(getPhStruct());

  readAllDataFromMemoryToRAM();

  cdd.restartCounter = cdd.restartCounter + 1;
  memory.putULong("reCount", cdd.restartCounter);
}

void subscribeEndpoints() {
  mqttClient.subscribe("cyberdone/"UUID"/updateTime");
  mqttClient.subscribe("cyberdone/"UUID"/timezone");
  mqttClient.subscribe("cyberdone/"UUID"/autotime");

  mqttClient.subscribe("cyberdone/"UUID"/phValue");
  mqttClient.subscribe("cyberdone/"UUID"/temperatureValue");
  mqttClient.subscribe("cyberdone/"UUID"/ecValue");
  mqttClient.subscribe("cyberdone/"UUID"/tdsValue");

  mqttClient.subscribe("cyberdone/"UUID"/phUpStart");
  mqttClient.subscribe("cyberdone/"UUID"/phUpStop");
  mqttClient.subscribe("cyberdone/"UUID"/phDownStart");
  mqttClient.subscribe("cyberdone/"UUID"/phDownStop");
  mqttClient.subscribe("cyberdone/"UUID"/fertilizerStart");
  mqttClient.subscribe("cyberdone/"UUID"/fertilizerStop");

  mqttClient.subscribe("cyberdone/"UUID"/mlPerMilisecond");
  mqttClient.subscribe("cyberdone/"UUID"/regulateErrorPhUp");
  mqttClient.subscribe("cyberdone/"UUID"/regulateErrorPhDown");
  mqttClient.subscribe("cyberdone/"UUID"/regulateErrorFertilizer");
  mqttClient.subscribe("cyberdone/"UUID"/phUpDoseMl");
  mqttClient.subscribe("cyberdone/"UUID"/phDownDoseMl");
  mqttClient.subscribe("cyberdone/"UUID"/fertilizerDoseMl");
  mqttClient.subscribe("cyberdone/"UUID"/recheckDosatorsAfterMs");
  mqttClient.subscribe("cyberdone/"UUID"/setupPhValue");
  mqttClient.subscribe("cyberdone/"UUID"/setupTdsValue");
  mqttClient.subscribe("cyberdone/"UUID"/setupTemperatureValue");

  mqttClient.subscribe("cyberdone/"UUID"/sendDataToServerEvery");
  mqttClient.subscribe("cyberdone/"UUID"/checkSensorEvery");

  mqttClient.subscribe("cyberdone/"UUID"/deviceEnable");
  mqttClient.subscribe("cyberdone/"UUID"/dosatorsEnable");
  mqttClient.subscribe("cyberdone/"UUID"/sensorsEnable");
  mqttClient.subscribe("cyberdone/"UUID"/restartCounter");

  mqttClient.subscribe("cyberdone/"UUID"/calibratePhLow");
  mqttClient.subscribe("cyberdone/"UUID"/calibratePhHigh");
  mqttClient.subscribe("cyberdone/"UUID"/calibratePhClear");

  mqttClient.subscribe("cyberdone/"UUID"/calibrateTds");
  mqttClient.subscribe("cyberdone/"UUID"/calibrateTdsClear");

  mqttClient.subscribe("cyberdone/"UUID"/readAll");
  mqttClient.subscribe("cyberdone/"UUID"/saveAll");
  mqttClient.subscribe("cyberdone/"UUID"/restart");

  mqttClient.subscribe("cyberdone/"UUID"/wifiSSID");
  mqttClient.subscribe("cyberdone/"UUID"/wifiPASS");
}

String currentTime() {
  DateTime now = rtc.now();
  //char date[25] = "[YYYY-MM-DD hh:mm:ss";
  //return String(now.toString(date));
  return "[" + String(now.year()) + "," + String(now.month()) + "," + String(now.day()) + "," + String(now.hour()) + "," + String(now.minute()) + "," + String(now.second()) + "]";
}

String createJSON() {

  String str = "{\n";
  str += "\"phValue\":\"" + String(cdd.phValue, 2) + "\",\n";
  str += "\"temperatureValue\":\"" + String(cdd.temperatureValue, 2) + "\",\n" ;
  str += "\"ecValue\":\"" + String(cdd.ecValue, 2) + "\",\n";
  str += "\"tdsValue\":\"" + String(cdd.tdsValue) + "\",\n" ;
  str += "\"mlPerMilisecond\":\"" + String(cdd.mlPerMilisecond, 11) + "\",\n" ;
  str += "\"regulateErrorPhUp\":\"" + String(cdd.regulateErrorPhUp, 2) + "\",\n" ;
  str += "\"regulateErrorPhDown\":\"" + String(cdd.regulateErrorPhDown, 2) + "\",\n" ;
  str += "\"regulateErrorFertilizer\":\"" + String(cdd.regulateErrorFertilizer, 2) + "\",\n" ;
  str += "\"phUpDoseMl\":\"" + String(cdd.phUpDoseMl, 1) + "\",\n" ;
  str += "\"phDownDoseMl\":\"" + String(cdd.phDownDoseMl, 1) + "\",\n" ;
  str += "\"fertilizerDoseMl\":\"" + String(cdd.fertilizerDoseMl, 1) + "\",\n" ;
  str += "\"recheckDosatorsAfterMs\":\"" + String((uint32_t)cdd.recheckDosatorsAfterMs) + "\",\n" ;
  str += "\"setupPhValue\":\"" + String(cdd.setupPhValue, 2) + "\",\n" ;
  str += "\"setupTdsValue\":\"" + String(cdd.setupTdsValue) + "\",\n" ;
  str += "\"setupTemperatureValue\":\"" + String(cdd.setupTemperatureValue, 2) + "\",\n" ;

  str += "\"wifiSSID\":\"" + String(cdd.wifiSSID) + "\",\n" ;
  str += "\"wifiPASS\":\"" + String(cdd.wifiPASS) + "\",\n" ;
  
  str += "\"microcontrollerTime\":" + currentTime() + ",\n" ;
  str += "\"timeZone\":\"" + String(cdd.timeZone) + "\",\n" ;
  str += "\"autotime\":\"" + String(cdd.autoTime ? "true" : "false") + "\",\n" ;

  str += "\"deviceEnable\":\"" + String(cdd.deviceEnable) + "\",\n" ;
  str += "\"dosatorsEnable\":\"" + String(cdd.dosatorsEnable) + "\",\n" ;
  str += "\"sensorsEnable\":\"" + String(cdd.sensorsEnable) + "\",\n" ;

  str += "\"isDosatorPhUpOpen\":\"" + String(cdd.isDosatorPhUpOpen ? "true" : "false") + "\",\n" ;
  str += "\"isDosatorPhDownOpen\":\"" + String(cdd.isDosatorPhDownOpen ? "true" : "false") + "\",\n" ;
  str += "\"isDosatorTdsOpen\":\"" + String(cdd.isDosatorTdsOpen ? "true" : "false") + "\",\n" ;

  str += "\"restartCounter\":\"" + String((uint32_t)cdd.restartCounter) + "\",\n" ;
  str += "\"UUID\":\""UUID"\"\n" ;
  str += "}";
  return str;
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

  if (strcmp(topic, "cyberdone/"UUID"/phValue") == 0) {
    cdd.phValue = (float) floatValue;
    memory.putFloat("phVal", cdd.phValue);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/temperatureValue") == 0) {
    cdd.temperatureValue = (float) floatValue;
    memory.putFloat("teVal", cdd.temperatureValue);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/ecValue") == 0) {
    cdd.ecValue = (float) floatValue;
    memory.putFloat("ecVal", cdd.ecValue);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/tdsValue") == 0) {
    cdd.tdsValue = (float) floatValue;
    memory.putFloat("tdVal", cdd.tdsValue);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/deviceEnable") == 0) {
    cdd.deviceEnable = (bool) int32_t_Value;
    memory.putBool("devEn", cdd.deviceEnable);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/dosatorsEnable") == 0) {
    cdd.dosatorsEnable = (bool) int32_t_Value;
    memory.putBool("dosEn", cdd.dosatorsEnable);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/sensorsEnable") == 0) {
    cdd.sensorsEnable = (bool) int32_t_Value;
    memory.putBool("senEn", cdd.sensorsEnable);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/restartCounter") == 0) {
    cdd.restartCounter = uint64_t_Value;
    memory.putULong("reCount", cdd.restartCounter);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/mlPerMilisecond") == 0) {
    cdd.mlPerMilisecond = doubleValue;
    memory.putDouble("mlPerMil", cdd.mlPerMilisecond);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/regulateErrorPhUp") == 0) {
    cdd.regulateErrorPhUp = doubleValue;
    memory.putDouble("reErPhUp", cdd.regulateErrorPhUp);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/regulateErrorPhDown") == 0) {
    cdd.regulateErrorPhDown = doubleValue;
    memory.putDouble("reErPhDo", cdd.regulateErrorPhDown);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/regulateErrorFertilizer") == 0) {
    cdd.regulateErrorFertilizer = doubleValue;
    memory.putDouble("reErFer", cdd.regulateErrorFertilizer);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/phUpDoseMl") == 0) {
    cdd.phUpDoseMl = doubleValue;
    memory.putDouble("phUpDoMl", cdd.phUpDoseMl);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/phDownDoseMl") == 0) {
    cdd.phDownDoseMl = doubleValue;
    memory.putDouble("phDoDoMl", cdd.phDownDoseMl);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/fertilizerDoseMl") == 0) {
    cdd.fertilizerDoseMl = doubleValue;
    memory.putDouble("feDoMl", cdd.fertilizerDoseMl);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/recheckDosatorsAfterMs") == 0) {
    cdd.recheckDosatorsAfterMs = uint64_t_Value;
    memory.putULong("reDoAfMs", cdd.recheckDosatorsAfterMs);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/setupPhValue") == 0) {
    cdd.setupPhValue = doubleValue;
    memory.putDouble("sePhVal", cdd.setupPhValue);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/setupTdsValue") == 0) {
    cdd.setupTdsValue = uint32_t_Value;
    memory.putUInt("seTdsVal", cdd.setupTdsValue);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/setupTemperatureValue") == 0) {
    cdd.setupTemperatureValue = doubleValue;
    memory.putDouble("seTempVal", cdd.setupTemperatureValue);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibratePhLow") == 0) {
    phSensor.calibrationLow(doubleValue);
    savePhStruct();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibratePhHigh") == 0) {
    phSensor.calibrationHigh(doubleValue);
    savePhStruct();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibratePhClear") == 0) {
    phSensor.calibrationClear();
    savePhStruct();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibrateTds") == 0) {
    memory.putDouble("tdskValue", tdsSensor.tdsCalibration(doubleValue));
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibrateTdsClear") == 0) {
    memory.putDouble("tdskValue", tdsSensor.tdsClearCalibration());
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

  if (strcmp(topic, "cyberdone/"UUID"/phUpStart") == 0) {
    startDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B, (uint8_t) int32_t_Value);
    cdd.isDosatorPhUpOpen = (bool) int32_t_Value;
  }
  if (strcmp(topic, "cyberdone/"UUID"/phUpStop") == 0) {
    stopDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B);
    cdd.isDosatorPhUpOpen = false;
  }

  if (strcmp(topic, "cyberdone/"UUID"/phDownStart") == 0) {
    startDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B, (uint8_t) int32_t_Value);
    cdd.isDosatorPhDownOpen = (bool) int32_t_Value;
  }
  if (strcmp(topic, "cyberdone/"UUID"/phDownStop") == 0) {
    stopDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B);
    cdd.isDosatorPhDownOpen = false;
  }

  if (strcmp(topic, "cyberdone/"UUID"/fertilizerStart") == 0) {
    startDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B, (uint8_t) int32_t_Value);
    cdd.isDosatorTdsOpen = (bool) int32_t_Value;
  }
  if (strcmp(topic, "cyberdone/"UUID"/fertilizerStop") == 0) {
    stopDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B);
    cdd.isDosatorTdsOpen = false;
  }
}

void toDefaultWiFiSettings() {
  memory.putString("wiSSID", String("CyberDone"));
  memory.putString("wiPASS", String("accesspoint"));
  ESP.restart();
}

void actIfConnectionFailedForALongTime() {
  int connectCounter = 0;
  while (WiFi.status() != WL_CONNECTED && connectCounter < 20) {
    connectCounter++;
    delay(1000);
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
    delay(1000);
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
  delay(1000);
  WiFi.softAPdisconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("WIFI status = "); Serial.println(WiFi.getMode());
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(1000);
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

void mqttLoop() {
  if (WiFi.status() == WL_CONNECTED) {
    for (int i = 0; i < 50; i++) {
      if (!mqttClient.connected()) {
        reconnect();
        byte state = mqttClient.state();
        Serial.print("Mqtt connection state="); Serial.println(state == 0 ? "CONNECTED" : String(state));
      }
      delay(9);
      mqttClient.loop();
      delay(10);
    }
    mqttClient.publish("hydro-chip", createJSON().c_str());
    if (cdd.autoTime) {
      mqttClient.publish("time", timeJSON().c_str());
    }
  }
  else {
    restartIfConnectionFailedForALongTime();
  }
}

void stopAllDosators() {
  stopDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B);
  stopDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B);
  stopDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B);
  cdd.isDosatorPhUpOpen = false;
  cdd.isDosatorPhDownOpen = false;
  cdd.isDosatorTdsOpen = false;
}

void dosatorsLoop() {
  if (millis() - lastRecheckDosators >= cdd.recheckDosatorsAfterMs) {
    lastRecheckDosators = millis();
    if (cdd.phValue < (cdd.setupPhValue - (cdd.regulateErrorPhUp / 2))) {
      startDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B, true);
      cdd.isDosatorPhUpOpen = true;
      delay(doseTimeMs(cdd.phUpDoseMl));
      stopDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B);
      cdd.isDosatorPhUpOpen = false;
    }
    if (cdd.phValue > (cdd.setupPhValue + (cdd.regulateErrorPhDown / 2))) {
      startDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B, true);
      cdd.isDosatorPhDownOpen = true;
      delay(doseTimeMs(cdd.phDownDoseMl));
      stopDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B);
      cdd.isDosatorPhDownOpen = false;
    }
    if (cdd.tdsValue < (cdd.setupTdsValue - (cdd.regulateErrorFertilizer / 2))) {
      startDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B, true);
      cdd.isDosatorTdsOpen = true;
      delay(doseTimeMs(cdd.fertilizerDoseMl));
      stopDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B);
      cdd.isDosatorTdsOpen = false;
    }
  }
}

void setupDosators() {
  pinMode(PH_UP_DOSATOR_PORT_A, OUTPUT);
  pinMode(PH_UP_DOSATOR_PORT_B, OUTPUT);
  pinMode(PH_DOWN_DOSATOR_PORT_A, OUTPUT);
  pinMode(PH_DOWN_DOSATOR_PORT_B, OUTPUT);
  pinMode(FERTILIZER_DOSATOR_PORT_A, OUTPUT);
  pinMode(FERTILIZER_DOSATOR_PORT_B, OUTPUT);
}

void setupSensors() {
  temperatureSensor.setResolution(12);
  tdsSensor.begin();
}

void sensorsLoop() {
  cdd.temperatureValue = temperatureSensor.getTempC();

  tdsSensor.setTemperature(cdd.temperatureValue);
  tdsSensor.update();
  cdd.tdsValue = tdsSensor.getTdsValue();
  cdd.ecValue = tdsSensor.getEcValue();

  cdd.phValue = phSensor.singleReading().getpH();
  phSensor.temperatureCompensation(cdd.temperatureValue);
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
