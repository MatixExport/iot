#ifndef SOCKETS_H
#define SOCKETS_H

#include <WiFi.h>

// #define ssid "MERCUSYS_TOYA"
// #define password "internet2015"

//#define ssid "arduino"
//#define password "12345678"

#define ssid "Redmi Note 7"
#define password "d47019c2770a"

class CUSTOM_SOCKET{
  private:
    WiFiClient client;

  public:
    CUSTOM_SOCKET();
    void send(char* strBuf);

};







#endif
