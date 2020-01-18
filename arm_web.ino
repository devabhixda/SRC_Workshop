#include <Servo.h>
#include <ESP8266WiFi.h>

const char* ssid     = "ESP8266-Access-Point"; //Team Name
const char* password = "123456789"; //Wifi password

Servo myservo1;
Servo myservo2;
Servo myservo3;
static const int servoPin1 = 13;
static const int servoPin2 = 14;
static const int servoPin3 = 12;
WiFiServer server(80);
String header;
String valueString1 = String(5);
String valueString2 = String(5);
String valueString3 = String(5);
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;
int pos6 = 0;
void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  myservo1.attach(servoPin1);
  myservo2.attach(servoPin2);
  myservo3.attach(servoPin3);
  Serial.print("Connecting to ");
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(IP);  
  //Serial.println(WiFi.localIP());
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
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println(); //very imp to init html on client side
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
            client.println("</head><body><h1>NodeMcu with Servo</h1>");

            //Servo 1
            client.println("<p>Position: <span id=\"servoPos1\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider1\" onchange=\"servo1(this.value)\" value=\""+valueString1+"\"/>");         
            client.println("<script>var slider1 = document.getElementById(\"servoSlider1\");");
            client.println("var servoP1 = document.getElementById(\"servoPos1\"); servoP1.innerHTML = slider1.value;");
            client.println("slider1.oninput = function() { slider1.value = this.value; servoP1.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo1(pos) { ");
            client.println("$.get(\"/?value1=\" + pos + \"&\"); {Connection: close};}</script>");

            //Servo 2
            client.println("<p>Position: <span id=\"servoPos2\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider2\" id=\"servoSlider2\" onchange=\"servo2(this.value)\" value=\""+valueString2+"\"/>");         
            client.println("<script>var slider2 = document.getElementById(\"servoSlider2\");");
            client.println("var servoP2 = document.getElementById(\"servoPos2\"); servoP2.innerHTML = slider2.value;");
            client.println("slider2.oninput = function() { slider2.value = this.value; servoP2.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo2(pos) { ");
            client.println("$.get(\"/?value2=\" + pos + \"&\"); {Connection: close};}</script>");

            //Servo 3
            client.println("<p>Position: <span id=\"servoPos3\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider3\" id=\"servoSlider3\" onchange=\"servo3(this.value)\" value=\""+valueString3+"\"/>");         
            client.println("<script>var slider3 = document.getElementById(\"servoSlider3\");");
            client.println("var servoP3 = document.getElementById(\"servoPos3\"); servoP3.innerHTML = slider3.value;");
            client.println("slider3.oninput = function() { slider3.value = this.value; servoP3.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo3(pos) { ");
            client.println("$.get(\"/?value3=\" + pos + \"&\"); {Connection: close};}</script>");
            client.println("</body></html>");     

            if(header.indexOf("GET /?value1=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString1 = header.substring(pos1+1, pos2);
              
              //Rotate the servo
              myservo1.write(valueString1.toInt());
              Serial.println("First "+valueString1); 
            }

            if(header.indexOf("GET /?value2=")>=0) {
              pos3 = header.indexOf('=');
              pos4 = header.indexOf('&');
              valueString2 = header.substring(pos3+1, pos4);
              
              //Rotate the servo
              myservo2.write(valueString2.toInt());
              Serial.println("Second "+valueString2); 
            }

            if(header.indexOf("GET /?value3=")>=0) {
              pos5 = header.indexOf('=');
              pos6 = header.indexOf('&');
              valueString3 = header.substring(pos5+1, pos6);
              
              //Rotate the servo
              myservo3.write(valueString3.toInt());
              Serial.println("Third "+valueString3); 
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
