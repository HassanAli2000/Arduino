#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN 10//RX slave select
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.



int const led=7 ;
int const buzz=6 ;

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  
  Serial.println("CLEARSHEET");                 // clears starting at row 1
  Serial.println("LABEL,Date,Time,content");// make four columns (Date,Time,[Name:"user name"]line 48 & 52,[Number:"user number"]line 49 & 53)

  pinMode(led,OUTPUT);
  pinMode(buzz,OUTPUT);
   }

    
void loop() {
  
  //look for new card
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
  return;//got to start of loop if there is no card present
 }
 // Select one of the cards
 if ( ! mfrc522.PICC_ReadCardSerial()) {
  return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
 }
 
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
   
  Serial.println();
  content.toUpperCase();
  Serial.println("DATA,DATE,TIME," +content);//send the Name to excel
  digitalWrite(led,HIGH);
  tone(buzz,650);
  delay(250);
  noTone(buzz);
  delay(1000);
  digitalWrite(led,LOW);
  
  
      delay(2000);
  }

    
