/*
 * Author: Jay Pacamarra
 * Date: Septemeber 09, 2020
 * Due Date: September 10, 2020
 * 
 * Modified wireless RC Car using the NRF24L01
 * Designed for speed
 * 
 * Code: TRANSMITTER
 * 
 */
 
//Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Function prototypes
void wireless_init(void);

/****** User Config *****/
RF24 radio(7,8);  // CE, CSN
const byte address[6] = "00001";

#define pin_pot_speed     A0
#define pin_pot2_dir      A1
int motorSpeed;
int motorDir;
/****** User Config *****/

void setup() {
  wireless_init();

  pinMode(pin_pot_speed, INPUT);
  pinMode(pin_pot2_dir, INPUT) ;

}

void loop() {
  //*********************************
  //  pin_pot_speed neutral at 132  *  --> VRY
  //  pin_pot2_dir neutral at 127   *  --> VRX
  //*********************************
  
  motorSpeed = map(analogRead(pin_pot_speed),0,1023,255,0);
  motorDir = map(analogRead(pin_pot2_dir),0,1023,0,180);
  
  radio.write(&motorDir, sizeof(motorDir));
  delay(100);
}


void wireless_init(void)
{
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
