/*
 * ESP_NOW Protocol on ESP8266
 * 
 * peer to peer communication between two ESPs
 * 
 * Based on WifiEspNow Library created by YOURSUNNY. Github: https://github.com/yoursunny/WifiEspNow
 * 
 * Like us on facebook : https://fb.com/electropoint4u
 * Follow @electropoint4u at Instagram : https://instagram.com/electropoint4u
 * 
 * Press the button in one ESP to lightup a LED on the other board and vice versa

   How does it Works : 
   
    Upload this code as it is to your ESP module
    Open up serial monitor you will se something like this
    MAC address of this node is XX:XX:XX:XX:XX:XX
    Note down the MAC address
    Do the Same for both Modules
    Now change the code of the FIRST node ( 00 in the PEER[]) with the actual MAC address values of the SECOND node
    upload the code and it's done!
    
*/


#include <WifiEspNow.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include <Adafruit_NeoPixel.h>

static uint8_t PEER[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};     //Change this Section according to the MAC ADDRESS of your board

bool state = false;

#define BUZZER    0                 // a buzzer is connected to the GPIO 0
#define BUTTON    2                 // a Push Button is connected to the GPIO 2
#define PIN       5                 // a WS2812b RGB led is connected to GPIO 5
#define NUMPIXELS 1                 // Number of RGB Leds

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//RECIEVE DATA
void printReceivedMessage(const uint8_t mac[6], const uint8_t* buf, size_t count, void* cbarg) {
  Serial.printf("Message from %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  for (int i = 0; i < count; ++i) {
    Serial.print(static_cast<char>(buf[i]));
  }
  Serial.println();
  state = true;

}

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color( 50,   0,   0));
  pixels.show();
  pinMode(BUTTON, INPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.println();

  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESPNOW", nullptr, 3);
  WiFi.softAPdisconnect(false);

  Serial.print("MAC address of this node is ");
  Serial.println(WiFi.softAPmacAddress());

  bool ok = WifiEspNow.begin();
  if (!ok) {
    Serial.println("WifiEspNow.begin() failed");
    ESP.restart();
  }

  WifiEspNow.onReceive(printReceivedMessage, nullptr);

  ok = WifiEspNow.addPeer(PEER);
  if (!ok) {
    Serial.println("WifiEspNow.addPeer() failed");
    ESP.restart();
  }
  else {
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
  }
}

void loop() {
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(  0,   0,   0));
  pixels.show();
  if (digitalRead(BUTTON) == LOW) {      //SEND DATA
    char msg[60];
    int len = snprintf(msg, sizeof(msg), "hello ESP-NOW from %s at %lu", WiFi.softAPmacAddress().c_str(), millis());
    WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), len);
    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(  0,  50,   0));
    pixels.show();
    delay(6000);
    ESP.restart();
  }
  if (state) {
    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(  0,   0,  50));
    pixels.show();
    for (int k = 0; k < 10; k++) {
      digitalWrite(BUZZER, HIGH);
      delay(200);
      digitalWrite(BUZZER, LOW);
      delay(100);
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
      delay(200);
    }
    ESP.restart();
  }
}
