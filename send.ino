// nrf24_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_NRF24 class. RH_NRF24 class does not provide for addressing or
// reliability, so you should only use RH_NRF24 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example nrf24_server.
// Tested on Uno with Sparkfun NRF25L01 module
// Tested on Anarduino Mini (http://www.anarduino.com/mini/) with RFM73 module
// Tested on Arduino Mega with Sparkfun WRL-00691 NRF25L01 module

#include <SPI.h>
#include <RH_NRF24.h>
#include<Wire.h>

// Singleton instance of the radio driver
RH_NRF24 nrf24;
// RH_NRF24 nrf24(8, 7); // use this to be electrically compatible with Mirf
// RH_NRF24 nrf24(8, 10);// For Leonardo, need explicit SS pin
// RH_NRF24 nrf24(8, 7); // For RFM73 on Anarduino Mini

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int pushButton = 4;
int touchPin = 3;

struct player
{
  int vals[3];  //holds x,y,z values
  int grab;
  int touch;
};

player data;

void setup() 
{
  Serial.begin(9600);
  while (!Serial) 
    ; // wait for serial port to connect. Needed for Leonardo only
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(2))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");    
  
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  pinMode(pushButton, INPUT);
  pinMode(touchPin, INPUT);

}


void loop()
{

  int buttonState = digitalRead(pushButton);
  int touchState = digitalRead(touchPin);

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43);  // starting with register 0x43 (GYRO_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,6,true);  // request a total of 14 registers

  data.grab = buttonState;
  data.touch = touchState;
  data.vals[0]=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  data.vals[1]=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  data.vals[2]=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L) 
//
  Serial.println(data.grab);
  Serial.println(data.touch);
  Serial.println(data.vals[0]);
  Serial.println(data.vals[1]);
  Serial.println(data.vals[2]);
  Serial.println(sizeof(data));

  Serial.println("Sending to nrf24_server");
  // Send a message to nrf24_server
  //uint8_t data[] = "Hello World!";
  nrf24.send((uint8_t*)&data, sizeof(data));

  delay(200);
}

