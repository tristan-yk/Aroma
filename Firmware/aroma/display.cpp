#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "state.h"
#include "sensor.h"
#include <Fonts/FreeSansOblique12pt7b.h>  
#include <Fonts/FreeSansOblique9pt7b.h>  

#define TFT_CS   16
#define TFT_RST  -1
#define TFT_DC   5

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

uint8_t lerp(uint8_t a, uint8_t b, float t) {
  return a + t * (b - a);
}

void drawTitle() {
  int midpoint = tft.height() / 2;

  // Darker cotton candy background
  for (int y = 0; y < tft.height(); y++) {
    uint8_t r, g, b;

    if (y < midpoint) {
      float t = (float)y / midpoint;
      r = lerp(128, 200, t);  // dark pink → grayish
      g = lerp(64, 200, t);
      b = lerp(96, 200, t);
    } else {
      float t = (float)(y - midpoint) / (tft.height() - midpoint);
      r = lerp(200, 80, t);   // grayish → muted blue
      g = lerp(200, 120, t);
      b = lerp(200, 160, t);
    }

    uint16_t color = tft.color565(r, g, b);
    tft.drawFastHLine(0, y, tft.width(), color);
  }

  // Draw centered italic text with outline
  tft.setFont(&FreeSansOblique12pt7b);
  String text = "AROMA";

  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  int x = (tft.width() - w) / 2;
  int y = (tft.height() + h) / 2;

  // Outline pass — draw black text in 8 surrounding pixels
  tft.setTextColor(ST77XX_BLACK);
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (dx == 0 && dy == 0) continue;
      tft.setCursor(x + dx, y + dy);
      tft.print(text);
    }
  }

  // Main white text
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(x, y);
  tft.print(text);

  tft.setFont(); // default bitmap font
  tft.setTextSize(1);
  String footer = "T. Yan-Klassen";

  tft.getTextBounds(footer, 0, 0, &x1, &y1, &w, &h);
  int footerX = (tft.width() - w) / 2;
  int footerY = tft.height() - h - 7;

  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(footerX + 1, footerY + 1);
  tft.print(footer);

  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(footerX, footerY);
  tft.print(footer);

  // Reset font to default
  tft.setFont();
}

void drawActiveScreen(const char* scentName, bool waterHigh) {
  // ----- Gradient Selection -----
  for (int y = 0; y < tft.height(); y++) {
    float t = (float)y / tft.height();

    uint8_t r = 200, g = 200, b = 200; // default neutral

    if (strcmp(scentName, "LAVENDER") == 0) {
      // soft purple → lilac
      r = lerp(120, 200, t);
      g = lerp(100, 180, t);
      b = lerp(180, 230, t);

    } else if (strcmp(scentName, "CITRUS") == 0) {
      // lemon yellow → warm orange
      r = lerp(255, 255, t);
      g = lerp(240, 180, t);
      b = lerp(150, 90, t);

    } else if (strcmp(scentName, "MINT") == 0) {
      // cool teal → soft blue-green
      r = lerp(100, 180, t);
      g = lerp(230, 255, t);
      b = lerp(200, 220, t);

    } else if (strcmp(scentName, "EUCALYPTUS") == 0) {
      // pale green → dusky forest green
      r = lerp(160, 80, t);
      g = lerp(220, 150, t);
      b = lerp(180, 120, t);

    } else if (strcmp(scentName, "VANILLA") == 0) {
      // creamy white → golden beige
      r = lerp(255, 230, t);
      g = lerp(245, 210, t);
      b = lerp(220, 180, t);
    }

    uint16_t color = tft.color565(r, g, b);
    tft.drawFastHLine(0, y, tft.width(), color);
  }

  // ----- AROMA Title -----
  tft.setFont(&FreeSansOblique9pt7b);
  String aroma = "AROMA";

  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(aroma, 0, 0, &x1, &y1, &w, &h);
  int aromaX = 10;
  int aromaY = 55;

  // Draw black outline
  tft.setTextColor(ST77XX_BLACK);
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (dx == 0 && dy == 0) continue;
      tft.setCursor(aromaX + dx, aromaY + dy);
      tft.print(aroma);
    }
  }

  // Draw white foreground
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(aromaX, aromaY);
  tft.print(aroma);

  // ----- Scent + Water Level -----
  tft.setFont();  // default bitmap font
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_BLACK);

  String scentLine = "SCENT: " + String(scentName);
  String waterLine = "WATER: " + String(waterHigh ? "HIGH" : "LOW");

  int scentY = aromaY + 35;
  int waterY = scentY + 25;

  tft.setCursor(10, scentY);
  tft.print(scentLine);
  tft.setCursor(10, waterY);
  tft.print(waterLine);
}

void setupDisplay() {
  tft.init(170, 320); // For Adafruit 1.9" 320x170 IPS display
  tft.setRotation(3);
  tft.setFont();  // Default font
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
}

void displayState(SystemState state) {
  static SystemState lastState = (SystemState)-1;
  if (state == lastState) return;
  lastState = state;

  tft.setFont();  // default font
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_WHITE);

  switch (state) {
    case OFF:
      drawTitle();
      break;

    case ACTIVE_1:
      drawActiveScreen("LAVENDER", waterSensorFull());
      break;

    case ACTIVE_2:
      drawActiveScreen("CITRUS", waterSensorFull());
      break;

    case ACTIVE_3:
      drawActiveScreen("MINT", waterSensorFull());
      break;

    case ACTIVE_4:
      drawActiveScreen("EUCALYPTUS", waterSensorFull());
      break;

    case ACTIVE_5:
      drawActiveScreen("VANILLA", waterSensorFull());
      break;

    default:
      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(10, 10);
      tft.println("ERROR");
      break;
  }
}