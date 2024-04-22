/* WiFi access point 
* we want a button to play a song
*
* we are using wifiaccess point and speaker examples
*
*
* Edward Mills
*21Apr2024
*Edwmills@uat,edu
*/

//library and setup for wifi
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <M5StickCPlus.h>

const char *ssid = "EdwM5stickC";
const char *password = "test1test1";

WiFiServer server(80);

//m5stickc plus speaker stuff
const int servo_pin = 26;
int freq            = 50;
int ledChannel      = 0;
int resolution      = 10;
extern const unsigned char m5stack_startup_music[];
#define delayTime 20
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");
  
// You can remove the password parameter if you want the AP to be open.
// a valid password must have more than 7 characters
if (!WiFi.softAP(ssid, password)) {
log_e("Soft AP creation failed.");
while(1);
}
IPAddress myIP = WiFi.softAPIP();
Serial.print("AP IP address: ");
Serial.println(myIP);
server.begin();

Serial.println("Server started");


M5.begin();
M5.Lcd.setRotation(0);
M5.Lcd.setCursor(25, 80, 4);
M5.Lcd.println("speaker");
ledcSetup(ledChannel, freq, resolution);
ledcAttachPin(servo_pin, ledChannel);
ledcWrite(ledChannel, 0);  // 0°

}

//music from the speaker sketch
void playMusic(const uint8_t* music_data, uint16_t sample_rate) {
uint32_t length         = strlen((char*)music_data);
uint16_t delay_interval = ((uint32_t)1000000 / sample_rate);
for (int i = 0; i < length; i++) {
    ledcWriteTone(ledChannel, music_data[i] * 50);
    delayMicroseconds(delay_interval);
}
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }


        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          underPressure();
        }
        if (currentLine.endsWith("GET /L")) {
          Serial.println("/L chosen");
        }
       
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }

}

//the song i wish to play
  void underPressure(){
    //under pressire baseline
    //D4 is 587 Hz
    // 1/4 note is 516ms
    // 1/16 note is 129
    // 1/8 note is 258
    //a note is 220
    // put your main code here, to run repeatedly:
      for(int i = 0; i< 3; i++){
  ledcWriteTone(ledChannel, 587);
delay(258); 
ledcWriteTone(ledChannel, 0);
delay(delayTime);
}
  for(int i = 0; i< 2; i++){
  ledcWriteTone(ledChannel, 587);
delay(129); 
  ledcWriteTone(ledChannel, 0);
delay(delayTime);
}
ledcWriteTone(ledChannel, 587);
delay(258);
ledcWriteTone(ledChannel, 0);
delay(delayTime);
ledcWriteTone(ledChannel, 220);
delay(258);
ledcWriteTone(ledChannel, 0);
delay(delayTime);
ledcWriteTone(ledChannel, 0);
delay(1000);
    }