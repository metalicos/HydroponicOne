#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "RTClib.h"
#include "data.h"
#include <Preferences.h>
#include "DS18B20.h"
#include "CyberDonePhSensor.h"
#include "CyberDoneTdsSensor.h"
#include "Cipher.h"
#include <base64.h>

uint64_t lastRecheckDispensers = 0;


PhSensorData phData = PhSensorData();
TdsSensorData tdsData = TdsSensorData();
DeviceData dd = DeviceData();
SystemData sd = SystemData();
DeviceSettings ds = DeviceSettings();

WiFiClient espClient; //WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);
RTC_Millis rtc;
Preferences memory;
TaskHandle_t core1, core2;

DS18B20 temperatureSensor(TEMPERATURE_SENSOR_PORT);
CyberDonePhSensor phSensor(PH_SENSOR_PORT);
CyberDoneTdsSensor tdsSensor(TDS_SENSOR_PORT, ADC_REFERENCE_VOLTAGE, ADC_MAX);
