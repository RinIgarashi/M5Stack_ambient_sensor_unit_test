#include <Arduino.h>
#include <bsec2.h>

/**
 * @brief : This function checks the BSEC status, prints the respective error
 * code. Halts in case of error
 * @param[in] bsec  : Bsec2 class object
 */
void checkBsecStatus(Bsec2 bsec);

/**
 * @brief : This function is called by the BSEC library when a new output is
 * available
 * @param[in] input     : BME68X sensor data before processing
 * @param[in] outputs   : Processed BSEC BSEC output data
 * @param[in] bsec      : Instance of BSEC2 calling the callback
 */
void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec);

/* Create an object of the class Bsec2 */
Bsec2 envSensor;

/* Entry point for the example */
void setup(void) {
  /* Desired subscription list of BSEC2 outputs */
  bsecSensor sensorList[] = {BSEC_OUTPUT_IAQ,     BSEC_OUTPUT_RAW_TEMPERATURE,      BSEC_OUTPUT_RAW_PRESSURE, BSEC_OUTPUT_RAW_HUMIDITY,
                             BSEC_OUTPUT_RAW_GAS, BSEC_OUTPUT_STABILIZATION_STATUS, BSEC_OUTPUT_RUN_IN_STATUS};

  /* Initialize the communication interfaces */
  Serial.begin(115200);
  Wire.begin(SDA, SCL);

  /* Valid for boards with USB-COM. Wait until the port is open */
  while (!Serial) delay(10);

  /* Initialize the library and interfaces */
  if (!envSensor.begin(BME68X_I2C_ADDR_HIGH, Wire)) {
    checkBsecStatus(envSensor);
  }

  /* Subsribe to the desired BSEC2 outputs */
  if (!envSensor.updateSubscription(sensorList, ARRAY_LEN(sensorList), BSEC_SAMPLE_RATE_LP)) {
    checkBsecStatus(envSensor);
  }

  /* Whenever new data is available call the newDataCallback function */
  envSensor.attachCallback(newDataCallback);

  Serial.printf("BSEC library version %d.%d.%d.%d\n", envSensor.version.major, envSensor.version.minor, envSensor.version.major_bugfix, envSensor.version.minor_bugfix);
}

/* Function that is looped forever */
void loop(void) {
  /* Call the run function often so that the library can
   * check if it is time to read new data from the sensor
   * and process it.
   */
  if (!envSensor.run()) {
    checkBsecStatus(envSensor);
  }
}

void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec) {
  if (!outputs.nOutputs) {
    return;
  }

  Serial.printf("\ntimestamp = %lld\n", outputs.output[0].time_stamp / INT64_C(1000000));
  for (uint8_t i = 0; i < outputs.nOutputs; i++) {
    const bsecData output = outputs.output[i];
    switch (output.sensor_id) {
      case BSEC_OUTPUT_IAQ:
        Serial.printf("irq = %.1f\n", output.signal);
        Serial.printf("iaq accuracy = %d\n", output.accuracy);
        break;
      case BSEC_OUTPUT_RAW_TEMPERATURE:
        Serial.printf("temperature = %.2f\n", output.signal);
        break;
      case BSEC_OUTPUT_RAW_PRESSURE:
        Serial.printf("pressure = %.1f\n", output.signal);
        break;
      case BSEC_OUTPUT_RAW_HUMIDITY:
        Serial.printf("humidity = %.2f\n", output.signal);
        break;
      case BSEC_OUTPUT_RAW_GAS:
        Serial.printf("gas resistance = %.1f\n", output.signal);
        break;
      case BSEC_OUTPUT_STABILIZATION_STATUS:
        Serial.printf("stabilization status = %.1f\n", output.signal);
        break;
      case BSEC_OUTPUT_RUN_IN_STATUS:
        Serial.printf("run in status = %.1f\n", output.signal);
        break;
      default:
        break;
    }
  }
}

void checkBsecStatus(Bsec2 bsec) {
  if (bsec.status < BSEC_OK) {
    Serial.printf("BSEC error code : %d\n", bsec.status);

  } else if (bsec.status > BSEC_OK) {
    Serial.printf("BSEC warning code : %d\n", bsec.status);
  }

  if (bsec.sensor.status < BME68X_OK) {
    Serial.printf("BME68X error code : %d\n", bsec.sensor.status);
  } else if (bsec.sensor.status > BME68X_OK) {
    Serial.printf("BME68X warning code : %d\n", bsec.sensor.status);
  }
}