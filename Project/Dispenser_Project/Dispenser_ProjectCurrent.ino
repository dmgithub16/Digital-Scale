//CS467 ES Project Weight Controlled Dispenser

#include <Rotary.h>
#include <HX711.h>
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Button.h>

//Rotary
Rotary rotary = Rotary(10,11);
//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
//Buttons
Button toggleButton = Button(4, HIGH);
Button enterButton = Button(5, HIGH);
//Servo
Servo servo;
int servoPin = 9;
int openPos = 60;
int closePos = 0;
bool allRun = false;
//Scale
HX711 scale(6, 7);
float calFactor = -359.50;
float value = 0;
float valueTo = 0;
float tolerance = 0.0;//This isn't really necessary, the weights come out more accurate on average at 0.0

void setup() {
  //LCD setup
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("Scale Starting   ");   
  //attach servo controls
  servo.attach(servoPin);
  servo.write(closePos);// Close the dispenser on startup, prevents messes
  //start scale and tare
  scale.set_scale();
  scale.tare();  //Reset the scale to 0
  delay(500);//Make sure everyone is started
  //Notify User
  lcd.println("Scale Ready");
  updateMessageToUser();
}
void loop() {
//Attach event listener to buttons  
toggleButton.listen();
enterButton.listen();
//Check rotary switch for changes
unsigned char result = rotary.process();
  if (result == DIR_CW) {
    //increment value to weigh to
    valueTo++;
    updateMessageToUser();
  } else if (result == DIR_CCW && valueTo >= 1) {
    //decrement value to weigh to by .50
    valueTo = valueTo - 0.50;
    updateMessageToUser();
  }
//Enter button event handler  
if(enterButton.isPressed() && valueTo > 0) { // The second test is when no value has been set, so just skip opening the dispenser
  while(value < (valueTo - tolerance)) {  //While the value (weight on the scale) is less than the valueTo (weight requested by user),
          servo.write(openPos); //open the dispenser
          weigh(); //Keep checking the scale
          allRun = true; //mark the dispenser open for the toggle 
          //Notify user
          lcd.setCursor(0,1);
          lcd.print("Dispensing");    
      }
      //clear user notification
      servo.write(closePos); 
      lcd.setCursor(0,1);
      lcd.print("              ");    
  }
  //Toggle button event handler, reverse whatever state the dispenser is in
  if(toggleButton.isPressed()) {
    if(allRun == false) {
    servo.write(openPos);
    allRun = true;
    valueTo = 999;
    }
    else {
      servo.write(closePos);
      allRun = false;
      valueTo = 0;
      }
      lcd.setCursor(0,1);
      lcd.print("              ");   
    }

}
//This is were the magic happens
void weigh() {
    scale.set_scale(calFactor);//set the calibration factor, very important
    if(scale.get_units() < 0) {
     value = 0; //If the scale is reporting a negative value, set it to zero
      }
      else {
        value = scale.get_units();//Here is were our value var is set by the scale
        }
        updateMessageToUser();
  }
//Here is were we update the user's LCD  
void updateMessageToUser() {
      lcd.clear();
    if(valueTo >= 999) {
      lcd.print(" Override ");     
    }
    else {
      lcd.print(valueTo);  
    }  
      lcd.print(" === ");
      lcd.print(value);
      lcd.print("g.");

}
