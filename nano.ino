// =====================================================
// GoodBoy Pet Dog - Base Servo Command Firmware (SMOOTH)
// Board: Arduino Uno / Nano / Mega
// Servos: 8 (connected to pins 2–9)
// =====================================================

#include <Servo.h>

#define SERVO_COUNT 8

// Servo pins: Servo1 -> pin2 ... Servo8 -> pin9
const uint8_t servoPins[SERVO_COUNT] = {2,3,4,5,6,7,8,9};

Servo servos[SERVO_COUNT];

struct Pose {
  const char* name;
  uint8_t angle[SERVO_COUNT];
};

// ------------------- POSES ----------------------------
Pose POSES[] = {
  {"sit",        {62, 71, 90, 94, 110, 125, 82, 90}},
  {"stand",     {109, 48, 44, 112, 41, 135, 129, 63}},
  {"halfstand", {110, 65, 44, 90, 58, 114, 129, 88}},
  {"forwardsit",{97, 43, 55, 113, 31, 66, 150, 140}},
  {"backsit",   {87, 136, 76, 23, 53, 113, 121, 95}},
  {"namaste",   {62, 116, 90, 45, 101, 60, 92, 150}}
};

const uint8_t POSE_COUNT = sizeof(POSES) / sizeof(POSES[0]);

// ------------------- SMOOTH MOVE ----------------------
void moveToPose(const Pose &p, uint16_t stepDelay) {
  static int currentAngle[SERVO_COUNT] = {90,90,90,90,90,90,90,90};

  bool moving = true;

  while (moving) {
    moving = false;

    for (uint8_t i = 0; i < SERVO_COUNT; i++) {
      if (currentAngle[i] < p.angle[i]) {
        currentAngle[i]++;
        servos[i].write(currentAngle[i]);
        moving = true;
      }
      else if (currentAngle[i] > p.angle[i]) {
        currentAngle[i]--;
        servos[i].write(currentAngle[i]);
        moving = true;
      }
    }

    delay(stepDelay);
  }
}

// ------------------- SERIAL HELP ----------------------
void printHelp() {
  Serial.println(F("\nAvailable commands:"));
  for (uint8_t i = 0; i < POSE_COUNT; i++) {
    Serial.print(" - ");
    Serial.println(POSES[i].name);
  }
  Serial.println(F(" - help"));
}

// ------------------- SETUP ----------------------------
void setup() {
  Serial.begin(115200);

  for (uint8_t i = 0; i < SERVO_COUNT; i++) {
    servos[i].attach(servoPins[i]);
  }

  Serial.println(F("GoodBoy Pet Dog Ready (Smooth Mode) 🐶"));
  printHelp();
}

// ------------------- LOOP -----------------------------
void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.equalsIgnoreCase("help")) {
      printHelp();
      return;
    }

    for (uint8_t i = 0; i < POSE_COUNT; i++) {
      if (cmd.equalsIgnoreCase(POSES[i].name)) {
        moveToPose(POSES[i], 15);  // change speed here
        Serial.print(F("Pose executed: "));
        Serial.println(POSES[i].name);
        return;
      }
    }

    Serial.println(F("Unknown command. Type 'help'"));
  }
}
