#include <Servo.h>
#include <ESP8266WiFi.h>

const char* ssid     = "ESP8266-Access-Point";
const char* password = "123456789";

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
static const int servoPin1 = 13;
static const int servoPin2 = 14;
static const int servoPin3 = 12;
static const int servoPin4 = 15;
WiFiServer server(80);
String header;
String valueString1 = String(5);
String valueString2 = String(5);
String valueString3 = String(5);
String valueString4 = String(5);
int servo1 = 90;
int servo2 = 90;
int servo3 = 90;
void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  myservo1.attach(servoPin1);
  myservo2.attach(servoPin2);
  myservo3.attach(servoPin3);
  myservo4.attach(servoPin4);
  Serial.print("Connecting to ");
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(IP);  
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
              servo1--;
              //Rotate the servo
              myservo1.write(servo1);
              Serial.println(servo1); 
            }
            
            if(header.indexOf("GET /right")>=0) {
              servo1++;
              //Rotate the servo
              myservo1.write(servo1);
              Serial.println(servo1); 
            }            

            if(header.indexOf("GET /forward")>=0) {
              servo2--;
              //Rotate the servo
              myservo1.write(servo2);
              Serial.println(servo2); 
            }
            
            if(header.indexOf("GET /backward")>=0) {
              servo2++;
              //Rotate the servo
              myservo1.write(servo2);
              Serial.println(servo2); 
            }

            if(header.indexOf("GET /clawopen")>=0) {
              servo3--;
              //Rotate the servo
              myservo1.write(servo3);
              Serial.println(servo3); 
            }
            
            if(header.indexOf("GET /clawclose")>=0) {
              servo3++;
              //Rotate the servo
              myservo1.write(servo3);
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
