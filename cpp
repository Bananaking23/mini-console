#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>



const int bPin = 6;  
const int b1Pin  = 0;
const int b2Pin  = 1;
const int b3Pin  = 2;
const int b4Pin  = 3;

#define sWidth 128
#define sHeight 64
#define sReset -1

Adafruit_SSD1306 dis(sWidth, sHeight, &Wire, sReset);

int pX = 64;
int pY = 32;


int points = 0;
int scoreX;
int scoreY = 5;
int bX = random(128);
int bY = random(64);
int pSpeed = 1.5;
static bool disableY = 0;
static bool disableX = 0;


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


template <typename T>
void centerText(T Text1, int y, bool flipColor = false) {
  int16_t x1, y1;
  uint16_t w, h;
  String text = String(Text1);
  
  // Get width and height of the given text string
  dis.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  
  // Calculate X position to center horizontally
  int x = (sWidth - w) / 2;
  scoreX = x;

  dis.setCursor(x, y);
  if(flipColor){
    if(pX - 4.5 <= x + 4.5 && x + 4.5 <= x - 4.5   &&
       pY - 4.5 <= y + 4.5 && y + 4.5 >= y - 4.5){
      dis.setTextColor(SSD1306_BLACK);
      dis.print(text);
    }
    else{
      dis.setTextColor(SSD1306_WHITE);
      dis.print(text);
    }
  }
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
  dis.setTextSize(2);
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
  bool notCaught = true;

  switch (detectPress(4)) {
    case 0:
      if(!disableX){
        pX -= pSpeed;
        break;
      }
    case 1:
      if(!disableY){
        pY -= pSpeed;
        break;
      }

    case 2:
      if(!disableY){
        pY += pSpeed;
        break;
      }

    case 3:
      if(!disableX){
        pX += pSpeed;
        break;
      }
    case -1:  
      dis.clearDisplay();
      break;
  }
  
  if( pX <= -2.5 ){
    pX = sWidth;
  }else if( pX >= sWidth + 2.5){
    pX = -2.5;
  }
  if( pY <= -2.5 ){
    pY = sHeight;
  }else if( pY >= sHeight + 2.5){
    pY = -2.5;
  }

  if(notCaught){
    if(pX - 4.5 <= bX + 4.5 && pX + 4.5 >= bX - 4.5   &&
       pY - 4.5 <= bY + 4.5 && pY + 4.5 >= bY - 4.5){  
      points++;
      notCaught = false;
      bX = random(128);
      bY = random(64);
    }
  }

  
  dis.clearDisplay();
  dis.fillCircle(pX, pY, 5, SSD1306_WHITE);
  dis.drawCircle(bX, bY, 5, SSD1306_WHITE);
  centerText(points, scoreY, true);
  dis.display();

}
