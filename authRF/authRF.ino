#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#define SS_PIN 10
#define RST_PIN 5
#define RELAY 3 //relay pin
#define BUZZER 2 //buzzer pin

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup()
{
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);  // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();  // Initiate MFRC522
  pinMode(RELAY,OUTPUT);
  pinMode(BUZZER,OUTPUT);
  noTone(BUZZER);
  digitalWrite(RELAY,LOW);
  Serial.println("Approximate your card to the reader...");
  Serial.println();

}
void loop()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "D3 02 86 11") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    lcd.clear();
    lcd.print("Authorized access");
    digitalWrite(RELAY,HIGH);
    
    tone(BUZZER,300);
    delay(5000);
    digitalWrite(RELAY,LOW);
    noTone(BUZZER);
    Serial.println();
    delay(3000);
  }

else  {
    Serial.println(" Access denied");
    lcd.clear();
    lcd.print("Access denied");
    delay(3000);
  }
}
