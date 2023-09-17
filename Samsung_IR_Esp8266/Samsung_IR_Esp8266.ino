//Wifi Parameters
#include <ESP8266WiFi.h>
const char* ssid = "WifiSSID"; //add your SSID here
const char* password = "WifiPW"; //add your Wifi Password here
const int wifiTryLimit = 50; // Limits wait time for Wifi Seconds. Higher Value = longer boottime on a failed connection

// IR Setup
#include <IRremote.h>
//Pin Layout
uint8_t RECV_PIN = D5;
uint8_t SEND_PIN = D6;
uint8_t relaisPin = D7;

//IR global
String readCode = "";
IRrecv irrecv(RECV_PIN);
IRsend irsend(SEND_PIN);

void setup() {
  Serial.begin(115200);
  delay(10);
  connectToWifi();

}

void loop() {
  receiveCode();
}

//Regards Wifi Connection
  void connectToWifi(){
    WiFi.begin(ssid, password);
    Serial.print("Commecting");
    int i=0;
    while (WiFi.status() != WL_CONNECTED && i<wifiTryLimit) {
      i++;
      delay(1000);
      Serial.print(".");
      Serial.println("not yet connected");
        if(i==100)
    {
      Serial.println("Connection failed");
    }
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("");
      Serial.println("Connected to WiFi");
      Serial.println(WiFi.localIP());
    }
  }





//Regards IR Signal Handling
  void receiveCode(){
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

  void sendCode(IRData data){
    irsend.sendSamsung(data.address, data.command, 1);
  }


  //matches RawCode
  void analyseCode(String code){
    if (code=="2473330439") // red button
      { 
          Serial.println("turnonpc");
          turnOnPc();
      }
    else if(code =="3910534919") // blue button
      {
        IRData CodeToSend; // simulates power button push
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



  //used to print commands
  void printCommand(){
    irrecv.printIRResultShort(&Serial);
  }

  void printBlankCode (String Code){
    Serial.println("no match found: Add this to analyseCode():");
    Serial.println("else if(code ==\"" + String(Code) + "\"){");
    Serial.println("//Add your code here");
    Serial.println("}");
  }


// Example UseCases
  void turnOnPc(){ // Function to start by button push
  
    digitalWrite(relaisPin, HIGH);
      delay(500);
      digitalWrite(relaisPin, LOW);
}



