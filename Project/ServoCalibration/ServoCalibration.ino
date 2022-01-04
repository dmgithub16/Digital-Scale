
//Short sketch to fine tune servo positions

#include <Servo.h>
#include <Rotary.h>

//Rotary Encoder
int outputRgt = 10;
int outputLft = 11;
float rotary = 0;
int rgtState;
int rgtLastState;
int lftState;
int lftLastState;

Servo servo;

float servoPin = 9;

void setup() {
  //begin serial
  Serial.begin(9600);
//rotary setup
pinMode(outputRgt, INPUT);
pinMode(outputLft, INPUT);

rgtLastState = digitalRead(outputRgt);

  servo.attach(servoPin);
  servo.write(0);

}
void loop() {
  
rgtState = digitalRead(outputRgt);
if(rgtState != rgtLastState) {
  if(digitalRead(outputLft) != rgtState) {
    rotary--;
    } 
    else {
      rotary++;
      }
      Serial.print("Rotary:");
      Serial.println(rotary);
      servo.write(rotary);
      
  }    
  rgtLastState = rgtState;

  }
