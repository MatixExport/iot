#include "ACC.h"


                                             //:konstruktor pola z private w ACC.h ??
ACC::ACC(uint8_t bus_num, int sda, int scl) : I2C(bus_num){
    Serial.println("Starting ACC constructor");
    I2C.begin(sda, scl, I2C_Freq);

    I2C.beginTransmission(ACC_ADDRESS);
    if (I2C.write(0x22) == 0) Serial.println("Failed to write register address");
    if (I2C.write(0x00) == 0) Serial.println("Failed to write data to register");
    
    if (I2C.write(0x1A) == 0) Serial.println("Failed to write register address");
    if (I2C.write(0x38) == 0) Serial.println("Failed to write data to register");
    // if (I2C.write(0x20) == 0) Serial.println("Failed to write register address");
    // if (I2C.write(0x05) == 0) Serial.println("Failed to write data to register");

    if (I2C.endTransmission() == 0) {
        Serial.println("Transmission successful");
    } else {
        Serial.println("Failed to complete transmission");
    }
}


int8_t ACC::read_from_acc(int8_t register_address){
  I2C.beginTransmission(ACC_ADDRESS);
  I2C.write(register_address); 
  I2C.endTransmission();
  I2C.requestFrom(ACC_ADDRESS,1); 
  return I2C.read();  
}

int8_t ACC::readX(){
  return ACC::read_from_acc(X_REGISTER);
}
int8_t ACC::readY(){
  return ACC::read_from_acc(Y_REGISTER);
}
int8_t ACC::readZ(){
  return ACC::read_from_acc(Z_REGISTER);
}
int8_t ACC::readAnyMotion(){
  return ACC::read_from_acc(ANY_MOTION_REGISTER);
}
