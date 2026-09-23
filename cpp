#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>



const int bPin = 6;  //buzzer pin
const int b1Pin  = 0;
const int b2Pin  = 1;
const int b3Pin  = 2;
const int b4Pin  = 3;

#define sWidth 128
#define sHeight 64
#define sReset -1

Adafruit_SSD1306 dis(sWidth, sHeight, &Wire, sReset);

void shoot(int buzzerPin, int speedMultiplier = 1, int toneDelay = 10){
  for (int i = 100; i < 1000; i += 50*speedMultiplier) {        //high tone
      tone(buzzerPin, i);
      delay(toneDelay);
    }
    noTone(buzzerPin);

    for (int i = 1000; i >100; i -= 50) {                       //low tone
      tone(buzzerPin, i);
      delay(toneDelay);
    }
    noTone(buzzerPin);
}

void centerText(String text, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  
  // Get width and height of the given text string
  dis.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  
  // Calculate X position to center horizontally
  int x = (sWidth - w) / 2;
  
  dis.setCursor(x, y);
  dis.print(text);
}

int detectPress(int numberOfButtons){
  for(int i = 0; i < numberOfButtons; i++){
    if(digitalRead(i) == LOW){
      return i;
    }
  }
  return -1;
}


void setup() {
  Wire.begin();

  if(!dis.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    for(;;); // Halt if the screen fails to start
  }

  dis.clearDisplay();
  dis.setTextSize(1);
  dis.setTextColor(SSD1306_WHITE);
  dis.setCursor(50, 30);
  dis.display();

  pinMode(b1Pin, INPUT_PULLUP);
  pinMode(b2Pin, INPUT_PULLUP);
  pinMode(b3Pin, INPUT_PULLUP);
  pinMode(b4Pin, INPUT_PULLUP);
  
  pinMode(bPin, OUTPUT);
}

void loop() {

  switch (detectPress(4)) {
    case 0:
      dis.fillRect(1, 0, 32, 64, SSD1306_WHITE);
      break;

    case 1:
      dis.fillRect(32, 0, 32, 64, SSD1306_WHITE);
      break;

    case 2:
      dis.fillRect(64, 0, 32, 64, SSD1306_WHITE);
      break;

    case 3:
      dis.fillRect(96, 0, 32, 64, SSD1306_WHITE);
      break;

    case -1:
      dis.clearDisplay();
      dis.drawRect(1, 0, 32, 64, SSD1306_WHITE);
      dis.drawRect(32, 0, 32, 64, SSD1306_WHITE);
      dis.drawRect(64, 0, 32, 64, SSD1306_WHITE);
      dis.drawRect(96, 0, 32, 64, SSD1306_WHITE);
      break;
  }

  dis.display();
}
