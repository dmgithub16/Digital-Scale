/*
 * Modified Program to get calibration factor for HX711
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Arduino pin 6 -> HX711 CLK
 Arduino pin 5 -> HX711 DOUT
 Arduino pin 5V -> HX711 VCC
 Arduino pin GND -> HX711 GND 
*/

#include "HX711.h"

HX711 scale(5, 6);

float calibration_factor = -359.5; // this calibration factor is adjusted according to my load cell
float units;
float ounces;

float temp_cal = calibration_factor;
float temp_units = 10000.1012;  

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {


  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  
  //Serial.print("Reading: ");
  units = scale.get_units(5), 2;
    if(units < 0) {
    units = 0.0;
    }
  if(temp_units == 10000.1012)
  {
    temp_units = units;
    }

  ounces = units * 0.035274;
  //Serial.print(units);
  //Serial.print(" grams ");
  if(units > 5.6) {
    calibration_factor = calibration_factor - .001;  
    if(units < temp_units)
        {
        temp_cal = calibration_factor;
        temp_units = units;  
        }     
    }
  else if (units < 5.6) {
    calibration_factor = calibration_factor + .001;
    if(units > temp_units)
      {
        temp_cal = calibration_factor;
        temp_units = units;  
      }
    }

  //Serial.print("CF: ");
  //Serial.print(calibration_factor);
  Serial.println();
  Serial.print("X: ");
  Serial.print(temp_cal);
  Serial.print("Y: ");
  Serial.print(temp_units);
  Serial.println();
  

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 1;
  }
}


float doAverage() {
 
  
  }
