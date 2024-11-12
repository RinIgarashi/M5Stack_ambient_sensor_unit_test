#include <Arduino.h>
// #include <Wire.h>
// #include <SPI.h>
// #include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

// #define BME_SCK 13
// #define BME_MISO 12
// #define BME_MOSI 11
// #define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

// Adafruit_BME680 bme(&Wire); // I2C
Adafruit_BME680 bme;  // I2C
// Adafruit_BME680 bme(&Wire1); // example of I2C on another bus
// Adafruit_BME680 bme(BME_CS); // hardware SPI
// Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.printf("BME680 test\n");

  if (!bme.begin()) {
    Serial.printf("Could not find a valid BME680 sensor, check wiring!\n");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);  // 320*C for 150 ms
}

float sampling_rate_Hz = 0.1;

uint32_t prev_millis = 0;

void loop() {
  if ((millis() - prev_millis) >= (1000 / sampling_rate_Hz)) {
    prev_millis = millis();

    if (!bme.performReading()) {
      Serial.printf("Failed to perform reading\n");
      return;
    }

    Serial.printf("temp:%.2f\t", bme.temperature);
    Serial.printf("humidity:%.2f\t", bme.humidity);
    Serial.printf("pressure:%d\t", bme.pressure);
    Serial.printf("gas:%d\n", bme.gas_resistance);
  }
}