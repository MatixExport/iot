#include "ACC.h"
#include "SOCKETS.h"
 
#define SDA_0 22
#define SCL_0 23
 
#define SDA_1 16
#define SCL_1 17

#define fullyOpen = -600
#define fullyClosed = 50


hw_timer_t *Timer0_Cfg = NULL;

enum State {
  CLOSED,
  OPENING1,
  OPENING2,
  OPENING3,
  OPEN,
  CLOSING1,
  CLOSING2,
  CLOSING3,
  ERROR
};
State currentState = CLOSED;

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
Timer0_Cfg = timerBegin(10000);
}


int lastTenReads1[10] = {0};
int lastTenReads2[10] = {0};
int sum1;
int sum2;
int iterator = 0;
float openness = 0;
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

  switch (currentState) {
    case CLOSED:                          
      if(sum1 < -10){ //starts oppening
          currentState = OPENING1;
          timerRestart(Timer0_Cfg);
      }
      break;
    case OPENING1:
      openness = -(sum1+10)/21;
      if(sum1 < -600){            //finnished opening first gate module
          currentState = OPENING2;
          step1Time = timerRead(Timer0_Cfg);
          change = (float)30 / step1Time;     //calibrate change
      }
      break;
    case OPENING2:
      openness = timerRead(Timer0_Cfg) * change;
      if(sum2 < 0 && sum2 > -10){
        currentState = OPENING3;
      }
      else if(openness >= 80){
        currentState = ERROR;
      }
      break;
    case OPENING3:
      openness = -(sum2+10)/21 + 70;
      if(openness > 100){
        currentState = OPEN;
      }
      break;
    case ERROR:
      Serial.println("error");
      if(sum1 < 150 && sum1 > 50){
        if(sum2 < 150 && sum2 > 50){    // fully colsed
          currentState = CLOSED;
          openness = 0;
        }
      }
      if(sum1 < -600 && sum2 < -600){ // fully open
        currentState = OPEN;
        openness = 100;
      }
      break;
    case OPEN:
      if(sum2 > -600){ //starts closing
          currentState = CLOSING1;
          timerRestart(Timer0_Cfg);
        }
      break;

    case CLOSING1:
      openness = -(sum2+10)/21 + 70;
      if(openness < 70){
        currentState = CLOSING2;
        step1Time = timerRead(Timer0_Cfg);
        change = (float)30 / step1Time;
      }
      break;
        
    case CLOSING2:
      openness = 100 - timerRead(Timer0_Cfg) * change;
      if(sum1 > -600){
        currentState = CLOSING3;
      }
      else if(openness <= 30){
        currentState = ERROR;
      }
      break;
    case CLOSING3:
      openness = - (sum1+10)/21;
      if(openness <= 0){
        currentState = CLOSED;
      }
      break;
  }
  if(currentState != ERROR){
    Serial.print(currentState);
    Serial.print(" openness: ");
    Serial.println(openness);
  }  
}
