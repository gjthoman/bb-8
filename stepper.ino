#include <AccelStepper.h>
#include <MultiStepper.h>
#include <math.h>

int ch1_pin = 12;
int ch2_pin = 11;

int ch1;
int ch2;
long lastUpdate = 0;
int updateFreq = 100; 
int degree = 0;
int velocity = 0;
String inputString = "";  
boolean stringComplete = false;

float degreeTable[12][3] = {
    {1, -0.5, -0.5},//0
    {1, 0, -1},//30
    {0.5, 0.5, -1},//60
    {0, 1, -1},//90
    {-0.5, 1, -0.5},//120
    {-1, 1, 0},//150
    {-1, 0.5, 0.5},//180
    {-1, 0, 1},//210
    {-0.5, -0.5, 1},//240
    {0, -1, 1},//270
    {0.5, -1, 0.5},//300
    {1, -1, 0}//330
};
int stepper2newSpeed = 0;

#define STEPPER1_DIR_PIN 9
#define STEPPER1_STEP_PIN 10

#define STEPPER2_DIR_PIN 6
#define STEPPER2_STEP_PIN 7

#define STEPPER3_DIR_PIN 3
#define STEPPER3_STEP_PIN 4

AccelStepper stepper1(AccelStepper::DRIVER, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, STEPPER2_STEP_PIN, STEPPER2_DIR_PIN);
AccelStepper stepper3(AccelStepper::DRIVER, STEPPER3_STEP_PIN, STEPPER3_DIR_PIN);

void setup()
{  
    pinMode(ch1_pin, INPUT);
    pinMode(ch2_pin, INPUT);
    Serial.begin(115200);
    inputString.reserve(200);
    stepper1.setMaxSpeed(4000.0);
    stepper2.setMaxSpeed(4000.0);
    stepper3.setMaxSpeed(4000.0);
}

void updateMotors(){
  stepper1.setSpeed(degreeTable[degree][0]*velocity);
  stepper2.setSpeed(degreeTable[degree][1]*velocity);
  stepper3.setSpeed(degreeTable[degree][2]*velocity);
}

int d2r(int degree){
  return degree*3.14159/180;
}

void loop()
{
  if (stringComplete) {
    stringComplete = false;
    char str[100];
    strcpy(str, inputString.c_str());

    char *dir[10];
    dir[0] = strtok(str, ",");
    dir[1] = strtok (NULL, ",;");
    
    for(int i = 0; i < 2; i++){
        Serial.println(dir[i]);  
    }

    degree = charToInt(dir[0]) % 12;
    velocity = charToInt(dir[1]) * 40;
    updateMotors();
    inputString = "";
  }

    stepper1.runSpeed();
    stepper2.runSpeed();
    stepper3.runSpeed();
}

int charToInt(char *str){
  return atoi(str);
}

int getRangeValue(int value) {
  int rangeValue = int((float(value) - 1110)/((1904-1110)/2)*100) - 100;
  return rangeValue;
}

float getAngle(int xDiff, int yDiff){
  return atan2(yDiff, xDiff) * (180 / 3.14159); 
}

float getVelocity(int x, int y){
  return max(min(102, sqrt(x*x + y*y)), 2) - 2;
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    
    inputString += inChar;
    
    if (inChar == ';') {
      stringComplete = true;
    }
  }
}

