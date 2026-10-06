#include <ZumoMotors.h>
#include <SoftwareSerial.h>

// -------- Pin Configuration --------
#define TRIG_PIN      A1
#define ECHO_PIN      A0
#define BT_RX_PIN     4    // receives from HC-05 TXD
#define BT_TX_PIN     5    // sends to HC-05 RXD

// -------- Configuration --------
#define SPEED         200
#define TURN_SPEED    180
#define OBSTACLE_DIST 15   // cm

ZumoMotors motors;
SoftwareSerial bluetooth(BT_RX_PIN, BT_TX_PIN);

char currentCommand = 'S';

// -------- Get distance --------
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

// -------- Execute command --------
void executeCommand(char cmd) {
  // Securite : bloquer l'avance si obstacle detecte
  if (cmd == 'F' && getDistance() < OBSTACLE_DIST) {
    motors.setSpeeds(0, 0);
    Serial.println("Obstacle bloque !");
    return;
  }

  switch (cmd) {
    case 'F': motors.setSpeeds( SPEED,       SPEED);       break;
    case 'B': motors.setSpeeds(-SPEED,      -SPEED);       break;
    case 'L': motors.setSpeeds(-TURN_SPEED,  TURN_SPEED);  break;
    case 'R': motors.setSpeeds( TURN_SPEED, -TURN_SPEED);  break;
    case 'S': motors.setSpeeds(0, 0);                      break;
  }
}

// -------- Setup --------
void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  motors.setSpeeds(0, 0);
  Serial.println("Robot pret - en attente de commandes...");
}

// -------- Loop --------
void loop() {
  // Lire commande Bluetooth si disponible
  if (bluetooth.available()) {
    char cmd = bluetooth.read();

    if (cmd == 'F' || cmd == 'B' || cmd == 'L' || cmd == 'R' || cmd == 'S') {
      currentCommand = cmd;
      Serial.print("Commande reçue : ");
      Serial.println(cmd);
    }
  }

  // Executer la commande courante
  executeCommand(currentCommand);

  delay(50);
}