/***********************************************************************
 * Exp2_DriveForward -- RedBot Experiment 2
 * 
 * Drive forward and stop.
 * 
 * Hardware setup:
 * The Power switch must be on, the motors must be connected, and the board must be receiving power 
 * from the battery. The motor switch must also be switched to RUN.
 *
 * 23 Sept 2013 N. Seidle/M. Hord
 * 04 Oct 2014 B. Huang
 ***********************************************************************/

#include <RedBot.h>  // This line "includes" the RedBot library into your sketch.
// Provides special objects, methods, and functions for the RedBot.

RedBotSensor rightSensor = RedBotSensor(A0);
RedBotSensor middleSensor = RedBotSensor(A1);
RedBotSensor leftSensor = RedBotSensor(A2);

const int AIN1 = 13;
const int AIN2 = 12;
const int PWMA = 11;
const int BIN1 = 8;
const int BIN2 = 9;
const int PWMB = 10;
const int threshold = 750;
const int speedBase = 50;
const int increment = 15;

int speedL;
int speedR;


/*
 * Right to left: A0, A1, A2
 */
void setup()
{
  Serial.begin(9600);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMA, 0); 
  analogWrite(PWMB, 0);
}

void loop()
{
  int switchVal = digitalRead(7);

  Serial.println("Raw sensor data: ");
  Serial.println("Left: " + String(leftSensor.read()) + ", Middle: "
  + String(middleSensor.read()) + ", Right: " + String(rightSensor.read()));
  delay(500);
/*
 * All IR sensors sensing red tape threshold  
 * (Should be roughly same as black tape)
 * Motors set to stop 
 */
 if((rightSensor.read() > threshold) && (leftSensor.read() > threshold) && (middleSensor.read() > threshold)) {
  /*Whatever a motor.stop() or whatever is equivalent to
   */
   motor.stop();
 } else {
  /*
   * whatever setting motor to speed is for the left and right motor
   */
   motor.motorL(speedL);
   motor.motorR(speedR);
 }
/*
 * Power is added based off of reading from IR sensors
 * Middle sensor on line = normal power
 * Right sensor reading = cut power on right and add to left
 * Left sensor reading = cut power on left and add to right
 */
  if(middleSensor.read() > threshold) {
    speedL = -speedBase;
    speedR = speedBase;
  } else if (rightSensor.read() > threshold) {
    leftSpeed = -(speedBase + increment);
    rightSpeed = (speedBase - increment);
  } else if (leftSensor.read() > threshold) {
    leftSpeed = -(speedBase - increment);
    rightSpeed = (speedBase + increment);
  } 

  

  /*
   * IR Sensor value > 700 for black tape = on the line, if right or left exceed this threshold
   * and center falls below the threshold, adjustment is needed
   * 
   * IR sensor value < 200 for red = on the line
   * 
   * Turn left and right functions require a VERY VERY slow turn speed to properly update the values of the IR sensors
   * and determine when to stop. Will determine which way to turn based on what sensor interacts with the black tape
   * If left sensor is on black tape, turn right and vice versa; Turn until the sensor that was on the tape is no longer on the tape
   * (should work for off and on ramp)
   * 
   * If obstacle encountered, stationary turn (one motor is backwards, one is forwards)
   * 
   * Need to get raw input from analog distance sensor for tunnel (adjust turn based on relative distance difference between distance to left and right wall)
   * 
   * 
   */

  
}
