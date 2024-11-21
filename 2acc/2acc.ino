#include "ACC.h"
#include "SOCKETS.h"
 
#define SDA_0 22
#define SCL_0 23
 
#define SDA_1 16
#define SCL_1 17

ACC *acc1;
ACC *acc2;
CUSTOM_SOCKET* sockettt;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  acc1 = new ACC(0,SDA_0,SCL_0);
  acc2 = new ACC(1,SDA_1,SCL_1);
  sockettt = new CUSTOM_SOCKET();
}


void Accelerometer()
{
  char strBuf[100];
  strBuf[0] = 255;
  strBuf[1] = (char) acc1->readX();
  strBuf[2] = 255;
//  strBuf[2] = (char) acc1->readY();
//  strBuf[3] = (char) acc2->readX();
//  strBuf[4] = (char) acc2->readY();
//  strBuf[5] = 0;
  
  Serial.println((int) strBuf[1]);
   sprintf(strBuf, "x,%3d,%3d,%3d,%3d,%3d,%3d;", acc1->readX(), acc1->readY(), acc1->readZ(), acc2->readX(), acc2->readY(), acc2->readZ());
//   Serial.println(strBuf);
  sockettt->send(strBuf);
}

void loop()
{
 Accelerometer();
  //delay(1000);
}
