void Line_Notify(String message) ;
#include <ESP8266WiFi.h>
#include <WiFiClientSecureAxTLS.h>
// Config connect WiFi
#define WIFI_SSID ""// your wifi name 
#define WIFI_PASSWORD "" // your wifi password
// Line config
#define LINE_TOKEN ""//your line token
void setup() {
  Serial.begin(9600);
  pinMode(D0, INPUT);
  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}
int smokedetect_v;
String message = "Smoke Detected!";
void loop() {
  smokedetect_v = digitalRead(A0);
  Serial.println(analogRead(A0));
  if (smokedetect_v == 0)
  {
    Line_Notify(message = "smoke detected!" );
    Serial.println("smoke detect");
  } else
  {
    Serial.println("Smoke not detect");
    }
  delay(5000);
}
//Line Notify method sent String to Line Account on phone
void Line_Notify(String message) {
  axTLS::WiFiClientSecure client;
  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;
  }
  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  client.print(req);
  delay(20);
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }

  }

}
