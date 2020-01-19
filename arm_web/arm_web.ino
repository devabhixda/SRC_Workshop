#include <Servo.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

//Declare 3 servos
Servo myservo1;
Servo myservo2;
Servo myservo3;

//Allot pins to each servo
static const int servoPin1 = 13;
static const int servoPin2 = 14;
static const int servoPin3 = 12;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Decode HTTP GET value and set sliders initially to 0
String valueString1 = String(0);
String valueString2 = String(0);
String valueString3 = String(0);

int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;
int pos6 = 0;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("SRC");
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  
  // attaches the servo on the servoPin to the servo object
  myservo1.attach(servoPin1);
  myservo2.attach(servoPin2);
  myservo3.attach(servoPin3);

  // Print local IP address and start web server
  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println(); //very imp to init html on client side

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
            client.println("</head><body><h1>Acrobot</h1>");

            //Servo 1
            client.println("<h3>Base Motion</h3>");
            client.println("<p>Position: <span id=\"servoPos1\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider1\" onchange=\"servo1(this.value)\" value=\""+valueString1+"\"/>");         
            client.println("<script>var slider1 = document.getElementById(\"servoSlider1\");");
            client.println("var servoP1 = document.getElementById(\"servoPos1\"); servoP1.innerHTML = slider1.value;");
            client.println("slider1.oninput = function() { slider1.value = this.value; servoP1.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo1(pos) { ");
            client.println("$.get(\"/?value1=\" + pos + \"&\"); {Connection: close};}</script>");

            //Servo 2
            client.println("<h3>Claw Motion</h3>");
            client.println("<p>Position: <span id=\"servoPos2\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"100\" class=\"slider\" id=\"servoSlider2\" onchange=\"servo2(this.value)\" value=\""+valueString2+"\"/>");         
            client.println("<script>var slider2 = document.getElementById(\"servoSlider2\");");
            client.println("var servoP2 = document.getElementById(\"servoPos2\"); servoP2.innerHTML = slider2.value;");
            client.println("slider2.oninput = function() { slider2.value = this.value; servoP2.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo2(pos) { ");
            client.println("$.get(\"/?value2=\" + pos + \"&\"); {Connection: close};}</script>");

            //Servo 3
            client.println("<h3>Elbow Motion</h3>");
            client.println("<p>Position: <span id=\"servoPos3\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider3\" onchange=\"servo3(this.value)\" value=\""+valueString3+"\"/>");         
            client.println("<script>var slider3 = document.getElementById(\"servoSlider3\");");
            client.println("var servoP3 = document.getElementById(\"servoPos3\"); servoP3.innerHTML = slider3.value;");
            client.println("slider3.oninput = function() { slider3.value = this.value; servoP3.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo3(pos) { ");
            client.println("$.get(\"/?value3=\" + pos + \"&\"); {Connection: close};}</script>");
            
            client.println("</body></html>");     

            //GET /?value1=180& HTTP/1.1
            if(header.indexOf("GET /?value1=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString1 = header.substring(pos1+1, pos2);
              
              //Rotate the servo
              myservo1.write(valueString1.toInt());
              Serial.println("First "+valueString1); 
            }

            //GET /?value2=180& HTTP/1.1
            if(header.indexOf("GET /?value2=")>=0) {
              pos3 = header.indexOf('=');
              pos4 = header.indexOf('&');
              valueString2 = header.substring(pos3+1, pos4);
              
              //Rotate the servo
              myservo2.write(valueString2.toInt());
              Serial.println("Second "+valueString2); 
            }

            //GET /?value3=180& HTTP/1.1
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

            // Break out of the while loop
            break;
          } else {
            // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {

           // if you got anything else but a carriage return character,
           // add it to the end of the currentLine
          currentLine += c;
        }
      }
    }
    // Clear the header variable
    header = "";
    
    // Close the connection
    client.stop();
    Serial.println("");
  }
}
