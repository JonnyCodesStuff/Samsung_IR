//Pin Layout
int RECV_PIN = A2; // Pin to which IR receiver is connected
int SEND_PIN = A3;
int relaisPin = 9;

// IR Setup
#include <IRremote.h>
String readCode = "";
IRrecv irrecv(RECV_PIN);
IRsend irsend(SEND_PIN);

void setup(){
  pinMode(LED_BUILTIN, OUTPUT); // used to indicate if IR is enabled
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(relaisPin,OUTPUT);
  digitalWrite(relaisPin,LOW);
  Serial.begin(115200);  
  
  irrecv.enableIRIn(); // Start the IR receiver
  digitalWrite(LED_BUILTIN, HIGH);  
  Serial.println("is enabled");
}

void loop(){
  receiveCode();
}

// Regards IR Signal Handling
  void receiveCode(){
    irrecv.decode();
    if (irrecv.decodedIRData.decodedRawData !=0)
    {
      // Samsung IR Code
      if (irrecv.decodedIRData.protocol == SAMSUNG)
      {
        printCommand();
      }
      // Getting RawData to use for easy matching
      readCode=(irrecv.decodedIRData.decodedRawData);
      Serial.println("RawCode:" + readCode);
      irrecv.decodedIRData.decodedRawData = 0;
      delay(1000); // Used to eliminate multiple button pushes, might need adjustment if button needs to be held.
    }
    if (readCode!=""){
      analyseCode(readCode);
    }  
    irrecv.resume(); // Receive the next code
    delay(100);
    readCode="";
  }

  // Matches RawCode
  void analyseCode(String code){
    if (code=="2473330439") // red button
    {
      turnOnPc();
    }
    else if(code =="3910534919") // blue button
    {
      turnOnTV(); 
    }
    else
    {
      printBlankCode(code);
    }
  }

  void sendCode(IRData data){
    irsend.sendSamsung(data.address, data.command, 1); //Samsung Code Only here
  }

  void printCommand(){
    irrecv.printIRResultShort(&Serial); // This will get you the Commands if you want to analyse and copy a IR Signal

  }

  void printBlankCode (String Code){
    Serial.println("no match found: Add this to analyseCode():");
    Serial.println("else if(code ==\"" + String(Code) + "\"){");
    Serial.println("//Add your code here");
    Serial.println("}");
  }


// Example UseCases
  void turnOnPc(){ //simulates button push
    Serial.println("Turning on PC");
    digitalWrite(relaisPin, HIGH);
    delay(500);
    digitalWrite(relaisPin, LOW);
  }

  void turnOnTV(){ //simulates power button push on remote
    Serial.println("Turning on the TV");
    IRData CodeToSend; 
    CodeToSend.protocol = SAMSUNG;
    CodeToSend.address = 0x707;
    CodeToSend.command = 0x2 ;
    CodeToSend.numberOfBits = 32;
            
    sendCode(CodeToSend);
  }