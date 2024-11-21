#include "ACC.h"
#include "SOCKETS.h"
 
#define SDA_0 22
#define SCL_0 23
 
#define SDA_1 16
#define SCL_1 17

hw_timer_t *Timer0_Cfg = NULL;

ACC *acc1;
ACC *acc2;
CUSTOM_SOCKET* sockettt;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  acc1 = new ACC(0,SDA_0,SCL_0);
  acc2 = new ACC(1,SDA_1,SCL_1);
  // sockettt = new CUSTOM_SOCKET();

}


int lastTenReads1[10] = {0};
int lastTenReads2[10] = {0};
int sum1;
int sum2;
int iterator = 0;
float oppenness = 0;
int step1Time = 0;
float change = 0.0005;
bool started = false;
bool finished = false;

// void Accelerometer()
// {
//   char strBuf[100];
//   strBuf[0] = 255;
//   strBuf[1] = (char) acc1->readX();
//   strBuf[2] = 255;
// //  strBuf[2] = (char) acc1->readY();
// //  strBuf[3] = (char) acc2->readX();
// //  strBuf[4] = (char) acc2->readY();
// //  strBuf[5] = 0;
  
//   Serial.println((int) strBuf[1]);
//    sprintf(strBuf, "x,%3d,%3d,%3d,%3d,%3d,%3d;", acc1->readX(), acc1->readY(), acc1->readZ(), acc2->readX(), acc2->readY(), acc2->readZ());
// //   Serial.println(strBuf);
//   sockettt->send(strBuf);
// }

void loop()
{
  sum1 -= lastTenReads1[iterator];
  lastTenReads1[iterator] = acc1->readZ();
  sum1 += lastTenReads1[iterator];
 
  sum2 -= lastTenReads2[iterator];
  lastTenReads2[iterator] = acc2->readZ();
  sum2 += lastTenReads2[iterator];

  iterator += 1;
  iterator = iterator %10;
  Serial.print("sum1: ");
  Serial.print(sum1);

  Serial.print(" sum2: ");
  Serial.print(sum2);
  if(sum1 < 150 && sum1 > 50){
    if(sum2 < 150 && sum2 > 50){    // fully colsed
      oppenness = 0;
    }
  }
  else if(sum1 < -650 && sum2 < -650){ // fully open
    oppenness = 100;
    change = -0.0005;
  }
  else{                               //neither
    if(sum1 < 0 && sum1 > -10){ //starts oppening
      if(!started){
        Timer0_Cfg = timerBegin(10000);
        started = true;
      } 
    }
    if(sum1 < -650){            //finnished opening first gate module
      if(started){
        step1Time = timerRead(Timer0_Cfg);
        oppenness = 30;
        change = (float)30 / step1Time;     //calibrate change
        started = false;
        finished = true;
      }
    }
    if(started){
      oppenness = -(sum1+10)/21;
    }
    else if(finished){
      oppenness = timerRead(Timer0_Cfg) * change;
    }
  }



  Serial.print(" Oppenness: ");
  Serial.print(oppenness);
  Serial.print(" change: ");
  Serial.println(change, 10);
  
}
