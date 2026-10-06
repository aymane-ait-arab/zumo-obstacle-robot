#include <ZumoMotors.h>

// -------- Pin Configuration --------
#define TRIG_PIN      A1
#define ECHO_PIN      A0

// -------- Configuration --------
#define SPEED         200
#define TURN_SPEED    180
#define OBSTACLE_DIST 20

ZumoMotors motors;

// -------- Get distance in cm --------
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) return 999;

  return duration * 0.034 / 2;
}

// -------- Setup --------
void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Obstacle avoidance ready !");
  delay(1000);
}

// -------- Loop --------
void loop() {
  long distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < OBSTACLE_DIST) {
    Serial.println("Obstacle ! Turning...");

    motors.setSpeeds(0, 0);
    delay(200);

    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(400);

    motors.setSpeeds(0, 0);
    delay(100);

  } else {
    motors.setSpeeds(SPEED, SPEED);
  }

  delay(50);
}