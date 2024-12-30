#include <Wire.h>
#include "SparkFun_BNO08x_Arduino_Library.h"  // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_BNO08x

BNO08x myIMU;

// For the most reliable interaction with the SHTP bus, we need
// to use hardware reset control, and to monitor the H_INT pin.
#define BNO08X_INT  A4
#define BNO08X_RST  A5

#define BNO08X_ADDR 0x4B  // SparkFun BNO08x Breakout default address

// Variables to store incoming values
float mx, my, mz;            // Magnetometer readings
byte magAccuracy;
float quatI, quatJ, quatK, quatReal; // Quaternion values

unsigned long previousDebugMicros = 0;
#define DEBUG_INTERVAL_MICROSECONDS 1000000

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for USB Serial to initialize
  
  Serial.println();
  Serial.println("BNO08x Calibration Example");

  // Initialize I2C communication
  Wire.begin();
  Wire.setClock(100000); // Set I2C clock speed to 400kHz for faster communication

  // if (myIMU.setCalibrationConfig(SH2_CAL_ACCEL | SH2_CAL_GYRO | SH2_CAL_MAG)) {
  //   Serial.println(F("Calibration Command Sent Successfully"));
  // } else {
  //   Serial.println("Could not send Calibration Command. Freezing...");
  //   while (1) delay(10);
  // }
  
  if (myIMU.begin(BNO08X_ADDR, Wire, BNO08X_INT, BNO08X_RST) == false) {
  Serial.println("Initialization failed: Possible causes:");
  Serial.println("- Incorrect I2C wiring");
  Serial.println("- INT/RST pins misconfigured");
  Serial.println("- Sensor not powered or damaged");
  while (1);
} else {
  Serial.println("BNO08x initialization successful!");
}


  setReports();

  Serial.println("Reading events");
  delay(100);
}

// Define sensor outputs to receive
void setReports(void) {
  Serial.println("Setting desired reports");

  if (myIMU.enableMagnetometer(1)) {
    Serial.println(F("Magnetometer enabled (x, y, z in uTesla)"));
  } else {
    Serial.println("Could not enable magnetometer");
  }

  if (myIMU.enableGameRotationVector(1)) {
    Serial.println(F("Game Rotation vector enabled (i, j, k, real)"));
  } else {
    Serial.println("Could not enable game rotation vector");
  }
}

void loop() {
  delayMicroseconds(10);

  if (myIMU.wasReset()) {
    Serial.println("Sensor was reset");
    setReports();
  }

  if (myIMU.getSensorEvent()) {
    if (myIMU.getSensorEventID() == SENSOR_REPORTID_MAGNETIC_FIELD) {
      mx = myIMU.getMagX();
      my = myIMU.getMagY();
      mz = myIMU.getMagZ();
      magAccuracy = myIMU.getMagAccuracy();
    } else if (myIMU.getSensorEventID() == SENSOR_REPORTID_GAME_ROTATION_VECTOR) {
      quatI = myIMU.getGameQuatI();
      quatJ = myIMU.getGameQuatJ();
      quatK = myIMU.getGameQuatK();
      quatReal = myIMU.getGameQuatReal();
    }
  }

  unsigned long microsSinceLastSerialPrint = micros() - previousDebugMicros;

  if (microsSinceLastSerialPrint > DEBUG_INTERVAL_MICROSECONDS) {
    Serial.print(mx, 2); Serial.print("\t");
    Serial.print(my, 2); Serial.print("\t");
    Serial.print(mz, 2); Serial.print("\t");
    printAccuracyLevel(magAccuracy); Serial.print("\t");

    Serial.print(quatI, 2); Serial.print("\t");
    Serial.print(quatJ, 2); Serial.print("\t");
    Serial.print(quatK, 2); Serial.print("\t");
    Serial.print(quatReal, 2); Serial.print("\t");

    Serial.print(microsSinceLastSerialPrint);
    Serial.println();

    previousDebugMicros = micros();
  }

  if (Serial.available()) {
    byte incoming = Serial.read();

    if (incoming == 's') {
      if (myIMU.saveCalibration()) {
        Serial.println(F("Calibration data saved successfully"));
      } else {
        Serial.println("Save Calibration Failure");
      }
    }
  }
}

// Print accuracy level
void printAccuracyLevel(byte accuracyNumber) {
  if (accuracyNumber == 0) Serial.print(F("Unreliable"));
  else if (accuracyNumber == 1) Serial.print(F("Low"));
  else if (accuracyNumber == 2) Serial.print(F("Medium"));
  else if (accuracyNumber == 3) Serial.print(F("High"));
}
