#include <Arduino.h>
#include <M5_STHS34PF80.h>

M5_STHS34PF80 TMOS;

uint8_t motionHysteresis = 0;
int16_t motionVal = 0, presenceVal = 0;
uint16_t motionThresholdVal = 0, precenceThresholdVal = 0;
sths34pf80_gain_mode_t gainMode;

void setup() {
  Serial.begin(115200);

  // Initialise I2C and devices
  while (!TMOS.begin(&Wire, STHS34PF80_I2C_ADDRESS, SDA, SCL)) {
    Serial.printf("I2C Error - check I2C Address\n");
    Serial.printf("Error setting up device - please check wiring.\n");
    delay(1000);
  }

  // Set the TMOS sensor output data rate
  TMOS.setTmosODR(STHS34PF80_TMOS_ODR_AT_2Hz);

  // TMOS.setMotionThreshold(0xFF);
  TMOS.setPresenceThreshold(0xC8);  // Default value
  TMOS.setMotionThreshold(0xC8);
  TMOS.setPresenceHysteresis(0x32);
  TMOS.setMotionHysteresis(0x32);

  // Default mode, may cause inaccurate detection of MoionValFlag bit.
  // TMOS.setGainMode(STHS34PF80_GAIN_DEFAULT_MODE);

  // Decreasing gain mode, detection distance decreases
  TMOS.setGainMode(STHS34PF80_GAIN_WIDE_MODE);
  TMOS.setTmosSensitivity(0xFF);
  TMOS.resetAlgo();

  TMOS.getGainMode(&gainMode);
  TMOS.getMotionThreshold(&motionThresholdVal);
  TMOS.getPresenceThreshold(&precenceThresholdVal);
  TMOS.getMotionHysteresis(&motionHysteresis);
  Serial.printf("presenceThresholdVal:%x, motionThresholdVal:%x, motionHysteresis:%x GainMode:%x\n", precenceThresholdVal, motionThresholdVal, motionHysteresis, gainMode);
  delay(1000);
}

void loop() {
  sths34pf80_tmos_drdy_status_t dataReady;
  TMOS.getDataReady(&dataReady);

  if (dataReady.drdy == 1) {
    // Real-time detection of presence and motion infrared energy values
    sths34pf80_tmos_func_status_t status;
    TMOS.getPresenceValue(&presenceVal);
    TMOS.getMotionValue(&motionVal);
    TMOS.getStatus(&status);

    Serial.printf("Presence:%d\t", presenceVal);
    Serial.printf("PresenceFlag:%s\t", status.pres_flag ? "Detected" : "None");
    Serial.printf("Motion:%d\t", motionVal);
    Serial.printf("MotionValFlag:%s\n", status.mot_flag ? "Detected" : "None");
  }
}