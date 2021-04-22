#include <HX711.h>


HX711 scale;

const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

}

void loop() {
  // put your main code here, to run repeatedly:


  if (scale.is_ready()) {
    long reading = scale.read();
//    Serial.print("HX711 reading: ");
    Serial.println(String(reading));
  } else {
    Serial.println("HX711 not found.");
  }

  delay(1000);
  
//float data = analogRead(A0);
//data = (data/1024.00) * 5;
//String dataToSend = String(data);
//Serial.println(dataToSend);
//delay(300);
}
