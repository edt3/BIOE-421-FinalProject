#include <HX711.h>


HX711 scale;

const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 2;

//#define DOUT 
#define calibration_factor 190850
#define wait_time 60000 // in milliseconds

unsigned long prevtime;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println("Do not place anything on load cell until you see 'READY'");
delay(10);
scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
scale.set_scale(calibration_factor);
scale.tare();
delay(10);
Serial.println("READY");
delay(5000);
Serial.println("Press Ctrl + C in your python IDE to quit the program (Windows)"); // look up how to trigger a KeyboardInterrupt exception on your device!

}

void loop() {
  // put your main code here, to run repeatedly:


  while (millis() - prevtime < wait_time);
  prevtime = millis();
//  if (scale.is_ready()) {
    float reading = scale.get_units();
//    Serial.print("HX711 reading: ");
    Serial.println(reading, 3); // second input is number of decimal points with which to represent the float reading
//    Serial.println(String(69));
//  } else {
//    Serial.println("HX711 not found.");
//  }

//  delay(60000);
  
//float data = analogRead(A0);
//data = (data/1024.00) * 5;
//String dataToSend = String(data);
//Serial.println(dataToSend);
//delay(300);
}

/*
 * Experiment notes: 04/27
 * starting with .095lb (43g) of ipa
 * time increment of 1 minute
 * 
 * Experiment 2: 04/28
 * starting with .101lb (46g) of ipa, but measured 38g on a scale
 * time increment of 1 minute
 * 
 * 
 */
