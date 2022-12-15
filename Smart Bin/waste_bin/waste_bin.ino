#include "qrcode.h"
 #include <TFT.h>  // Arduino LCD library
 #include <SPI.h>
 #include "HX711.h"
/**********************
| SCREEN              |
**********************/

    #define cs   10
    #define dc   8
    #define rst  9
    TFT TFTscreen = TFT(cs, dc, rst);

int redpin = A5;
int greenpin= A4;
/**********************
| DS3231              |
**********************/
    String weight;
    String location;
    String weight_inserted;
    String strdistance;
    String concatenate;
    String strTime;
    String totalweight;
    char dateCharArray[100]; // dd.mm.yyyy
    //char timeCharArray[7]; // xx:xx:xx



#define LOADCELL_DOUT_PIN  A0
#define LOADCELL_SCK_PIN  A1
#define LOADCELL_DOUT_2PIN  A2
#define LOADCELL_SCK_2PIN  A3

HX711 scale,scale1;

float calibration_factor = 110000; //-7050 worked for my 440lb max scale setup
float calibration_factor1 = 25000; //-7050 worked for my 440lb max scale setup
float calibration2_1= 110000;
//60000;
float calibration2_2 = 25000;
//40000;

/*float calibration_factor0 = 110000; //-7050 worked for my 440lb max scale setup
float calibration_factor01 = 25000; //-7050 worked for my 440lb max scale setup
float calibration02_01= 110000;
float calibration02_02 = -25000;*/


float nw;


// defines pins numbers
const int trigPin = 5;
const int echoPin = 6;
int interrupt = 2;
// defines variables
int duration;
float distance;
float store = 0;
float wi;

int flag=0;


/**********************
| QR Code             |
**********************/

    QRCode qrcode;





void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(interrupt , INPUT_PULLUP);
//attachInterrupt(digitalPinToInterrupt(interrupt),operation,FALLING);
pinMode(redpin,OUTPUT);
pinMode(greenpin,OUTPUT);
Serial.begin(9600);



  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");


  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  //scale.tare();


   scale1.begin( LOADCELL_DOUT_2PIN , LOADCELL_SCK_2PIN);
   scale1.set_scale();
   //scale1.tare();

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);









    /**********************
    | SCREEN              |
    **********************/
        TFTscreen.begin();
        // Rotate the screen
        TFTscreen.setRotation(2);
        // clear the screen with a white background
        TFTscreen.background(255,255,255);
        TFTscreen.setTextSize(1);
        TFTscreen.stroke(0,0,0);
        TFTscreen.fill(0,0,0);
        }




void loop() {

  if(digitalRead(interrupt)== 0)
  {
    operation();
  }

  if(flag==1)
  {
 strTime = ",";
strdistance = String(distance);
weight_inserted = String(store);
location = "Gulistan-e-Jauhar ";
totalweight = String(nw);
concatenate = strdistance + strTime + weight_inserted + strTime + totalweight + strTime + location;
 concatenate.toCharArray(dateCharArray, 100);


digitalWrite(greenpin,LOW);
digitalWrite(redpin,HIGH);
  /**********************
    | QR Code             |
    **********************/

    TFTscreen.background(255,255,255);
    TFTscreen.setTextSize(1);
    TFTscreen.stroke(0,0,0);
    TFTscreen.fill(0,0,0);


        // Print First Time
        uint8_t qrcodeData[qrcode_getBufferSize(4)];
        qrcode_initText(&qrcode, qrcodeData, 4, 0, dateCharArray);// parameter, e.g. "Hello World"
        for (uint8_t y = 0; y < qrcode.size; y++) {
            // Each horizontal module
            for (uint8_t x = 0; x < qrcode.size; x++) {
                if(qrcode_getModule(&qrcode, x, y)) {
                    TFTscreen.rect(5+(x*3), 5+(y*4), 3, 4);
                }
            }
        } 

    delay(10000);
    flag=0;
  }

}

void operation(){

  digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
Serial.print("Distance: ");
Serial.println(distance);




 scale.set_gain(32);
 scale.set_scale(calibration_factor); //Adjust to this calibration factor
  float kilo= scale.get_units()*0.453592+ 0.071;
  Serial.print("Reading: ");
  Serial.print(kilo ,3);
  Serial.print(" Kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

   scale.set_gain(128);
 scale.set_scale(calibration_factor1); //Adjust to this calibration factor
  float kilo1= scale.get_units()*0.453592 + 0.192;
  Serial.print("Reading: ");
  Serial.print(kilo1 ,3);
  Serial.print(" Kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor1: ");
  Serial.print(calibration_factor1);
  Serial.println();


 scale1.set_gain(32);
  scale1.set_scale(calibration2_1); //Adjust to this calibration factor
  float kilo2= scale1.get_units()*0.453592- 0.789;
  Serial.print("Reading: ");
  Serial.print(kilo2 ,3);
  Serial.print(" Kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration2_1: ");
  Serial.print(calibration2_1);
  Serial.println();


  scale1.set_gain(128);
  scale1.set_scale(calibration2_2); //Adjust to this calibration factor
  float kilo3= scale1.get_units()*0.453592 - 0.982;
  Serial.print("Reading: ");
  Serial.print(kilo3 ,3);
  Serial.print(" Kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration2_2 :");
  Serial.print(calibration2_2);
  Serial.println();

 nw =  (kilo + kilo1 + kilo2 + kilo3 - 8.710);
Serial.println("nw");
Serial.println(nw, 3);

store = nw - store;
//Serial.println("W.I =");
//Serial.print(store);







    TFTscreen.background(0,0,0);
    TFTscreen.setTextSize(1);
    TFTscreen.stroke(0,0,0);
    TFTscreen.fill(0,0,0);

    flag=1;

    //break;
}
