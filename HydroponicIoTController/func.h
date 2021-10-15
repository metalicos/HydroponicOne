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
  return lround(doseMl / ds.mlPerMilisecond);
}

void saveDeviceDataToMemory() {
  memory.putBytes("deviceData", &dd, sizeof(dd));
}

void saveSystemDataToMemory() {
  memory.putBytes("systemData", &sd, sizeof(sd));
}

void saveSettingsToMemory() {
  memory.putBytes("deviceSettings", &ds, sizeof(ds));
}

void savePhSettingsToMemory() {
  memory.putBytes("phSettings", &phData, sizeof(phData));
}

void saveTdsSettingsToMemory() {
  memory.putBytes("tdsSettings", &tdsData, sizeof(tdsData));
}


void getDeviceDataFromMemory() {
  size_t length = memory.getBytes("deviceData", NULL, NULL);
  char tmp[length];
  memory.getBytes("deviceData", tmp, length);
  memcpy(&dd, tmp, length);
}

void getSystemDataFromMemory() {
  size_t length = memory.getBytes("systemData", NULL, NULL);
  char tmp[length];
  memory.getBytes("systemData", tmp, length);
  memcpy(&sd, tmp, length);
}

void getDeviceSettingsFromMemory() {
  size_t length = memory.getBytes("deviceSettings", NULL, NULL);
  char tmp[length];
  memory.getBytes("deviceSettings", tmp, length);
  memcpy(&ds, tmp, length);
}

void getPhSettingsFromMemory() {
  size_t length = memory.getBytes("phSettings", NULL, NULL);
  char tmp[length];
  memory.getBytes("phSettings", tmp, length);
  memcpy(&phData, tmp, length);
}

void getTdsSettingsFromMemory() {
  size_t length = memory.getBytes("tdsSettings", NULL, NULL);
  char tmp[length];
  memory.getBytes("tdsSettings", tmp, length);
  memcpy(&tdsData, tmp, length);
}

DateTime getTimeFromMemory() {
  DateTime time = rtc.now();
  size_t length = memory.getBytes("time", NULL, NULL);
  char tmp[length];
  memory.getBytes("time", tmp, length);
  memcpy(&time, tmp, length);
  return time;
}

void saveAll() {
  saveDeviceDataToMemory();
  saveSystemDataToMemory();
  saveSettingsToMemory();
  savePhSettingsToMemory();
  saveTdsSettingsToMemory();
}

void getAll() {
  getDeviceDataFromMemory();
  getSystemDataFromMemory();
  getDeviceSettingsFromMemory();
  getPhSettingsFromMemory();
  getTdsSettingsFromMemory();
  rtc.adjust(getTimeFromMemory());
}


void setupMemory() {
  memory.begin("HydroData", false);
  getAll();
  ds.restartCounter += 1;
  saveSettingsToMemory();
}

