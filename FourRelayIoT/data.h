#define UUID "11111111-bf00-4d67-a44f-479344820f5c"
struct DeviceData {
  bool relay[4] = {false, false, false, false};
  uint64_t restartCounter = 0;
  bool deviceEnable = 0;
  char wifiSSID[20] = {'A', 'B', 'C'}; //  {'C', 'Y', 'B', 'E', 'R', '_', 'D', 'O', 'N', 'E', '_', 'A', 'P'};
  char wifiPASS[20] = {'0', '6', '8', '1', '8', '4', '5', '0', '4', '0'};
};
