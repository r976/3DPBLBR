#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver();

#define servoMIN 150
#define servoMAX 600

void setup() {
  Serial.begin(9600);
  pca9685.begin();
  pca9685.setPWMFreq(60);
}

void loop() {
  for (int servo = 0; servo < 10; servo++ ) {
    pca9685.setPWM(servo, 0, servoMIN);
    Serial.println(servo);
    delay(300);
  }

  for (int servo = 9; servo >= 0; servo-- ) {
    pca9685.setPWM(servo, 0, servoMAX);
    Serial.println(servo);
    delay(300);
  }

}
