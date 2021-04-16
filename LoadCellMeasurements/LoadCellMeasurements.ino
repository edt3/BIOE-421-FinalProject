

/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This example demonstrates basic scale output. See the calibration sketch to get the calibration_factor for your
 specific load cell setup.

 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE

 The HX711 does one thing well: read load cells. The breakout board is compatible with any wheat-stone bridge
 based load cell which should allow a user to measure everything from a few grams to tens of tons.
 Arduino pin 2 -> HX711 CLK
 3 -> DAT
 5V -> VCC
 GND -> GND

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/


/*
 * 
 * change in percentage of water?
 * 
 * 
 * use the lights to display/track amount of water
 * don't have to have it tethered to my computer
 * 
 * 
 * think about PUCs
 * can use buttons to make it more controllable
 * 
 * 
 * LED to estimate how long ago you watered it
 * another that shows relative weight
 * 
 * 
 * Flow Chart!
 * 
 * 
 * python script to plot, append new data to it
 * - python can put time stamp on data point (instead of arduino)
 * 
 * 
 * idea: one button to start measuring, both buttons to stop and transition:
 * - have lights indicate how much water is left, relatively (store weight when transition happens)
 * 
 */


#include <HX711.h>
//#include "HX711.h"
#include <TimeLib.h>
#include <Adafruit_CircuitPlayground.h>



#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#define DOUT  3
#define CLK  2

HX711 scale;

#define fast_time 120000 // delay for 2 minutes
//#define meas_time 600000 // delay for 10 minutes
#define meas_time 2000 // for demo purposes, delay 2 seconds
unsigned long time_marker;

#define array_length 5
float weight_arr[array_length];
float temp_arr[array_length];
float light_arr[array_length];

float low_weight;
float high_weight;
float demo_weight_arr[array_length] = {8, 7.8, 7.6, 7.3, 7.02};
float demo_weight_arr2[array_length] = {7.9, 7.6, 7.4, 7.15, 7.1};

//bool start_flag;
//bool end_flag;
int iter;


float find_avg(float *arr){

  float sum = 0;
  for (int i = 0; i < array_length; i++){
    sum += arr[i];
  }

  return sum / array_length;
}


enum states{idle, measuring, processing, awaiting_input, displaying} state;
enum light_states{low, med_low, med, med_high, high} light_state;


void display_lights(){
  CircuitPlayground.clearPixels();
//  Serial.println("In display_lights");
  switch(light_state){
    case high:
      CircuitPlayground.setPixelColor(0, CircuitPlayground.colorWheel(0));
      CircuitPlayground.setPixelColor(1, CircuitPlayground.colorWheel(34));
      CircuitPlayground.setPixelColor(2, CircuitPlayground.colorWheel(51));
      CircuitPlayground.setPixelColor(3, CircuitPlayground.colorWheel(68));
      CircuitPlayground.setPixelColor(4, CircuitPlayground.colorWheel(85));
      break;
    case med_high:
      CircuitPlayground.setPixelColor(0, CircuitPlayground.colorWheel(0));
      CircuitPlayground.setPixelColor(1, CircuitPlayground.colorWheel(34));
      CircuitPlayground.setPixelColor(2, CircuitPlayground.colorWheel(51));
      CircuitPlayground.setPixelColor(3, CircuitPlayground.colorWheel(68));
      break;
    case med:
      CircuitPlayground.setPixelColor(0, CircuitPlayground.colorWheel(0));
      CircuitPlayground.setPixelColor(1, CircuitPlayground.colorWheel(34));
      CircuitPlayground.setPixelColor(2, CircuitPlayground.colorWheel(51));
      break;
    case med_low:
      CircuitPlayground.setPixelColor(0, CircuitPlayground.colorWheel(0));
      CircuitPlayground.setPixelColor(1, CircuitPlayground.colorWheel(34));
      break;
    case low:
      CircuitPlayground.setPixelColor(0, CircuitPlayground.colorWheel(0));
      break;
  }
}



void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("HX711 scale demo");
  CircuitPlayground.begin();

//  scale.begin(DOUT, CLK);
//  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
//  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

//  Serial.println("Readings:");

  state = idle;

  delay(5000);
  
}

