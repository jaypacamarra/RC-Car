/*
   Author: Jay Pacamarra
   Date: Septemeber 09, 2020
   Due Date: September 10, 2020

   Modified wireless RC Car using the NRF24L01
   Designed for speed

   Code: TRANSMITTER

*/

//Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Function prototypes
void wireless_init(void);

/****** User Config *****/
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

#define pin_pot_speed     A0
#define pin_pot2_dir      A1

struct Data_Package {
  int motorSpeed;
  int motorDir;
};

Data_Package data; //Create a variable with the above structure
/****** User Config *****/

void setup() {
  wireless_init();

  pinMode(pin_pot_speed, INPUT);
  pinMode(pin_pot2_dir, INPUT) ;

  //Initialize data
  data.motorSpeed = 132;
  data.motorDir = 90;

}

void loop() {
  //*********************************
  //  pin_pot_speed neutral at 132  *  --> VRY
  //  pin_pot2_dir neutral at 127   *  --> VRX
  //*********************************

  data.motorSpeed = map(analogRead(pin_pot_speed), 0, 1023, 255, 0);
  data.motorDir = map(analogRead(pin_pot2_dir), 0, 1023, 0, 180);

  radio.write(&data, sizeof(Data_Package));
  delay(100);
}


void wireless_init(void)
{
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}
