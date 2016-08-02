#include <math.h>

int ch1_pin = 12;
int ch2_pin = 13;

int speed1Pin = 9; // H-bridge enable pin for speed control
int motor1ForwardPin = 2; // H-bridge leg 1
int motor1BackwardPin = 3; // H-bridge leg 2

int speed2Pin = 10; // H-bridge enable pin for speed control
int motor2ForwardPin = 4; // H-bridge leg 1
int motor2BackwardPin = 5; // H-bridge leg 2

int speed3Pin = 11; // H-bridge enable pin for speed control
int motor3ForwardPin = 6; // H-bridge leg 1
int motor3BackwardPin = 7; // H-bridge leg 2

int deadArea = 5;
 
int ch1;
int ch2;
 
void setup() {
  pinMode(ch1_pin, INPUT);
  pinMode(ch2_pin, INPUT);

  pinMode(speed1Pin, OUTPUT);
  pinMode(motor1ForwardPin, OUTPUT);
  pinMode(motor1BackwardPin, OUTPUT);
  
  pinMode(speed2Pin, OUTPUT);
  pinMode(motor2ForwardPin, OUTPUT);
  pinMode(motor2BackwardPin, OUTPUT);
  
  pinMode(speed3Pin, OUTPUT);
  pinMode(motor3ForwardPin, OUTPUT);
  pinMode(motor3BackwardPin, OUTPUT);
 
  //Serial.begin(9600);
}
 
void loop() {
  ch1 = getRangeValue(pulseIn(ch1_pin, HIGH));
  ch2 = - getRangeValue(pulseIn(ch2_pin, HIGH));

  int vel = getVelocity(ch1, ch2);
  float ang = getAngle(ch1, ch2);
//    Serial.print("\t");
//  Serial.print(ang);
  float motor_1_rv = sin(ang - degreeToRad(120)) * vel; //SIN((degrees + 120) * pi / 180)
  float motor_2_rv = sin(ang) * vel; //SIN(degrees * pi / 180)
  float motor_3_rv = sin(ang + degreeToRad(120)) * vel; //SIN((degrees + 240) * pi / 180)
//  Serial.print("\t");
//  Serial.print(motor_1_rv);
//  Serial.print("\t");
//  Serial.print(motor_2_rv);
//  Serial.print("\t");
//  Serial.println(motor_3_rv);

  if (motor_1_rv >= 0 ) {
    digitalWrite(motor1ForwardPin, LOW);
    digitalWrite(motor1BackwardPin, HIGH); 
  } else {
    digitalWrite(motor1ForwardPin, HIGH);
    digitalWrite(motor1BackwardPin, LOW);
  }

  if (motor_2_rv >= 0 ) {
    digitalWrite(motor2ForwardPin, LOW);
    digitalWrite(motor2BackwardPin, HIGH); 
  } else {
    digitalWrite(motor2ForwardPin, HIGH);
    digitalWrite(motor2BackwardPin, LOW);
  }

  if (motor_3_rv >= 0 ) {
    digitalWrite(motor3ForwardPin, LOW);
    digitalWrite(motor3BackwardPin, HIGH); 
  } else {
    digitalWrite(motor3ForwardPin, HIGH);
    digitalWrite(motor3BackwardPin, LOW);
  }

  if(abs(motor_1_rv) > deadArea){
    analogWrite(speed1Pin, abs(motor_1_rv)); 
  } else {
    analogWrite(speed1Pin, 0); 
  }
  if(abs(motor_2_rv) > deadArea){
    analogWrite(speed2Pin, abs(motor_2_rv));   
  } else {
    analogWrite(speed2Pin, 0); 
  }
  if(abs(motor_3_rv) > deadArea){
    analogWrite(speed3Pin, abs(motor_3_rv));   
  } else {
    analogWrite(speed3Pin, 0); 
  }
  
  
}

int getRangeValue(int value) {
  int rangeValue = int((float(value) - 1110)/((1904-1110)/2)*255) - 255;
  return rangeValue;
}

float getAngle(int xDiff, int yDiff){
  return atan2(yDiff, xDiff); 
}

float degreeToRad(float degree){
  return degree * (180 / 3.14159);
}

int getVelocity(int x, int y){
  return max(min(102, sqrt(x*x + y*y)), 2) - 2;
}