void subscribeEndpoints() {
  mqttClient.subscribe("cyberdone/"UUID"/updateTime");
  mqttClient.subscribe("cyberdone/"UUID"/timezone");
  mqttClient.subscribe("cyberdone/"UUID"/autotime");

  mqttClient.subscribe("cyberdone/"UUID"/phUpPump");
  mqttClient.subscribe("cyberdone/"UUID"/phDownPump");
  mqttClient.subscribe("cyberdone/"UUID"/tdsPump");

  mqttClient.subscribe("cyberdone/"UUID"/mlPerMilisecond");
  mqttClient.subscribe("cyberdone/"UUID"/regulateErrorPh");
  mqttClient.subscribe("cyberdone/"UUID"/regulateErrorFertilizer");
  mqttClient.subscribe("cyberdone/"UUID"/phUpDoseMl");
  mqttClient.subscribe("cyberdone/"UUID"/phDownDoseMl");
  mqttClient.subscribe("cyberdone/"UUID"/fertilizerDoseMl");
  mqttClient.subscribe("cyberdone/"UUID"/recheckDosatorsAfterMs");
  mqttClient.subscribe("cyberdone/"UUID"/setupPhValue");
  mqttClient.subscribe("cyberdone/"UUID"/setupTdsValue");

  mqttClient.subscribe("cyberdone/"UUID"/dosatorsEnable");
  mqttClient.subscribe("cyberdone/"UUID"/sensorsEnable");
  mqttClient.subscribe("cyberdone/"UUID"/restartCounter");

  mqttClient.subscribe("cyberdone/"UUID"/calibratePhLow");
  mqttClient.subscribe("cyberdone/"UUID"/calibratePhHigh");
  mqttClient.subscribe("cyberdone/"UUID"/calibratePhClear");

  mqttClient.subscribe("cyberdone/"UUID"/calibrateTds");
  mqttClient.subscribe("cyberdone/"UUID"/calibrateTdsClear");

  mqttClient.subscribe("cyberdone/"UUID"/phCalibrationPoint");
  mqttClient.subscribe("cyberdone/"UUID"/phCalibrationValue1");
  mqttClient.subscribe("cyberdone/"UUID"/phCalibrationValue2");
  mqttClient.subscribe("cyberdone/"UUID"/phCalibrationAdc1");
  mqttClient.subscribe("cyberdone/"UUID"/phCalibrationAdc2");
  mqttClient.subscribe("cyberdone/"UUID"/phCalibrationSlope");
  mqttClient.subscribe("cyberdone/"UUID"/phCalibrationAdcOffset");
  mqttClient.subscribe("cyberdone/"UUID"/phOversampling");

  mqttClient.subscribe("cyberdone/"UUID"/tdsKValue");
  mqttClient.subscribe("cyberdone/"UUID"/tdsOversampling");

  mqttClient.subscribe("cyberdone/"UUID"/readAll");
  mqttClient.subscribe("cyberdone/"UUID"/saveAll");
  mqttClient.subscribe("cyberdone/"UUID"/restart");

  mqttClient.subscribe("cyberdone/"UUID"/wifiSSID");
  mqttClient.subscribe("cyberdone/"UUID"/wifiPASS");
  mqttClient.subscribe("cyberdone/"UUID"/updateToken");
}

String currentTime() {
  DateTime now = rtc.now();
  return "[" + String(now.year()) + "," + String(now.month()) + "," + String(now.day()) + "," + String(now.hour()) + "," + String(now.minute()) + "," + String(now.second()) + "]";
}

