#include <Arduino.h>
#include <QMP6988.h>
#include <SHT3X.h>

SHT3X sht3x;
QMP6988 qmp;

void setup() {
  Serial.begin(115200);

  if (!qmp.begin(&Wire, QMP6988_SLAVE_ADDRESS_L, SDA, SCL, 400000U)) {
    Serial.println("Couldn't find QMP6988");
    while (1) delay(1);
  }

  if (!sht3x.begin(&Wire, SHT3X_I2C_ADDR, SDA, SCL, 400000U)) {
    Serial.println("Couldn't find SHT3X");
    while (1) delay(1);
  }
}

uint8_t sampling_rate_Hz = 2;

uint32_t prev_millis = 0;

void loop() {
  if ((millis() - prev_millis) >= (1000 / sampling_rate_Hz)) {
    prev_millis = millis();
    if (sht3x.update() && qmp.update()) {
      Serial.printf("[sht3x] temp:%.2f, hum:%.2f\t", sht3x.cTemp, sht3x.humidity);
      Serial.printf("[qmp6988] temp:%.2f, pressure:%.2f\n", qmp.cTemp, qmp.pressure);
    } else {
      Serial.printf("Couldn't read sensor data\n");
    }
  }
}