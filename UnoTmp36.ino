float filteredTemperature = 0.0;

float readTmp36SensorTemp(float Vcc) {

  //throw away the 1st reading
  int firstReading = analogRead(0);
  int pinReading = 0;

  //average over 30 readings to stabilize the ADC and filter noise
  for (int i = 1; i <= 30; i++) {
    pinReading += analogRead(0);
  }
  pinReading /= 30.0;

  //calc to degc
  float pinVoltage = pinReading * Vcc / 1024;
  float tempDegC = (pinVoltage - 0.5) * 100;

  //further filtering
  if ( abs(filteredTemperature - tempDegC) > 0.5 ) {
    // Seed the filter if realtime value has changed by a large amount
    // If you don't seed the filter, it will take a while
    // to get to the correct value.
    filteredTemperature = tempDegC;
  }
  else
    filteredTemperature = filteredTemperature - (filteredTemperature - tempDegC) * 0.05;

  return filteredTemperature;
}

