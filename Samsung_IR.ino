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

    //NEC IR Code
    if (irrecv.decodedIRData.protocol == SAMSUNG)
    {
        printCommand();
    }

    //using RawData to use for easy matching
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


//matches RawCode
void analyseCode(String code)
{
  if (code=="2473330439")
    { //red button
        Serial.println("turnonpc");
        turnOnPc();
    }
  else if(code =="3910534919") // blue button
    {
      IRData CodeToSend; //simulates power button push
      CodeToSend.protocol = SAMSUNG;
      CodeToSend.address = 0x707;
      CodeToSend.command = 0xD ;
      CodeToSend.numberOfBits = 32;
      
      sendCode(CodeToSend);
    }
    else
    {
    printBlankCode(code);
    }
}

void sendCode(IRData data)
{
  irsend.sendSamsung(data.address, data.command, 1);
}


void turnOnPc() // Function to start by button push
{
  digitalWrite(relaisPin, HIGH);
    delay(500);
    digitalWrite(relaisPin, LOW);
}

//used to print commands
void printCommand()
{
  irrecv.printIRResultShort(&Serial);
}

void printBlankCode (String Code)
{
  Serial.println("no match found: Add this to analyseCode():");
  Serial.println("else if(code ==\"" + String(Code) + "\"){");
  Serial.println("//Add your code here");
  Serial.println("}");
}
