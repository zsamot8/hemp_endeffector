/*
 * rosserial Servo Control Example
 *
 * This sketch demonstrates the control of hobby R/C servos
 * using ROS and the arduiono
 * 
 * For the full tutorial write up, visit
 * www.ros.org/wiki/rosserial_arduino_demos
 *
 * For more information on the Arduino Servo Library
 * Checkout :
 * http://www.arduino.cc/en/Reference/Servo
 */
#include <ros.h>
#include <std_msgs/UInt16.h>

#define servoChannel_0  0
#define servoChannel_1  1
#define servoTimer12bit  12
#define servoBaseFreq  50

int servoPwmPin = 40;
int linActPwmPin = 41;
int powerActivationPin = 8; //done
int esfrontPin = 39; //done
int esbackPin = 47; //done
int directionPin = 48; //done 0 pump, 1 pull

// variable to store the servo position
int servoPos = 26;
int pumpspeed = 220; //255 - extensive heating and oscilations
int esStateFront = 0;
int esStateBack = 0;

void servoAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax=255){
  uint32_t duty = (4095/valueMax)* min(value, valueMax);
  ledcWrite (channel, duty);
}

ros::NodeHandle  nh;

void hempCb( const std_msgs::UInt16& cmd_msg){
  switch(cmd_msg.data)
  {
  case 1:
  servoAnalogWrite(servoChannel_1, pumpspeed);
  digitalWrite(directionPin, LOW);
  delay(500);
  servoAnalogWrite(servoChannel_1, 0);
  break;
  case 2:
  servoAnalogWrite(servoChannel_1, pumpspeed);
  digitalWrite(directionPin, LOW);
  delay(1000);
  servoAnalogWrite(servoChannel_1, 0);
  break;
  case 3:
  servoAnalogWrite(servoChannel_1, pumpspeed);
  digitalWrite(directionPin, LOW);
  delay(1500);
  servoAnalogWrite(servoChannel_1, 0);
  break;
  case 4:
  servoAnalogWrite(servoChannel_1, pumpspeed);
  digitalWrite(directionPin, HIGH);
  delay(6000);
  servoAnalogWrite(servoChannel_1, 0); 
  break;
  case 5:
  digitalWrite(powerActivationPin, HIGH); 
  servoPos = 11;//fully closed
  servoAnalogWrite(servoChannel_0, servoPos);
  delay(3000);
  servoPos = 26;//fully open
  servoAnalogWrite(servoChannel_0, servoPos);
  delay(3000);
  digitalWrite(powerActivationPin, LOW);
  break;
  case 6:
  digitalWrite(powerActivationPin, HIGH); 
  servoPos = 11;//fully closed
  servoAnalogWrite(servoChannel_0, servoPos);
  delay(1500);
  servoAnalogWrite(servoChannel_1, pumpspeed);
  digitalWrite(directionPin, LOW);
  delay(500);
  servoAnalogWrite(servoChannel_1, 0);
  delay(1000);
  servoPos = 26;//fully open
  servoAnalogWrite(servoChannel_0, servoPos);
  delay(3000);
  digitalWrite(powerActivationPin, LOW);
  break;
  default:
  delay(100);
  break; 
  }
}
ros::Subscriber<std_msgs::UInt16> sub("hemp", &hempCb);

void setup(){
  //servo
  pinMode(powerActivationPin, OUTPUT); 
  digitalWrite(powerActivationPin, HIGH); 
  ledcSetup(servoChannel_0, servoBaseFreq, servoTimer12bit);
  ledcAttachPin(servoPwmPin, servoChannel_0);
  //endswitches
  pinMode(esfrontPin, INPUT);
  pinMode(esbackPin, INPUT);
  //pompa
  pinMode(directionPin, OUTPUT);
  ledcSetup(servoChannel_1, servoBaseFreq, servoTimer12bit);
  ledcAttachPin(linActPwmPin, servoChannel_1);
  //ROS 
  nh.initNode();
  nh.subscribe(sub);
  
}

void loop(){
  nh.spinOnce();
  delay(1);
}
