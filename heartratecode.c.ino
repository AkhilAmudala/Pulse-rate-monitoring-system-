#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//  Variables
int PulseWire = A0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value.                           
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
#include<ESP8266WiFi.h>
#include"Adafruit_MQTT_Client.h"
#define wifi "wifi hotspot name"
#define password "wifi hotspot password"
#define server "io.adafruit.com"
#define port 1883
#define username "fill your username"
#define key "fill your key"
WiFiClient esp;
Adafruit_MQTT_Client mqtt(&esp,server,port,username,key);
Adafruit_MQTT_Publish feed1=Adafruit_MQTT_Publish(&mqtt,username"/feeds/Pulse_Rate");

void pulse_setup(){
    // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
}

int pulse_loop(){
   int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 

if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
 Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
 Serial.print("BPM: ");                        // Print phrase "BPM: " 
 Serial.println(myBPM);                        // Print the value inside of myBPM. 
}

  delay(20);                    // considered best practice in a simple sketch.
return myBPM;
}
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("Adafruit MQTT demo");
  Serial.print("Connecting to ");
  Serial.println(wifi);
  WiFi.begin(wifi,password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  Serial.print("IP Address :");
  Serial.println(WiFi.localIP());
  Serial.println("Connecting to MQTT");
  while(mqtt.connect()){
    Serial.print(".");
  }
  pulse_setup();
}

void loop() {
  if (mqtt.connected()){
    int bpm=pulse_loop();
    Serial.println(bpm);
    if(feed1.publish(bpm)){
      Serial.println("Success");
    }else{
      Serial.println("Fail");
    }
    delay(2000);
    
  }
}
