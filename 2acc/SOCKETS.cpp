#include "SOCKETS.h"

CUSTOM_SOCKET::CUSTOM_SOCKET(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("...");
  }

  Serial.print("WiFi connected with IP:");
  Serial.println(WiFi.localIP());

  while(!client.connect(IPAddress(192,168,43,130), 3131)){
    Serial.println("Connection to host failed");
    delay(1000);
  }
}

void CUSTOM_SOCKET::send(char *strBuf){
  client.print(strBuf);
}
