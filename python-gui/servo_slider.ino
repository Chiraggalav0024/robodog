#include <Servo.h>

#define SERVO_COUNT 8

Servo servo[SERVO_COUNT + 1];

// Servo number → Arduino pin
int servoPin[SERVO_COUNT + 1] = {
  0, 2, 3, 4, 5, 6, 7, 8, 9
};

int angle[SERVO_COUNT + 1] = {
  0,
  70, 80, 70, 80, 90, 80, 95, 110
};

void setup() {
  Serial.begin(9600);
  delay(2000);

  for (int i = 1; i <= SERVO_COUNT; i++) {
    servo[i].attach(servoPin[i]);
    servo[i].write(angle[i]);
  }

  Serial.println("Servo Slider Ready");
  Serial.println("Send: s1=90 s2=80 s3=70 ...");
}

void loop() {
  if (!Serial.available()) return;

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();

  // Parse commands like s1=90
  int sIndex = cmd.indexOf('s');
  while (sIndex != -1) {
    int eq = cmd.indexOf('=', sIndex);
    if (eq == -1) break;

    int servoNum = cmd.substring(sIndex + 1, eq).toInt();
    int value = cmd.substring(eq + 1).toInt();

    if (servoNum >= 1 && servoNum <= 8 && value >= 0 && value <= 180) {
      angle[servoNum] = value;
      servo[servoNum].write(value);
      Serial.print("Servo ");
      Serial.print(servoNum);
      Serial.print(" -> ");
      Serial.println(value);
    }

    sIndex = cmd.indexOf('s', eq);
  }
}
