//Pin Layout
uint8_t RECV_PIN = D5; // Connected to IR Receiver
uint8_t SEND_PIN = D6; // Connected to IR LED
uint8_t relaisPin = D7; //Used in Example

//Wifi Parameters
#include <ESP8266WiFi.h>
const char* ssid = "WifiSSID"; //add your SSID here
const char* password = "WifiPW"; //add your Wifi Password here
const int wifiTryLimit = 20; // Limits wait time for Wifi Seconds. Higher Value = longer boottime on a failed connection

// IR Setup
#include <IRremote.h>
String readCode = "";
IRrecv irrecv(RECV_PIN);
IRsend irsend(SEND_PIN);

// MQTT Setup
#include <PubSubClient.h>
const char* mqttServer = "MQTTBrokerIP";
const int mqttPort = 1883;
const char* mqttUser = "YourMqttUsername";
const char* mqttPassword = "YourMqttPassword";
const char* mqttTopic = "topic code";
WiFiClient espClient;
PubSubClient client(espClient);

void setup(){
  pinMode(LED_BUILTIN, OUTPUT); // used to indicate if IR is enabled
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(relaisPin,OUTPUT);
  digitalWrite(relaisPin,LOW);
  Serial.begin(115200);

  connectToWifi();  
  if (WiFi.status() != WL_CONNECTED)
  {
    //enables MQTT if WifiConnected
        client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    if (client.connect("IrTv", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe("homeassistant/IRSender");
    } else {
      Serial.println("Connection to MQTT broker failed");
    }
  }
  irrecv.enableIRIn(); // Start the IR receiver
  digitalWrite(LED_BUILTIN, LOW);  
  Serial.println("is enabled");
}
//Continously checks for IR-Signals
void loop(){
  if (client.connected())
  {
    client.loop();
  }
  receiveCode();
}

// Regards Wifi Connection
  void connectToWifi(){
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    int i=0;
    while (WiFi.status() != WL_CONNECTED && i<wifiTryLimit) {
      i++;
      delay(1000);
      Serial.print(".");
    }
    if(i==100){
      if (WiFi.status() != WL_CONNECTED){
        Serial.println("Connection failed");
      }      
      else{
        Serial.println("");
        Serial.println("Connected to WiFi");
        Serial.println(WiFi.localIP());
      }
    }
  }
  

//Regards MQTT
  void callback(char* topic, byte*payload,unsigned int length){
    if (strcmp(topic, mqttTopic) ==0){
      String mqttCode = "";
      for(unsigned int j=0; j<length; j++)
      {
        mqttCode += (char)payload[j];
      }
      if (mqttCode != "")
      {
        analyseCode(mqttCode);

      }
    }
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

  // Matches received RawCode
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
  //prints predone Code for uregistered/ not used IR Codes
  void printBlankCode (String Code){
    Serial.println("no match found: Add this to analyseCode():");
    Serial.println("else if(code ==\"" + String(Code) + "\"){");
    Serial.println("//Add your code here");
    Serial.println("}");
  }


// Example UseCases
  void turnOnPc(){ 
    //simulates button push with a relais
    Serial.println("Turning on PC");
    digitalWrite(relaisPin, HIGH);
    delay(500);
    digitalWrite(relaisPin, LOW);
  }

  void turnOnTV(){ 
    //simulates power button push on remote
      Serial.println("Turning on the TV");
      IRData CodeToSend;
      CodeToSend.protocol = SAMSUNG;
      CodeToSend.address = 0x707;
      CodeToSend.command = 0x2 ;
      CodeToSend.numberOfBits = 32;
      
      sendCode(CodeToSend);
  }