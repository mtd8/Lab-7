#include "arduino_secrets.h"
#include "thingProperties.h"

const int sensor_pin = 0;
int pulse_signal = 0;
int pulse_period = 0;
//float BPM = 0;

//  initialize flags
bool any_peak_detected = false;
bool first_peak_detected = false;
unsigned long first_pulse_time = 0;
unsigned long second_pulse_time = 0;
int upper_threshold = 800;
int lower_threshold = 200;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();  

  while (ArduinoCloud.connected() != 1) {
    ArduinoCloud.update();
    delay(500);
  }

}

void loop() { 

  ArduinoCloud.update();
  pulse_signal = analogRead(sensor_pin);
  //Serial.println(pulse_signal);
  delay(100);
  //Serial.println("first peak ");
  //Serial.println(first_peak_detected);
  //Serial.println("any peak ");
  //Serial.println(any_peak_detected);
  if (pulse_signal > upper_threshold && any_peak_detected == false) {
    any_peak_detected = true;
    if (first_peak_detected == false) {
      first_pulse_time = millis();
      first_peak_detected = true;
    }
    else {
      second_pulse_time = millis();
      pulse_period = second_pulse_time - first_pulse_time;
      bPM = 60000/pulse_period;
      Serial.println(BPM);
      first_peak_detected = false;
    }
  }
  if (pulse_signal < lower_threshold) {
    any_peak_detected = false;
  }

    counter++;
  if (counter > 200){
    ArduinoCloud.update();
    Serial.println(bPM);
    counter = 0;
  }

}
