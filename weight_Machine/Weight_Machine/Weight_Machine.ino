#include "HX711.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2

HX711 scale;

float calibration_factor = 110000; //-7050 worked for my 440lb max scale setup

void setup() {
  lcd.begin();


  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Weight Machine");
  lcd.setCursor(0,1);
  lcd.print("Insert weight");
  lcd.clear();
  Serial.begin(9600);

  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  //scale.tare(); //Reset the scale to 0
  
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
  
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  float kilo= scale.get_units()*0.453592 - 0.359;
  Serial.print("Reading: ");
  Serial.print(kilo, 3);
  //Serial.print(scale.get_units(), 1);
  Serial.print(" kg "); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  if(kilo<= 0.150 && kilo >= 0.010)
  {
  
  lcd.setCursor(0,0);
  lcd.print("Weight:");
  lcd.setCursor(8,0);
  lcd.print(kilo);
  lcd.setCursor(14,0);
  lcd.print("kg");
    
    lcd.setCursor(0,1);
    lcd.print("Out Of Stock");
    
    digitalWrite(5,HIGH);
    
  }else{

  digitalWrite(5,LOW);
  
  lcd.setCursor(0,1);  
  lcd.print("            ");
  lcd.setCursor(0,0);
  lcd.print("Weight:");
  lcd.setCursor(8,0);
  lcd.print(kilo);
  lcd.setCursor(14,0);
  lcd.print("kg");
  }

  
}
