/*Include Dependencies */
#include <Servo.h> //Helps sending commands to servos
#include "NewPing.h" //Helps parsing ultrasonic sensor data

/*Define Arduino Pins (Digital) */
#define SERVO_PIN 11
#define SONAR_TRIG 13
#define SONAR_ECHO 12

/*Define Parameters */
#define SONAR_THRESHOLD 8 // Threshold distance in which to actuate the servo[cm]
#define RETRACTED_ANGLE 0 // Servo Angle when the hand sanitizer nozzle is up [deg]
#define PUMP_ANGLE 100 // Servo Angle when the hand sanitizer nozzle is down [deg]
#define PUMP_TIME 1500 //Time the servo will stay in "pump mode" [ms]

/*Instantiate Objects */
Servo myservo; // instantiate servo object
NewPing sonar(SONAR_TRIG, SONAR_ECHO, 300); //instantiate NewPing sonar object

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //initialize serial communication
  myservo.attach(SERVO_PIN); // attach servo to specified pin
  myservo.write(RETRACTED_ANGLE); //set servo initially to retracted position
}

void loop() {
  //loop code, runs "infinitely"
  int sonar_reading= sonar.ping_cm(); //sonar reading in cm
  //Print Sonar Output
  Serial.print("Sonar Reading: ");
  Serial.println(sonar_reading);
  
  if (sonar_reading<SONAR_THRESHOLD){
    myservo.write(PUMP_ANGLE); //actuates servo to pull the string
    Serial.println("PUMPING");
    delay(PUMP_TIME); //wait some time 
    myservo.write(RETRACTED_ANGLE); // retracts servo to original position
    Serial.println("RETRACTING");
    delay(PUMP_TIME); //wait some time 
  }

  delay(25); //controls how fast(in ms) code will be executed
}
