#include <RedBot.h>  // This line "includes" the RedBot library into your sketch.
// Provides special objects, methods, and functions for the RedBot.

/*
Port inputs as constants corresponding to various analog I/O devices
Line followers, distance sensors, digital motor controller
*/
RedBotSensor leftFollower = RedBotSensor(A0);
RedBotSensor middleFollower = RedBotSensor(A1);
RedBotSensor rightFollower = RedBotSensor(A2);

/*
A is left, B is right
*/
const int AIN1 = 13;
const int AIN2 = 12;
const int PWMA = 11;
const int PWMB = 10;
const int BIN2 = 9;
const int BIN1 = 8;

String output;

//Motor gain related constants
const int blackThreshold = 750;
const int redThreshold = 400;

const int normal = 20;
const int adjust = 30;
const int turn = 70;
const int wallDist = 600;

/*
 * Right to left: A0, A1, A2
 * Motor polarity corresponding to 1-2: HIGH-LOW for forward, LOW-HIGH for backward
 */
void setup() {
  Serial.begin(9600);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, 0); /* LEFT */
  analogWrite(PWMB, 0); /* RIGHT */

}

void loop() {
  int switchVal = digitalRead(7);
  
  output = lineFollowerStats();
  Serial.println(output);
  delay(1000);
  
  output = distanceSensorStats();
  Serial.println(output);
  delay(1000);
  

  /* SENSOR DATA: Line Followers */
  /*
  Serial.println("Left: " + String(leftSensor.read()) + 
  ", Middle: "+ String(middleSensor.read()) + 
  ", Right: " + String(rightSensor.read()));
  delay(turn0);
  */

  /* SENSOR DATA: Distance Sensors */
  /*
  Serial.println("Left: " + String(analogRead(A3)) + 
  ", Front: "+ String(analogRead(A4)) + 
  ", Right: " + String(analogRead(A5)));
  delay(turn0);
  */
  
  /*
   * END
   * If all line followers read red
   * Stop all motors
   *
   */
 if((rightFollower.read() < redThreshold) && 
    (leftFollower.read() < redThreshold) && 
    (middleFollower.read() < redThreshold)) {
  /* 
   *  STOP MOTORS
   */
   analogWrite(PWMA, 0); 
   analogWrite(PWMB, 0);
   
 } else if((rightFollower.read() > blackThreshold) && 
    (leftFollower.read() < blackThreshold)) {
   /*
   * Power is added based off of reading from IR sensors
   * Middle sensor on line = normal power
   * Right sensor reading = cut power on right and add to left
   * Left sensor reading = cut power on left and add to right
   */
    analogWrite(PWMA, adjust); /* LEFT */
    analogWrite(PWMB, 0); /* RIGHT */
    delay(3000);
    analogWrite(PWMA, normal); /* LEFT */
    analogWrite(PWMB, normal); /* RIGHT */
      
 } else if ((rightFollower.read() < blackThreshold) && 
    (leftFollower.read() > blackThreshold)) {
    analogWrite(PWMA, 0); /* LEFT */
    analogWrite(PWMB, adjust); /* RIGHT */
    delay(3000);
    analogWrite(PWMA, normal); /* LEFT */
    analogWrite(PWMB, normal); /* RIGHT */
    
 } else if (rightFollower.read() > blackThreshold && leftFollower.read() > blackThreshold) {
  /*
   * EXIT HIGHWAY
   * 
   */
   analogWrite(PWMA, turn); /* LEFT */
   analogWrite(PWMB, normal); /* RIGHT */
   delay(1500);
   analogWrite(PWMA, normal); /* LEFT */
   analogWrite(PWMB, normal); /* RIGHT */
   
 } else if (analogRead(A4) > 800) {
  /*
   * EXIT UNDER CONSTRUCTION
   * If front distance sensor detects an object (2 to 3 inches)
   * Turn the car around 180 degrees (set power for a certain speed - no encoders or gyro to use)
   *
   */
   analogWrite(PWMA, normal); /* LEFT */
   analogWrite(PWMB, -normal); /* RIGHT */
   delay(3000); /* Time for 180 degrees */
   analogWrite(PWMA, normal); /* LEFT */
   analogWrite(PWMB, normal); /* RIGHT */
 
 } 
}
String lineFollowerStats(){
  String output = "Line following sensors:\nLeft: " + String(leftFollower.read()) + 
  ", Middle: " + String(middleFollower.read()) + 
  ", Right: " + String(rightFollower.read());
  return output;
}

String distanceSensorStats(){
  String output = "Distance sensors:\nLeft: " + String(analogRead(A3)) + 
  ", Middle: " + String(analogRead(A4)) + 
  ", Right: " + String(analogRead(A5));
  return output;
}
