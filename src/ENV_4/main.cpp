#include <Adafruit_BMP280.h>
#include <Adafruit_SHT4x.h>
#include <Arduino.h>

Adafruit_BMP280 bmp;
Adafruit_SHT4x sht4;

void setup() {
  Serial.begin(115200);

  // Wire.begin(SDA, SCL);

  // -------- search for SHT4x --------
  if (!sht4.begin()) {
    Serial.printf("Couldn't find SHT4x\n");
    while (1) delay(1);
  }
  // You can have 3 different precisions, higher precision takes longer
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);

  // -------- search for BMP280 --------
  if (!bmp.begin(BMP280_ADDRESS_ALT)) {
    Serial.printf(
        "Could not find a valid BMP280 sensor, check wiring or "
        "try a different address!\n");
    while (1) delay(1);
  }
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

uint8_t sampling_rate_Hz = 2;

uint32_t prev_millis = 0;

void loop() {
  if ((millis() - prev_millis) >= (1000 / sampling_rate_Hz)) {
    prev_millis = millis();

    sensors_event_t humidity, temp;
    sht4.getEvent(&humidity, &temp);  // populate temp and humidity objects with fresh data
    Serial.printf("[sht4x] temp:%.2f, hum:%.2f\t", temp.temperature, humidity.relative_humidity);

    // must call this to wake sensor up and get new measurement data
    // it blocks until measurement is complete
    if (bmp.takeForcedMeasurement()) {
      Serial.printf("[bmp280] temp:%.2f, pressure:%.2f", bmp.readPressure(), bmp.readTemperature());
    } else {
      Serial.printf("Forced measurement failed!");
    }

    Serial.printf("\n");
  }
}