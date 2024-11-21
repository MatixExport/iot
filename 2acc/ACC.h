#ifndef ACC_h
#define ACC_h

#include <Wire.h>
#include <Arduino.h>

#define I2C_Freq 100000

#define ACC_ADDRESS 0x0A
#define X_REGISTER 0x04
#define Y_REGISTER 0x06
#define Z_REGISTER 0x08
#define ANY_MOTION_REGISTER 0x18


class ACC {
  private:
    TwoWire I2C;
    int8_t read_from_acc(int8_t register_address);

  public:
    ACC(uint8_t bus_num, int sda, int scl);
    int8_t readX();
    int8_t readY();
    int8_t readZ();
    int8_t readAnyMotion();
};




#endif
