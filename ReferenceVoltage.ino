const float measuredVcc = 4970.0; //in mV. Fed via USB and multimeter measured on the board 5v pin
const float measuredRef = 5046.0; //mV. This is the internally measured Vcc using 1.1 * 1023 * 1000 as the boardConst where the internal ref voltage is assumed to be exactly 1.1v. This is inaccurate since the internal ref voltage is actually 1.1v +/- 10% tolerance
const float adjustedInternalRefVcc = 1.1 * (measuredVcc / measuredRef); //This is the 1.1v internal ref voltage calibrated for this board using the measuredVcc against the measuredRef
const long boardConst = (long)(adjustedInternalRefVcc * 1023 * 1000); //the board specific calibrated const


double readVcc(void) {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both

  long result = (high << 8) | low;

  result = boardConst / result; // Calculate Vcc (in mV);
  return result; // Vcc millivolts
}
