#include <IRremote.h>

//Pinbelegung
const int RECV_PIN = 11; // Pin to which IR receiver is connected
const int relaisPin = 9;

//globale Variablen
String readCode = "";

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  pinMode(relaisPin,OUTPUT);
  digitalWrite(relaisPin,LOW);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("is enabled");
}

void loop()
{
  irrecv.decode();
 
  if (irrecv.decodedIRData.decodedRawData !=0)
  {
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX); // Print the received code in hexadecimal format
    readCode=(irrecv.decodedIRData.decodedRawData);
    irrecv.decodedIRData.decodedRawData = 0;
    delay(1000);
  }
  if (readCode!="")
  {
    Serial.println("Code gelesen:" + readCode);  
    //readCode = "";
    analyseCode(readCode);
  }  



irrecv.resume(); // Receive the next code
      delay(100);
    readCode="";
}



void analyseCode(String Code)
{
    
  if (strcmp(Code.c_str(),"2473330439") == 0)
  {
    Serial.println("turnonpc");
      turnOnPc();
  }
}

void turnOnPc()
{
  digitalWrite(relaisPin, HIGH);
    delay(500);
    digitalWrite(relaisPin, LOW);
}