#include <Arduino.h>
#include <WiFi.h>

#define LED_PIN 15
#define LOCKDOOR_PIN 4
char user_input;

// WiFi settings
const char* ssid = "Yashiru";
const char* password = "16081999Il";

// Set web server port number to 80
WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();


  // Set LED pin as output
  pinMode(LED_PIN, OUTPUT);
  pinMode(LOCKDOOR_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //on and off LED with wifi client server
  WiFiClient client = server.available();   // Listen for incoming clients
  
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n'){
          if (currentLine.length()==0){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        }
        else if (c != '\r'){
          currentLine += c;
        }

        if (currentLine.endsWith("GET /LED=ON")){
          digitalWrite(LED_PIN, HIGH);
          client.println("LED is ON");
        }

        if (currentLine.endsWith("GET /LED=OFF")){
          digitalWrite(LED_PIN, LOW);
          client.println("LED is OFF");
        }

        if (currentLine.endsWith("GET /LOCK=ON")){
          digitalWrite(LOCKDOOR_PIN, HIGH);
          client.println("LOCK is ON");
        }

        if (currentLine.endsWith("GET /LOCK=OFF")){
          digitalWrite(LOCKDOOR_PIN, LOW);
          client.println("LOCK is OFF");
        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client disconnected.");

  }

  // if (Serial.available() > 0) {
  //   user_input = Serial.read();
  //   if (user_input=='a') {
  //     digitalWrite(LED_PIN, HIGH);
  //   } else if (user_input=='b') {
  //     digitalWrite(LED_PIN, LOW);
  //   } else if (user_input=='c') {
  //     digitalWrite(LOCKDOOR_PIN, HIGH);
  //     delay(3000);
  //     digitalWrite(LOCKDOOR_PIN, LOW);
  //   }
  // }
}