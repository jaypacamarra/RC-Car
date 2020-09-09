/*
 * Author: Jay Pacamarra
 * Date: Septemeber 09, 2020
 * Due Date: September 10, 2020
 * 
 * Modified wireless RC Car using the NRF24L01
 * Designed for speed
 * 
 * Code: RECEIVER
 * 
 */

#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/******************* User Config **************************************/
#define pin_motor1_dir0   2
#define pin_motor1_dir1   4
#define pin_motor2_dir0   7
#define pin_motor2_dir1   8
#define pin_motor1_pwm    3 //Analog output to control speed of motor
#define pin_motor2_pwm    5
#define pin_pot_speed     A0
#define pin_pot2_dir      A1
int motorSpeed = 0;
int motorDir;

RF24 radio(5,6); // CE, CSN
const byte address[6] = "00001";    //Address we send data. Should be same as receiving side.
/******************* User Config **************************************/

Servo dirServo; // create servo object

//Function prototypes
void printStatus(void);
void wireless_init(void);

void setup() {
  //Setup dirction pins
  pinMode(pin_motor1_dir0,OUTPUT);
  pinMode(pin_motor1_dir1,OUTPUT);
  pinMode(pin_motor2_dir0,OUTPUT);
  pinMode(pin_motor2_dir1,OUTPUT);

  //Setup of servo to control direction of RC car
  dirServo.attach(9);
  dirServo.write(90); //allign wheels straight

  //Wireless communcation setup
  wireless_init();
  Serial.begin(9600);

}

void loop() {
  //*********************************
  //  pin_pot_speed neutral at 132  *  --> VRY
  //  pin_pot2_dir neutral at 127   *  --> VRX
  //*********************************
  
  // Read Speed and Direction Input
  motorSpeed = map(analogRead(pin_pot_speed),0,1023,255,0);
  motorDir = map(analogRead(pin_pot2_dir),0,1023,0,180);
  delay(200);

  // Direction Decisions
  if(motorDir == 89)
  {
    dirServo.write(90);
  }
  else if(motorDir < 89)
  {
    dirServo.write(motorDir);
  }
  else if(motorDir > 89)
  {
    dirServo.write(motorDir);
  }

  // Speed Decisions
  if(motorSpeed == 132)
  {
    analogWrite(pin_motor1_pwm,0);
  }
  else if(motorSpeed < 132) //Slowdown or reverse
  {
    digitalWrite(pin_motor1_dir0, HIGH);
    digitalWrite(pin_motor1_dir1, LOW);
    analogWrite(pin_motor1_pwm, 255-motorSpeed);
  }
  else if(motorSpeed > 132)
  {
    digitalWrite(pin_motor1_dir0, LOW);
    digitalWrite(pin_motor1_dir1, HIGH);
    analogWrite(pin_motor1_pwm, motorSpeed);
  }

  // Print Status for Debugging
  printStatus();

  delay(100);
}

void printStatus(void)
{
  Serial.print("Speed: ");
  Serial.println(motorSpeed);
  Serial.print("Direction: ");
  Serial.println(motorDir); 
}

void wireless_init(void)
{
  Serial.begin(115200);
  radio.begin();                  //Start wireless comm
  radio.openWritingPipe(address); //Set address
  radio.setPALevel(RF24_PA_LOW);  //Set PA level low
  radio.startListening();         //Set module as receiver
}
