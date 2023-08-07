#include <Adafruit_GFX.h>    
#include <Adafruit_TFTLCD.h> 
#include <TouchScreen.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define TFT_WIDTH 240

// DEFINIÇÕES
#define WHITE          0xFFFF
#define YELLOW         0xFFE0
#define RED            0xF800
#define BLUE           0x001F
#define BLACK          0x0000
#define GREEN          0x07E0

#define LIGADO         0x07E0
#define DESLIGADO      0xF800

#define MINPRESSURE 10
#define MAXPRESSURE 1000


/*____CALIBRAR TFT LCD_____*/
#define YP A3
#define XM A2
#define YM 9
#define XP 8

#define TS_LEFT 129
#define TS_RT   924
#define TS_TOP  82
#define TS_BOT  896
/*______FIM DA CALIBRAÇÃO______*/

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); //300 É A SENSITIVIDADE
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

int X, Y;
long corBot[3] = {DESLIGADO, DESLIGADO, DESLIGADO};
const char* menu[3] = {"Scales", "Chords", "Improvisation"};
int currentMenu = 0;

TSPoint waitTouch() {
  TSPoint p;
  do {
    p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
  } while ((p.z < MINPRESSURE ) || (p.z > MAXPRESSURE));
  
  p.x = map(p.x, TS_LEFT, TS_RT, 0, 240);
  p.y = map(p.y, TS_BOT, TS_TOP, 0, 320);
  
  return p;
}

void setup() {
  Serial.begin(9600);
  
  tft.reset();
  tft.begin(0x9341); // CÓDIGO DO DRIVER DO SEU DISPLAY
  tft.setRotation(2);
  tft.fillScreen(BLACK);

  drawUI();
}

void drawUI() {
  tft.fillScreen(0);
  tft.setTextSize(2);
  tft.setCursor(80, 50);
  tft.print("Scales");

  tft.fillRect  (0, 200 , 240, 60, YELLOW);
  tft.fillRect  (115, 200 , 4, 60, BLACK);
  tft.setTextSize(4);
  tft.setTextColor(BLACK);
  tft.setCursor(40, 215);
  tft.print("<");
  tft.setCursor(170, 215);
  tft.print(">");

  tft.fillRect  (0, 260 , 240, 60, GREEN);
  tft.setCursor(40, 275);
  tft.print("PRATICE");
}

int getCenterXPosition(const char* text, uint8_t textSize) {
  int textWidth = strlen(text) * (6 * textSize); // Each character is 6 pixels wide in the default font
  int centerX = (TFT_WIDTH - textWidth) / 2;
  return centerX;
}

void UpdateSelectedOption() {
    tft.fillRect  (0, 50 , 240, 80, BLACK);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.setCursor(getCenterXPosition(menu[currentMenu], 2), 50);
    tft.print(menu[currentMenu]);
}

void loop() {
  TSPoint p = waitTouch();
  X = p.x; Y = p.y;
  ChoseMode();
  delay(300);
}

void ChoseMode() {
    if(Y > 60 && Y < 120) {
      if(X > 0 && X < 100) {
        if(currentMenu > 0) {
          currentMenu--;
          UpdateSelectedOption();
        }
      }

      if(X > 120 && X < 240) {
          if(currentMenu < 2) {
          currentMenu++;
          UpdateSelectedOption();
        }
      }
  }
}

// IMPLEMENTO DE FUNÇÕES

void DetectButtons() {

  if(Y > 220 && Y < 300) {
    if(X > 30 && X < 115) {
      tft.fillScreen(0);
      tft.setCursor(30, 100);
      tft.print("X");
    }
  }
}
