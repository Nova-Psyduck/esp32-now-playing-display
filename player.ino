#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Wi-Fi credentials ---
const char* ssid = " ";
const char* password = " ";

WebServer server(80);
String currentTrack = "";

// --- Scrolling Variables ---
int xPos = SCREEN_WIDTH; 
int minX = 0; 
unsigned long previousMillis = 0;
const long frameInterval = 25; 

// --- LED Breathing Variables ---
const int ledPin = D3; 
unsigned long previousFadeMillis = 0;
const long fadeInterval = 15; // Speed of the breathing (lower is faster)
int brightness = 0;
int fadeAmount = 5;

void handlePost() {
  if (server.hasArg("plain")) {
    String incoming = server.arg("plain"); 
    
    // If the bash script tells us the music stopped, clear the track
    if (incoming == "STOP" || incoming == " - ") {
      currentTrack = "";
    } else {
      currentTrack = incoming;
      xPos = SCREEN_WIDTH; 
      minX = -1 * (currentTrack.length() * 12); 
    }
    
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void setup() {
  // Serial.begin() removed so it runs headless without crashing
  
  pinMode(ledPin, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 5);
  display.println("Connecting to WiFi:");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi Connected!");
  display.setCursor(0, 16);
  display.println(WiFi.localIP());
  display.display();
  
  delay(5000); 

  server.on("/update", HTTP_POST, handlePost);
  server.begin();
}

void loop() {
  server.handleClient();
  unsigned long currentMillis = millis();
  
  // -----------------------------------------
  // 1. LED BREATHING ENGINE
  // -----------------------------------------
  if (currentTrack != "") {
    // If a song is playing, breathe the LED
    if (currentMillis - previousFadeMillis >= fadeInterval) {
      previousFadeMillis = currentMillis;
      
      analogWrite(ledPin, brightness);
      brightness = brightness + fadeAmount;
      
      // Reverse direction when we hit the limits
      if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount; 
      }
    }
  } else {
    // If no song is playing, force the LED off
    analogWrite(ledPin, 0);
    brightness = 0;
  }

  // -----------------------------------------
  // 2. OLED SCROLLING ENGINE
  // -----------------------------------------
  if (currentMillis - previousMillis >= frameInterval) {
    previousMillis = currentMillis;

    display.clearDisplay();
    display.setTextSize(2); 
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(false); 

    if (currentTrack == "") {
      display.setCursor(0, 8); 
      display.print("Waiting...");
    } else {
      display.setCursor(xPos, 8); 
      display.print(currentTrack);
      
      xPos -= 2; 
      if(xPos < minX) {
         xPos = SCREEN_WIDTH;
      }
    }
    
    display.display();
  }
}
