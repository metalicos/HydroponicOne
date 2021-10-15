#define UUID "bd887440-7e54-4e89-9b4e-72cd4c296592"
#define RELAY_ON 0
#define RELAY_OFF 1
#define MQTT_SERVER "192.168.1.100"
#define MQTT_SERVER_PORT 1883
#define MQTT_USER "user"
#define MQTT_PASSWORD "Qwerty123"
/*
class Timer {
  public:
    Timer(long unixtime, bool value) {
      this->unixtime = unixtime;
      this->value = value;
    }
    long unixtime;
    bool value;
};
Timer timer[4][8] = {
  {Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0)},
  {Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0)},
  {Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0)},
  {Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0), Timer(0, 0)}
};
*/
struct DeviceData {
  //uint64_t recheckRelaysAfterMs = 10000;
  bool scheduledRelays = true;
  bool relay[4] = {RELAY_OFF, RELAY_OFF, RELAY_OFF, RELAY_OFF};

  bool autoTime = true;
  char timeZone[35] = {'E', 'u', 'r', 'o', 'p', 'e', '/', 'K', 'i', 'e', 'v'};

  uint64_t restartCounter = 0;
  char wifiSSID[20] = {'C', 'y', 'b', 'e', 'r', 'D', 'o', 'n', 'e'}; //CyberDone
  char wifiPASS[20] = {'a', 'c', 'c', 'e', 's', 's', 'p', 'o', 'i', 'n', 't'}; //accesspoint
};
