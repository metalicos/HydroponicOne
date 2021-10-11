#define UUID "80aeff91-bf00-4d67-a44f-479344820f5c"
struct DeviceData {
  double phValue = 5.5;
  double temperatureValue = 23.0;
  double ecValue = 0;
  uint32_t tdsValue = 600; //ppm

  double mlPerMilisecond = 0.00106666666;     //64ml -> 60sec
  double regulateErrorPhUp = 0.5;             //0.5 PH
  double regulateErrorPhDown = 0.5;           //0.5 PH
  double regulateErrorFertilizer = 20;         //5 TDS
  double phUpDoseMl = 2.0;
  double phDownDoseMl = 2.0;
  double fertilizerDoseMl = 2.0;
  uint64_t recheckDosatorsAfterMs = 10000;
  double setupPhValue = 6.2;
  uint32_t setupTdsValue = 600; //ppm
  double setupTemperatureValue = 22.0;

  bool deviceEnable = true;
  bool dosatorsEnable = true;
  bool sensorsEnable = true;

  bool isDosatorPhUpOpen = false;
  bool isDosatorPhDownOpen = false;
  bool isDosatorTdsOpen = false;
  
  bool autoTime = true;
  char timeZone[35] = {'E', 'u', 'r', 'o', 'p', 'e', '/', 'K', 'i', 'e', 'v'};

  uint64_t restartCounter = 0;
  char wifiSSID[20] = {'C', 'y', 'b', 'e', 'r', 'D', 'o', 'n', 'e'}; //CyberDone
  char wifiPASS[20] = {'a', 'c', 'c', 'e', 's', 's', 'p', 'o', 'i', 'n', 't'}; //accesspoint
};
