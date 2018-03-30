// nrf24_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_NRF24 class. RH_NRF24 class does not provide for addressing or
// reliability, so you should only use RH_NRF24  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example nrf24_client
// Tested on Uno with Sparkfun NRF25L01 module
// Tested on Anarduino Mini (http://www.anarduino.com/mini/) with RFM73 module
// Tested on Arduino Mega with Sparkfun WRL-00691 NRF25L01 module

#include <SPI.h>
#include <RH_NRF24.h>

// Singleton instance of the radio driver
RH_NRF24 nrf24;
// RH_NRF24 nrf24(8, 7); // use this to be electrically compatible with Mirf
// RH_NRF24 nrf24(8, 10);// For Leonardo, need explicit SS pin
// RH_NRF24 nrf24(8, 7); // For RFM73 on Anarduino Mini


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
  if (!nrf24.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPowerm18dBm))
    Serial.println("setRF failed");    
}

void loop()
{
  if (nrf24.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (nrf24.recv(buf, &len))
    {
//      NRF24::printBuffer("request: ", buf, len);
    //string s = ((player*)buf)->grab + " " + ((player*)buf)->touch+ " " + ((player*)buf)->vals[0]+ " " + ((player*)buf)->vals[1] + " " + ((player*)buf)->vals[2];
      String val1 = (String)(((player*)buf)->touch);
      String val2 = String(((player*)buf)->grab);
      String val3 = String(((player*)buf)->vals[0]);
      String val4 = String(((player*)buf)->vals[1]);
      String val5 = String(((player*)buf)->vals[2]);
//      Serial.println(((player*)buf)->touch);
      Serial.println(val1 + " " + val2 + " " + val3 + " " + val4 + " " + val5);
//      string val3 = 
//      Serial.println((((player*)buf)->grab).ToSring() + " " + (((player*)buf)->touch).ToSring()+ " " + (((player*)buf)->vals[0]).ToSring()+ " " + (((player*)buf)->vals[1]).ToSring() + " " + (((player*)buf)->vals[2]).ToSring());
    }
    else
    {
      //Serial.println("recv failed");
    }
  }
  Serial.println(" ");
  delay(200);
}



