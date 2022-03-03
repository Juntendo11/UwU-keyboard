/* Giant UwU keyboard by Juntendo (Juichi Nishimura) https://www.youtube.com/channel/UC73LkQJ47rRI07iMOzu7-5g
I referenced code from several sources listed below:
https://circuitjournal.com/how-to-use-the-dfplayer-mini-mp3-module-with-an-arduino
https://stackoverflow.com/questions/59394851/how-to-create-a-rainbow-wave-on-led-strip-using-arduino
*/


#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <FastLED.h>
#include <Keyboard.h>

#define LED_TYPE WS2812B
#define DATA_PIN 10
#define NUM_LEDS 10
#define BRIGHTNESS 255
#define SATURATION 255

CRGB leds[NUM_LEDS];

// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_TX = 9; // Connects to module's RX
static const uint8_t PIN_MP3_RX = 8; // Connects to module's TX
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// Create the Player object
DFRobotDFPlayerMini player;

int button = 15;
int data = 10;
int num_led = 10;

boolean buttonActive = false;
boolean longPressActive = false;

unsigned long buttonTimer =0;
unsigned long longPressTime = 500;

void longPressed(){
  player.volume(30);
  player.playMp3Folder(1);
}

void shortPressed(){
    Keyboard.print("UwU");
    leds[0] = CRGB::Red;
    FastLED.show();
    int file = random(2,6);
    if (file == 2){
      player.volume(30);
      player.playMp3Folder(2);
    }
    else if (file == 3){
      player.volume(30);
      player.playMp3Folder(2);
    }
    else if (file ==4){
      player.volume(30);
      player.playMp3Folder(4);
    }
    else if (file ==5){
      player.volume(30);
      player.playMp3Folder(5);
    }
    fill_solid(leds, NUM_LEDS, CRGB::Pink);
    FastLED.show();
    delay(200);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
}

void setup() {
  pinMode(15,INPUT_PULLUP);
  Keyboard.begin();
  Serial.begin(9600);
  softwareSerial.begin(9600);
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
  // Start communication with DFPlayer Mini
  if (player.begin(softwareSerial)) {
    Serial.println("Connected!");
  } else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
}

void loop() {
  for (int j = 0; j < 255; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(i - (j * 2), SATURATION, BRIGHTNESS); /* The higher the value 4 the less fade there is and vice versa */ 
      if (digitalRead(button) == LOW){
        if(buttonActive == false){
          buttonActive = true;
          buttonTimer = millis();
        }
        if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
          longPressActive = true;
          longPressed();
        }
      }
      else{
        if (buttonActive == true){
          if (longPressActive == true){
            longPressActive = false;
          }
          else{
            shortPressed();
          }
          buttonActive = false;
        }
      }
    }
    FastLED.show();
    delay(25); /* Change this to your hearts desire, the lower the value the faster your colors move (and vice versa) */
  }
}
