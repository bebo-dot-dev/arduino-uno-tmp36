#include <avr/wdt.h>

float lastTemp = 0.0;
String lastTempStr = "";

void setup() {
  //enable the watchdog for 1s
  wdt_enable(WDTO_1S);
  
  // initialize serial communications at 115200 bps:
  Serial.begin(115200);
  Serial.println("Start up");
}

void loop() {

  double VccMv = readVcc(); //read the ref Vcc
  double Vcc = VccMv / 1000;

  float celcius = readTmp36SensorTemp(Vcc); //read tmp36 temperature
  float farenheit = (celcius * (9.0/5.0)) + 32;
  
  serialOut(celcius, farenheit); //log to serial output

  //happy path - reset the watchdog and keep running
  wdt_reset();
  
  delay(1000);
}

String floatToString(float number, int stringWidth, int numVarsAfterDecimal) {

  char outstr[6];
  dtostrf(number, stringWidth, numVarsAfterDecimal, outstr);
  String result = String(outstr);
  //free(outstr);
  return result;

}

bool stringsEqual(char* char1, char* char2) {

  String str1 = String(char1);
  String str2 = String(char2);

  return str1.equals(str2);

}

int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void serialOut(float currentTemp, float farenheit){

  //convert the temperature float value to a string representation of the floating point number
  //with the specified number of precision digits after the decimal point
  String currentTempStr = floatToString(currentTemp, 5, 1); 
  
  bool tempChange = !(currentTempStr.equals(lastTempStr)); //temp change detection
  
  if (tempChange) {
    String farenheitStr = floatToString(farenheit, 5, 1);
    
    Serial.print("Temp:");
    Serial.print(currentTempStr);
    
    if (lastTemp != 0.0) {
      
      Serial.print("°C ");

      Serial.print(farenheitStr);
      Serial.print("°F");

      String directionStr = currentTemp > lastTemp ? " ▲ " : " ▼ ";
      Serial.print(directionStr);
      Serial.print("(was");
      Serial.print(lastTempStr);
      Serial.println("°C)");
    } else {
      Serial.print("°C ");
      Serial.print(farenheitStr);
      Serial.println("°F");
    }

    lastTemp = currentTemp;
    lastTempStr = currentTempStr;
  }
}