String createJSON() {
  String str = "{\n";
  str += "\"phValue\":\"" + String(dd.phValue, 2) + "\",\n";
  str += "\"temperatureValue\":\"" + String(dd.temperatureValue, 2) + "\",\n" ;
  str += "\"tdsValue\":\"" + String(dd.tdsValue) + "\",\n" ;
  str += "\"isDosatorPhUpOpen\":\"" + String(dd.isDosatorPhUpOpen ? "true" : "false") + "\",\n" ;
  str += "\"isDosatorPhDownOpen\":\"" + String(dd.isDosatorPhDownOpen ? "true" : "false") + "\",\n" ;
  str += "\"isDosatorTdsOpen\":\"" + String(dd.isDosatorTdsOpen ? "true" : "false") + "\",\n" ;

  str += "\"setupPhValue\":\"" + String(ds.setupPhValue, 2) + "\",\n" ;
  str += "\"setupTdsValue\":\"" + String(ds.setupTdsValue) + "\",\n" ;
  str += "\"regulateErrorPh\":\"" + String(ds.regulateErrorPh, 2) + "\",\n" ;
  str += "\"regulateErrorFertilizer\":\"" + String(ds.regulateErrorFertilizer, 2) + "\",\n" ;
  str += "\"mlPerMilisecond\":\"" + String(ds.mlPerMilisecond, 11) + "\",\n" ;
  str += "\"phUpDoseMl\":\"" + String(ds.phUpDoseMl, 1) + "\",\n" ;
  str += "\"phDownDoseMl\":\"" + String(ds.phDownDoseMl, 1) + "\",\n" ;
  str += "\"fertilizerDoseMl\":\"" + String(ds.fertilizerDoseMl, 1) + "\",\n" ;
  str += "\"recheckDosatorsAfterMs\":\"" + String((uint32_t)ds.recheckDosatorsAfterMs) + "\",\n" ;
  str += "\"restartCounter\":\"" + String((uint32_t)ds.restartCounter) + "\",\n" ;

  str += "\"dosatorsEnable\":\"" + String(sd.dosatorsEnable) + "\",\n" ;
  str += "\"sensorsEnable\":\"" + String(sd.sensorsEnable) + "\",\n" ;
  str += "\"autotime\":\"" + String(sd.autoTime ? "true" : "false") + "\",\n" ;
  str += "\"timeZone\":\"" + String(sd.timeZone) + "\",\n" ;
  str += "\"wifiSSID\":\"" + String(sd.wifiSSID) + "\",\n" ;
  str += "\"wifiPASS\":\"" + String(sd.wifiPASS) + "\",\n" ;

  str += "\"tdsCalibrationCoefficientValue\":\"" + String(tdsData.kValue) + "\",\n" ;
  str += "\"tdsOversampling\":\"" + String(tdsData.oversampling) + "\",\n" ;

  str += "\"phCalibrationValuePoint\":\"" + String(phData.point) + "\",\n" ;
  str += "\"phCalibrationEtalonValue1\":\"" + String(phData.value[0]) + "\",\n" ;
  str += "\"phCalibrationEtalonValue2\":\"" + String(phData.value[1]) + "\",\n" ;
  str += "\"phCalibrationAdcValue1\":\"" + String(phData.adc[0]) + "\",\n" ;
  str += "\"phCalibrationAdcValue2\":\"" + String(phData.adc[1]) + "\",\n" ;
  str += "\"phCalibrationSlope\":\"" + String(phData.slope) + "\",\n" ;
  str += "\"phCalibrationValueOffset\":\"" + String(phData.adcOffset) + "\",\n" ;
  str += "\"phOversampling\":\"" + String(phData.oversampling) + "\",\n" ;

  str += "\"microcontrollerTime\":" + currentTime() + ",\n" ;
  str += "\"uuid\":\""UUID"\"\n" ;
  str += "}";
  return str;
}

String timeJSON() {
  String str = "{\n";
  str += "\"microcontrollerTime\":" + currentTime() + ",\n" ;
  str += "\"microcontrollerTimeZone\":\"" + String(sd.timeZone) + "\",\n" ;
  str += "\"uuid\":\""UUID"\"\n" ;
  str += "}";
  return str;
}


