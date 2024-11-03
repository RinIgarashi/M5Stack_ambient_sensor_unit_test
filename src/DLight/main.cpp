#include <Arduino.h>
#include <M5_DLight.h>

M5_DLight sensor;

void setup() {
  Serial.begin(115200);

  sensor.begin();
  sensor.setMode(CONTINUOUSLY_H_RESOLUTION_MODE);
}

uint8_t sampling_rate_Hz = 2;

uint32_t prev_millis = 0;

void loop() {
  if ((millis() - prev_millis) >= (1000 / sampling_rate_Hz)) {
    prev_millis = millis();
    uint16_t lux = sensor.getLUX();
    Serial.printf("lux: %d\n", lux);
  }
}