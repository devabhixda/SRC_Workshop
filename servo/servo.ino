#include <Servo.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

Servo myservo1;
Servo myservo2;
Servo myservo3;

static const int servoPin1 = 13;
static const int servoPin2 = 14;
static const int servoPin3 = 12;

WiFiServer server(80);
String header;
int servo1 = 90;
int servo2 = 90;
int servo3 = 90;
void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("SRC");
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  
  myservo1.attach(servoPin1);
  myservo2.attach(servoPin2);
  myservo3.attach(servoPin3);
  Serial.print("Connecting to ");
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();  
  if (client) {                             
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {

            if(header.indexOf("GET /left")>=0) {
              if(servo1>10){
                servo1=servo1-10;
              }
              //Rotate the servo
              myservo1.write(servo1);
              Serial.println(servo1); 
            }
            
            if(header.indexOf("GET /right")>=0) {
              if(servo1<180){
                servo1=servo1+10;
              }
              //Rotate the servo
              myservo1.write(servo1);
              Serial.println(servo1); 
            }            

            if(header.indexOf("GET /forward")>=0) {
              if(servo2>10){
                servo2=servo2-10;
              }
              //Rotate the servo
              myservo3.write(servo2);
              Serial.println(servo2); 
            }
            
            if(header.indexOf("GET /backward")>=0) {
              if(servo2<180){
                servo2=servo2+10;
              }
              //Rotate the servo
              myservo3.write(servo2);
              Serial.println(servo2); 
            }

            if(header.indexOf("GET /clawopen")>=0) {
              if(servo3>10){
                servo3=servo3-10;
              }
              //Rotate the servo
              myservo2.write(servo3);
              Serial.println(servo3); 
            }
            
            if(header.indexOf("GET /clawclose")>=0) {
              if(servo3<180){
                servo3=servo3+10;
              }
              //Rotate the servo
              myservo2.write(servo3);
              Serial.println(servo3); 
            }  
                                   
            // The HTTP response ends with another blank line
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
