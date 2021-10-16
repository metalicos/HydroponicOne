#define UUID "80aeff91-bf00-4d67-a44f-479344820f5c"

#define MQTT_SERVER "192.168.1.100"
#define MQTT_SERVER_PORT 1883
#define MQTT_USER "user"
#define MQTT_PASSWORD "Qwerty123"

#define ADC_MAX 4095
#define ADC_REFERENCE_VOLTAGE 3.3
#define PH_SENSOR_PORT 34
#define TDS_SENSOR_PORT 35
#define TEMPERATURE_SENSOR_PORT 32
#define PH_UP_DOSATOR_PORT_A 23
#define PH_UP_DOSATOR_PORT_B 22
#define PH_DOWN_DOSATOR_PORT_A 19
#define PH_DOWN_DOSATOR_PORT_B 18
#define FERTILIZER_DOSATOR_PORT_A 5
#define FERTILIZER_DOSATOR_PORT_B 17
#define SUCCESSFULLY_CONNECTED 6

struct TdsSensorData {
  double kValue = 1.0;
  int oversampling = 1000;  
};

struct DeviceData {
  double phValue = 5.5;
  double temperatureValue = 23.0;
  uint32_t tdsValue = 600; //ppm
  bool isDispenserPhUpOpen = false;
  bool isDispenserPhDownOpen = false;
  bool isDispenserTdsOpen = false;
};

struct DeviceSettings {
  double setupPhValue = 6.2;
  uint32_t setupTdsValue = 600;

  double regulateErrorPh = 0.5;
  double regulateErrorFertilizer = 20;

  double mlPerMillisecond = 0.00106666666;
  double phUpDoseMl = 2.0;
  double phDownDoseMl = 2.0;
  double fertilizerDoseMl = 2.0;

  uint64_t recheckDispensersAfterMs = 10000;
  uint64_t restartCounter = 0;
};

struct SystemData {
  bool dispensersEnable = true;
  bool sensorsEnable = true;
  bool autoTime = true;
  char timeZone[35] = {'E', 'u', 'r', 'o', 'p', 'e', '/', 'K', 'i', 'e', 'v'};
  char wifiSSID[20] = {'C', 'y', 'b', 'e', 'r', 'D', 'o', 'n', 'e'}; //CyberDone
  char wifiPASS[20] = {'a', 'c', 'c', 'e', 's', 's', 'p', 'o', 'i', 'n', 't'}; //accesspoint
};