String authJSON() {
  String str = "{\n";
  str += "\"uuid\":\""UUID"\",\n" ;
  str += "\"currentToken\":\"" + String(sd.token) + "\"\n" ;
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
    sscanf(data, " %s ", sd.wifiSSID);
    saveSystemDataToMemory();
    memory.putBool("WiFiChg", true);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/wifiPASS") == 0) {
    sscanf(data, " %s ", sd.wifiPASS);
    saveSystemDataToMemory();
    memory.putBool("WiFiChg", true);
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/updateToken") == 0) {
    sscanf(data, " %s ", sd.token);
    saveSystemDataToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/timezone") == 0) {
    sscanf(data, " %s ", sd.timeZone);
    saveSystemDataToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/autotime") == 0) {
    sd.autoTime = (bool) uint32_t_Value;
    saveSystemDataToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/dosatorsEnable") == 0) {
    sd.dosatorsEnable = (bool) int32_t_Value;
    saveSystemDataToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/sensorsEnable") == 0) {
    sd.sensorsEnable = (bool) int32_t_Value;
    saveSystemDataToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/restartCounter") == 0) {
    ds.restartCounter = uint64_t_Value;
    saveSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/mlPerMilisecond") == 0) {
    ds.mlPerMilisecond = doubleValue;
    saveSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/regulateErrorPh") == 0) {
    ds.regulateErrorPh = doubleValue;
    saveSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/regulateErrorFertilizer") == 0) {
    ds.regulateErrorFertilizer = doubleValue;
    saveSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/phUpDoseMl") == 0) {
    ds.phUpDoseMl = doubleValue;
    saveSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/phDownDoseMl") == 0) {
    ds.phDownDoseMl = doubleValue;
    saveSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/fertilizerDoseMl") == 0) {
    ds.fertilizerDoseMl = doubleValue;
    saveSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/recheckDosatorsAfterMs") == 0) {
    ds.recheckDosatorsAfterMs = uint64_t_Value;
    saveSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/setupPhValue") == 0) {
    ds.setupPhValue = doubleValue;
    saveSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/setupTdsValue") == 0) {
    ds.setupTdsValue = uint32_t_Value;
    saveSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibratePhLow") == 0) {
    phSensor.calibrationLow(doubleValue);
    phData = phSensor.getPhSensorData();
    savePhSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibratePhHigh") == 0) {
    phSensor.calibrationHigh(doubleValue);
    phData = phSensor.getPhSensorData();
    savePhSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibratePhClear") == 0) {
    phSensor.calibrationClear();
    phData = phSensor.getPhSensorData();
    savePhSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibrateTds") == 0) {
    tdsData.kValue = tdsSensor.tdsCalibration(doubleValue, dd.temperatureValue);
    saveTdsSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/calibrateTdsClear") == 0) {
    tdsData.kValue = tdsSensor.tdsClearCalibration();
    saveTdsSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/phCalibrationPoint") == 0) {
    phData.point = int32_t_Value;
    savePhSettingsToMemory();
    return;
  }
  if (strcmp(topic, "cyberdone/"UUID"/phCalibrationValue1") == 0) {
    phData.value[0] = doubleValue;
    savePhSettingsToMemory();
    return;
  }
  if (strcmp(topic, "cyberdone/"UUID"/phCalibrationValue2") == 0) {
    phData.value[1] = doubleValue;
    savePhSettingsToMemory();
    return;
  }
  if (strcmp(topic, "cyberdone/"UUID"/phCalibrationAdc1") == 0) {
    phData.adc[0] = int64_t_Value;
    savePhSettingsToMemory();
    return;
  }
  if (strcmp(topic, "cyberdone/"UUID"/phCalibrationAdc2") == 0) {
    phData.adc[1] = int64_t_Value;
    savePhSettingsToMemory();
    return;
  }
  if (strcmp(topic, "cyberdone/"UUID"/phCalibrationSlope") == 0) {
    phData.slope = doubleValue;
    savePhSettingsToMemory();
    return;
  }
  if (strcmp(topic, "cyberdone/"UUID"/phCalibrationAdcOffset") == 0) {
    phData.adcOffset = int64_t_Value;
    savePhSettingsToMemory();
    return;
  }
  if (strcmp(topic, "cyberdone/"UUID"/phOversampling") == 0) {
    phData.oversampling = int32_t_Value;
    savePhSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/tdsKValue") == 0) {
    tdsData.kValue = doubleValue;
    saveTdsSettingsToMemory();
    return;
  }
  if (strcmp(topic, "cyberdone/"UUID"/tdsOversampling") == 0) {
    tdsData.oversampling = int32_t_Value;
    saveTdsSettingsToMemory();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/restart") == 0) {
    ESP.restart();
    return;
  }

  if (strcmp(topic, "cyberdone/"UUID"/saveAll") == 0) {
    saveAll();
  }

  if (strcmp(topic, "cyberdone/"UUID"/readAll") == 0) {
    getAll();
  }

  if (strcmp(topic, "cyberdone/"UUID"/phUpPump") == 0) {
    if (int32_t_Value == 0) {
      startDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B, false);
      dd.isDosatorPhUpOpen = true;
    }
    if (int32_t_Value == 1) {
      startDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B, true);
      dd.isDosatorPhUpOpen = true;
    }
    if (int32_t_Value == 2) {
      stopDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B);
      dd.isDosatorPhUpOpen = false;
    }
  }

  if (strcmp(topic, "cyberdone/"UUID"/phDownPump") == 0) {
    if (int32_t_Value == 0) {
      startDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B, false);
      dd.isDosatorPhDownOpen = true;
    }
    if (int32_t_Value == 1) {
      startDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B, true);
      dd.isDosatorPhDownOpen = true;
    }
    if (int32_t_Value == 2) {
      stopDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B);
      dd.isDosatorPhDownOpen = false;
    }
  }

  if (strcmp(topic, "cyberdone/"UUID"/tdsPump") == 0) {
    if (int32_t_Value == 0) {
      startDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B, false);
      dd.isDosatorTdsOpen = true;
    }
    if (int32_t_Value == 1) {
      startDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B, true);
      dd.isDosatorTdsOpen = true;
    }
    if (int32_t_Value == 2) {
      stopDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B);
      dd.isDosatorTdsOpen = false;
    }
  }
}

