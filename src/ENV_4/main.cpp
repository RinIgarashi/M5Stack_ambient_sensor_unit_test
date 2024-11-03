#include <Arduino.h>
#include <BMP280.h>
#include <SHT4X.h>

SHT4X sht4x;
BMP280 bmp;

void setup() {
  Serial.begin(115200);

  if (!sht4x.begin(&Wire, SHT40_I2C_ADDR_44, SDA, SCL, 400000U)) {
    Serial.println("Couldn't find SHT4x");
    while (1) delay(1);
  }

  // You can have 3 different precisions, higher precision takes longer
  sht4x.setPrecision(SHT4X_HIGH_PRECISION);
  sht4x.setHeater(SHT4X_NO_HEATER);

  if (!bmp.begin(&Wire, BMP280_I2C_ADDR, SDA, SCL, 400000U)) {
    Serial.println("Couldn't find BMP280");
    while (1) delay(1);
  }
  /* Default settings from datasheet. */
  bmp.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                  BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  BMP280::FILTER_X16,      /* Filtering. */
                  BMP280::STANDBY_MS_500); /* Standby time. */
}

uint8_t sampling_rate_Hz = 2;

uint32_t prev_millis = 0;

void loop() {
  if ((millis() - prev_millis) >= (1000 / sampling_rate_Hz)) {
    prev_millis = millis();
    // Serial.println() is done in sht4x.update()
    if (sht4x.update() && bmp.update()) {
      Serial.printf("[sht4x] temp:%.2f, hum:%.2f\t", sht4x.cTemp, sht4x.humidity);
      Serial.printf("[bmp280] temp:%.2f, pressure:%.2f", bmp.cTemp, bmp.pressure);
    } else {
      Serial.printf("Couldn't read sensor data");
    }
  }
}