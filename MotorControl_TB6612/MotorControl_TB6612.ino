/*
   Author: Jay Pacamarra
   Date: Septemeber 09, 2020
   Due Date: September 10, 2020

   Modified wireless RC Car using the NRF24L01
   Designed for speed

   Code: RECEIVER

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

RF24 radio(5, 6); // CE, CSN
const byte address[6] = "00001";    //Address we send data. Should be same as receiving side.

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

struct Data_Package {
  int motorSpeed;
  int motorDir;
};
Data_Package data; //init data object
/******************* User Config **************************************/

Servo dirServo; // create servo object

//Function prototypes
void printStatus(void);
void wireless_init(void);
void resetData(void);

void setup() {
  //Setup dirction pins
  pinMode(pin_motor1_dir0, OUTPUT);
  pinMode(pin_motor1_dir1, OUTPUT);
  pinMode(pin_motor2_dir0, OUTPUT);
  pinMode(pin_motor2_dir1, OUTPUT);

  //Setup of servo to control direction of RC car
  dirServo.attach(9);
  dirServo.write(90); //allign wheels straight

  //Wireless communcation setup
  wireless_init();

}

void loop() {

  // Read Speed and Direction Input
  if (radio.available()) {
    Serial.println("Radio Data Received"); // Debugging only
    
    radio.read(&data, sizeof(Data_Package));

    //Just in case values exceed max of 255 or min of 0
    if(data.motorSpeed > 255) data.motorSpeed=255;
    if(data.motorSpeed < 0) data.motorSpeed=0;

    //Update time status for resetData or not
    lastReceiveTime = millis(); //this is the time we received data
    currentTime = millis();

    // ********** Direction Decisions ************
    if (data.motorDir > 400 && data.motorDir < 600)
    {

    }
    else {
      if (data.motorDir == 89)
      {
        dirServo.write(90);
      }
      else if (data.motorDir < 89)
      {
        dirServo.write(data.motorDir);
      }
      else if (data.motorDir > 89)
      {
        dirServo.write(data.motorDir);
      }
    }
    
    // ********** Speed Decisions ************
    if (data.motorSpeed == 132 || data.motorSpeed == 133)
    {
      analogWrite(pin_motor1_pwm, 0);
      Serial.println("stop");
    }
    else if (data.motorSpeed < 132) //Slowdown or reverse
    {
      digitalWrite(pin_motor1_dir0, HIGH);
      digitalWrite(pin_motor1_dir1, LOW);
      analogWrite(pin_motor1_pwm, 255 - data.motorSpeed);
      Serial.println("reverse");
    }
    else if (data.motorSpeed > 133)
    {
      digitalWrite(pin_motor1_dir0, LOW);
      digitalWrite(pin_motor1_dir1, HIGH);
      analogWrite(pin_motor1_pwm, data.motorSpeed);
      Serial.println("forward");
    }
  }
  //if ( millis() - lastReceiveTime > 7000) resetData(); //Reset car if out of range

  // Print Status for Debugging
  printStatus();
  
}

void printStatus(void)
{
  Serial.print("Speed: ");
  Serial.println(data.motorSpeed);
  Serial.print("Direction: ");
  Serial.println(data.motorDir);
  Serial.println("end of transmission code");
}

void wireless_init(void)
{
  Serial.begin(9600);
  radio.begin();                  //Start wireless comm
  radio.openReadingPipe(0, address); //Set address
  radio.setPALevel(RF24_PA_LOW);  //Set PA level min
  radio.startListening();         //Set module as receiver
  resetData();
}

void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.motorSpeed = 132;
  data.motorDir = 90;
}
