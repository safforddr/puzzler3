/*
    Puzzler-3 - a "safe" website
    Compile and run on an esp32-s3.
    This code will turn on and off the RGB led on a waveshare "esp32-s3 zero"
*/
#define RGB_BRIGHTNESS 10 // Change white brightness (max 255)
#define RGB_BUILTIN 21    // RGB pin

#include <WiFi.h>
#include <WiFiAP.h>

const char* ssid     = "PUZZLER3";
const char* password = "mxyzptlk";

WiFiServer server(80);  

void setup() {
  
  Serial.begin(115200);
  while(!Serial) {
    ;
  }
  
  WiFi.softAP(ssid, password);
  
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    char linebuf[80];
    int charcount=0;
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML><html><head>");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
          client.println("<h1>PUZZLER3 Web Server</h1>");
          client.println("YAY! You made it here!");
          client.println("<br>");
          client.println("The book for this puzzle is any image of the front of the Declaration of Independence. <br> ");          
          client.println("<br>The Ottendorf Key is: \"");
          client.println("<span style=\"color: transparent;\">{1,16}{2,1}{3,1}{3,11}</span>");
          client.println("\"<br>");
          client.println("In the key, the first number is the line, the second is the character on the line, ");
          client.println("counting punctuation, but not spaces.<br>");
          client.println("The key has only four pairs, which will result in two digits, which you have to repeat three times to open the safe.");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
          if (strstr(linebuf,"GET /on1")){
            Serial.println("LED 1 ON");
            neopixelWrite(RGB_BUILTIN,0,RGB_BRIGHTNESS,0); // Red??
          }
          else if (strstr(linebuf,"GET /off1")){
            Serial.println("LED 1 OFF");
            neopixelWrite(RGB_BUILTIN,0,0,0); // off
          }
          else if (strstr(linebuf,"GET /on2")){
            Serial.println("LED 2 ON");
            neopixelWrite(RGB_BUILTIN,RGB_BRIGHTNESS,0,0); // Green??
          }
          else if (strstr(linebuf,"GET /off2")){
            Serial.println("LED 2 OFF");
            neopixelWrite(RGB_BUILTIN,0,0,0); // off
          }
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("client disconnected");
  }
}
