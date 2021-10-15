#include <PubSubClient.h>
#include <WiFi.h>
#include "RTClib.h"
#include "data.h"
#include <Preferences.h>
#include "Button2.h";

int relayPin[4] = {25, 26, 33, 32};
uint64_t lastRecheckDosators = 0;
bool toDefaultWiFiSettingsFlag1 = false;
bool toDefaultWiFiSettingsFlag2 = false;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
DeviceData cdd;
RTC_Millis rtc;
Preferences memory;
TaskHandle_t core1, core2;

Button2 button1 =  Button2(34, INPUT_PULLUP, false, true);
Button2 button2 =  Button2(35, INPUT_PULLUP, false, true);
Button2 button3 =  Button2(36, INPUT_PULLUP, false, true);
Button2 button4 =  Button2(39, INPUT_PULLUP, false, true);
