#include "SPI.h"
#include "MFRC522.h"
#include <Servo.h>


#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN A0
#define LED_PIN2 A2

Servo servo;
int angle = 0 ;

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup() {
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();
    pinMode(LED_PIN, OUTPUT);
    servo.attach(8);
    servo.write(angle);
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();

  Serial.print("Tap card key: ");
  Serial.println(strID);

  // Code to control the LED by removing the variable strID compared
  if (strID.indexOf("80:C6:79:7A") >= 0) {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);                  
    digitalWrite(LED_PIN, LOW);                                
    // scan from 0 to 180 degrees
  for(angle = 0; angle < 110; angle++)  
  {                                  
    servo.write(angle);               
    delay(15);                   
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 110; angle > 0; angle--)    
  {                                
    servo.write(angle);           
    delay(200);       
  } 
  } else {
    digitalWrite(LED_PIN2, HIGH);
    delay(1000);                  
    digitalWrite(LED_PIN2, LOW);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
