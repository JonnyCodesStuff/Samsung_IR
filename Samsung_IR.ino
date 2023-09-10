#include <IRremote.h>

//Pin Layout
int RECV_PIN = A2; // Pin to which IR receiver is connected
int SEND_PIN = A3;
int relaisPin = 9;

//global
String readCode = "";
IRrecv irrecv(RECV_PIN);
IRsend irsend(SEND_PIN);
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
    //Serial.println(irrecv.decodedIRData.decodedRawData, HEX); // Print the received code in hexadecimal format
    readCode=(irrecv.decodedIRData.decodedRawData);
    irrecv.decodedIRData.decodedRawData = 0;
    delay(1000); // used to eliminate multiple button pushes, might need to adjust if button needs to be held.
  }
  if (readCode!="")
  {
    analyseCode(readCode);
  }  



irrecv.resume(); // Receive the next code
      delay(100);
    readCode="";
}



void analyseCode(String code)
{
  if (code=="2473330439")
    { //red button
        Serial.println("turnonpc");
        turnOnPc();
    }
  else if(code =="3910534919") // blue button
    {
      unsigned long NecPowerButton = 0x4244768519;
      sendCode("NecPowerButton"); //power button
    }
    else
    {
    printBlankCode(code);
    }
}

void sendCode(unsigned long codeToSend)
{
  irsend.sendNEC(codeToSend, 32);
}


void turnOnPc() // Function to start by button push
{
  digitalWrite(relaisPin, HIGH);
    delay(500);
    digitalWrite(relaisPin, LOW);
}

void printBlankCode (String Code)
{
  Serial.println("no match found: Add this to analyseCode():");
  Serial.println("else if(code ==\"" + String(Code) + "\"){");
  Serial.println("//Add your code here");
  Serial.println("}");
}
