#include <HX711.h>


HX711 scale;

const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 2;

//#define DOUT 
#define calibration_factor 10000

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
scale.set_scale(10000);
scale.tare();

}

void loop() {
  // put your main code here, to run repeatedly:


//  if (scale.is_ready()) {
    float reading = scale.get_units();
//    Serial.print("HX711 reading: ");
    Serial.println(reading);
//    Serial.println(String(69));
//  } else {
//    Serial.println("HX711 not found.");
//  }

  delay(1000);
  
//float data = analogRead(A0);
//data = (data/1024.00) * 5;
//String dataToSend = String(data);
//Serial.println(dataToSend);
//delay(300);
}