void loop() {

// initializing local variables
float mass_lost;
float volume_lost;
float volume_rate;


if (state == idle){
  if (CircuitPlayground.leftButton() || CircuitPlayground.rightButton())
    state = measuring;

  Serial.println("Awaiting Input");
  delay(1000);
}

else if (state == measuring) {
  Serial.println("Taking Measurements...");
  weight_arr[iter] = demo_weight_arr[iter];
//  weight_arr[iter] = scale.get_units();
  temp_arr[iter] = CircuitPlayground.temperature();
  light_arr[iter++] = CircuitPlayground.lightSensor(); // could do light_arr[iter++]
//  iter++;

  if (iter >= array_length){
    iter = 0;
    state = processing;

//    for (int i = 0; i < array_length; i++){
//      Serial.println(weight_arr[i]);
//    }
//    delay(10000);
  }
  
  unsigned long temp_time = millis();
  while ((millis() - temp_time) < meas_time);
}

else if (state == processing){ // might not need this state at all
    mass_lost = weight_arr[0] - weight_arr[array_length - 1];
    volume_lost = mass_lost * 2.20462 / 0.997; // using density of water = .997 kg/L converted to lb/L
    volume_rate = volume_lost / (array_length * meas_time / 3.6e6); // rate in hours,using number of samples * sample reading time in hours

    Serial.print("Rate of water lost: "); Serial.print(volume_rate); Serial.println(" L/hr");
    state = awaiting_input;
}

else if (state == awaiting_input){
//  bool input2 = false;

  while (!CircuitPlayground.leftButton() && !CircuitPlayground.rightButton());
  low_weight = demo_weight_arr[array_length - 1];
  Serial.print("Low weight: "); Serial.println(low_weight);
  delay(500);
  while (!CircuitPlayground.leftButton() && !CircuitPlayground.rightButton());
  high_weight = demo_weight_arr[0];
  Serial.print("High weight: "); Serial.println(high_weight);
  state = displaying;

//  if (input2 && (CircuitPlayground.leftButton() || CircuitPlayground.rightButton())){
//    Serial.println("Received Input 2");
//    high_weight = scale.get_units();
//    state = displaying;
//  }
//  else if (CircuitPlayground.leftButton() || CircuitPlayground.rightButton()){
//    Serial.println("Received Input 1");
//  low_weight = scale.get_units();
//  bool input2 = true;
//  delay(200);
//  }
}

else if (state == displaying){
  float cur_val = demo_weight_arr[iter];
//  float cur_val = scale.get_units();
  Serial.print("Current Weight: "); Serial.println(cur_val);

  int mapped_val = (cur_val - low_weight) * 4 / (high_weight - low_weight) + 0;

//  long mapped_val = map(cur_val, low_weight, high_weight, 0, 4);

  switch(mapped_val){
    case 4:
      light_state = high;
      break;
    case 3:
      light_state = med_high;
      break;
    case 2:
      light_state = med;
      break;
    case 1:
      light_state = med_low;
      break;
    case 0:
      light_state = low;
      break;
    default:
      light_state = low;
      break;
  }

  if (iter < array_length - 1)
    iter++;
  display_lights();
  delay(2000);
}
}





/*
 * Function scrapyard:
 * functions i've written that i will no longer used are commented out here, in case i might want to look at them later
 */

//  Serial.print("Reading: ");
//  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
//  Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
//  Serial.println();


//bool find_derivative(float *buf, int buffer_length){
//
//  bool is_negative = true;
//
//  for (int iter = 0; iter < (buffer_length - 1); iter++){
//    if ((buf[iter+1] - buf[iter]) > 0)
//    is_negative = false;
//  }
//
//  return is_negative;
//}

// pointer version of find_derivative, aka me trying to be cool
//bool find_derivative(float *_buf, int buffer_length){ // i don't like this i feel like i should do indices. i guess i'd need to do offset with a pointer
//
//  bool is_negative = true;
//
//
//  for (*_buf; _buf < _buf + buffer_length; _buf++){  // in example they did ++_buf?
//    if ((*(_buf + 1) - *_buf) > 0)
//      is_negative = false;
//  }
//  
//  return is_negative;
//}



//void update_array(float *_buf, float value){
//
////  static int iter;
//
//    _buf[iter] = value;
////  _buf[iter++] = scale.get_units(); // somehow make sure it returns a float...
//
//}


// if find_derivative(measurements, array_length)
//    state = prep_phase;
//  else
//    state = measuring;
//  
//
//  if(state == prep_phase)){
//
//    /*
//     * 
//     * take measurements over a smaller period of time to determine if it should start taking measurements
//     * 
//     */
//  
//  }
//  else if (state == measuring) {
//
//    /*
//     * 
//     * take more? / longer measurements over a longer period of time to collect data
//     * 
//     * run it until either it gains weight, or
//     * 
//     */
//    
//    time_marker = millis();
//    while(millis() - time_marker < (fast_time));
//    update_array(measurements);
//  }
//  else {
//    
//  }
