#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

const char* ssid = "Campcj-wifi";
const char* password = "Circus01";
const char* host = "10.0.0.115"; //it will tell you the IP once it starts up
//just write it here afterwards and upload
//int ledPin = LED_BUILTIN;
double dTempSet = 102.0;
LiquidCrystal_I2C lcd(0x27, 5, 4);

WiFiServer server(301); //just pick any port number you like

void setup() {
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight
  lcd.setCursor ( 0, 0 );            // go to the top left corner
  lcd.print("Clint Hot Tub v-1.0"); // write this string on the top row
  lcd.setCursor ( 0, 1 );            // go to the top left corner
  lcd.print("Temperature: 102.0"); // write this string on the top row
  Serial.begin(115200);
  delay(10);
  Serial.println(WiFi.localIP());
  // prepare GPIO2
  //  pinMode(ledPin, OUTPUT);
  //  digitalWrite(D3, LOW);

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

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

}

void loop() {
  // Check if a client has connected

  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');

  client.flush();
  Serial.println(req);
  // Match the request


  if (req.indexOf("") != -10) {  //checks if you're on the main page

    if (req.indexOf("/UP") != -1) { //checks if you clicked OFF
      dTempSet += 0.1;
      Serial.print("You clicked UP: ");
      Serial.println(dTempSet);
      lcd.setCursor ( 13, 1 );            // go to the top left corner
      lcd.print(dTempSet); // write this string on the top row
    }
    if (req.indexOf("/DOWN") != -1) { //checks if you clicked ON
      dTempSet = dTempSet - 0.1;
      Serial.print("You clicked DOWN: ");
      Serial.println(dTempSet);
      lcd.setCursor ( 13, 1 );            // go to the top left corner
      lcd.print(dTempSet); // write this string on the top row
    }
  }

  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  //  dTempSet = client.arg("webTemp").toDouble();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "<br><FORM METHOD=\"POST\"action=\"/postForm";
  s += dTempSet;
  s += "\"><input type=\"number\" name=\"webTemp\" min=\"65.0\" max=\"104.0\" step=\"0.1\" value=\"";
  s += dTempSet;
  s += "\"><input type=\"submit\" value=\"Post Notice\"></form><br><br>";
  s += "Analog input:  " + String(analogRead(A0));
  s += "<br><input type=\"button\" name=\"bl\" value=\"Temp UP \" onclick=\"location.href='/UP'\">";
  s += "<br><br><br>";
  s += "<br><input type=\"button\" name=\"bl\" value=\"Temp DOWN\" onclick=\"location.href='/DOWN'\">";
  s += "</html>\n";

  client.flush();


  // Send the response to the client
  client.print(s);
  delay(1);



}
