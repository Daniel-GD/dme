#include <Servo.h>
#include <NewPing.h>

#define SONARTHRESH 10
#define RETRACTED 0
#define KNOCK 120
#define SHORT 500
#define LONG 600000 //10 minute
#define LOOP_TIME 5000
#define READ_COUNT 5

#define SERVO_PIN 10
#define SONAR_TRIG 7
#define SONAR_ECHO 8

Servo myservo; // instantiate servo object
NewPing sonar(SONAR_TRIG, SONAR_ECHO, 300); //instantiate NewPing object

uint32_t timer;
bool objects[READ_COUNT];
bool last_detected;
uint8_t knock_count;



/* Function Declaration */
int readSonar(bool print_variable = false) {
  int sonar1 = sonar.ping_cm();
  if (print_variable) {
    Serial.println(sonar1);
  }
  return sonar1;
}

void knock_print() {
  /*
     This function knocks down the print towards the front of the printer and then retracts arm
  */
  myservo.write(KNOCK);
  delay(SHORT);
  myservo.write((KNOCK - RETRACTED) / 2);
  delay(SHORT);
  myservo.write(KNOCK);
  delay(SHORT);
  myservo.write(RETRACTED);
  delay(SHORT);
  myservo.write(KNOCK);
  delay(SHORT);
  myservo.write((KNOCK - RETRACTED) / 2);
  delay(SHORT);
  myservo.write(KNOCK);
  delay(SHORT);
  myservo.write(RETRACTED);
}

bool multiple_readings() {
  /*
     Returns true if sonar has read a value for READ_COUNT times
  */
  int s = 0;
  for (int i = 0; i < READ_COUNT; i++) {
    s += objects[i];
  }
  if (s == READ_COUNT) {
    return true;
  }
  else {
    return false;
  }
}

//////////////// End Functions //////////////////
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(SERVO_PIN); // attach servo to pin 10
  myservo.write(RETRACTED);
  for (int i = 0; i < READ_COUNT; i++) {
    objects[i] = false;
  }
  last_detected = false;
  knock_count=0;
}

void loop() {
  Serial.print("Sonar: ");
  int distance = readSonar(true);
  bool object_detected = distance < SONARTHRESH;
  bool shouldbe = object_detected && last_detected;
  if (shouldbe) Serial.println("Should be knocking");
  if ( object_detected && last_detected && millis() - timer >= LONG) {
    timer = millis(); //timer prevents from knocking more than once in a short span of time
    Serial.println("KNOCKING");
    knock_print();
    knock_count++;
  }
  Serial.print("Knock count: ");
  Serial.println(knock_count);
  last_detected = object_detected;

  delay(LOOP_TIME);

}