void toDefaultWiFiSettings() {
  char wifiSSID[20] = {"Cyberdone"};
  char wifiPASS[20] = {"accesspoint"};
  strncpy(sd.wifiSSID, wifiSSID, 20);
  strncpy(sd.wifiPASS, wifiPASS, 20);
  saveSystemDataToMemory();
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
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.print("WIFI status = "); Serial.println(WiFi.getMode());
}

void setupWifi() {
  Serial.print(sd.wifiSSID); Serial.print(" "); Serial.println(sd.wifiPASS);
  delay(10);
  wifiClear();
  WiFi.setAutoReconnect(true);
  WiFi.begin(sd.wifiSSID, sd.wifiPASS);
  randomSeed(micros());
  actIfConnectionFailedForALongTime();
}

void reconnect() {
  if (!mqttClient.connected()) {
    if (mqttClient.connect(UUID, MQTT_USER, MQTT_PASSWORD)) subscribeEndpoints();
  }
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
    mqttClient.publish("device-microservice/auth", authJSON().c_str());
    mqttClient.publish("device-microservice/hydroponic-v1/settings", createJSON().c_str());
    if (sd.autoTime) {
      mqttClient.publish("device-microservice/autotime", timeJSON().c_str());
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
  dd.isDosatorPhUpOpen = false;
  dd.isDosatorPhDownOpen = false;
  dd.isDosatorTdsOpen = false;
}

void dosatorsLoop() {
  if (millis() - lastRecheckDosators >= ds.recheckDosatorsAfterMs) {
    lastRecheckDosators = millis();
    if (dd.phValue < (ds.setupPhValue - (ds.regulateErrorPh / 2))) {
      startDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B, true);
      dd.isDosatorPhUpOpen = true;
      delay(doseTimeMs(ds.phUpDoseMl));
      stopDosing(PH_UP_DOSATOR_PORT_A, PH_UP_DOSATOR_PORT_B);
      dd.isDosatorPhUpOpen = false;
    }
    if (dd.phValue > (ds.setupPhValue + (ds.regulateErrorPh / 2))) {
      startDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B, true);
      dd.isDosatorPhDownOpen = true;
      delay(doseTimeMs(ds.phDownDoseMl));
      stopDosing(PH_DOWN_DOSATOR_PORT_A, PH_DOWN_DOSATOR_PORT_B);
      dd.isDosatorPhDownOpen = false;
    }
    if (dd.tdsValue < (ds.setupTdsValue - (ds.regulateErrorFertilizer / 2))) {
      startDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B, true);
      dd.isDosatorTdsOpen = true;
      delay(doseTimeMs(ds.fertilizerDoseMl));
      stopDosing(FERTILIZER_DOSATOR_PORT_A, FERTILIZER_DOSATOR_PORT_B);
      dd.isDosatorTdsOpen = false;
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
  tdsSensor.initialize(tdsData.oversampling, tdsData.kValue);
  phSensor.initialize(phData);
}

void sensorsLoop() {
  dd.temperatureValue = temperatureSensor.getTempC();
  tdsSensor.update(dd.temperatureValue);
  dd.tdsValue = tdsSensor.getTdsValue();
  dd.phValue = phSensor.getPH(dd.temperatureValue);
  phData = phSensor.getPhSensorData();
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
