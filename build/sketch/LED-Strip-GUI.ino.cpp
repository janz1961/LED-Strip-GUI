#include <Arduino.h>
#line 1 "D:\\OneDrive - Jan van Zeggelaar\\Arduino\\ESP3266\\LED-Strip-GUI\\LED-Strip-GUI.ino"
// ESP32 version

#include <Adafruit_NeoPixel.h>
#include "Strip.h"
#include "StripObject.h"
#include "StripBlock.h"
#include "StripFlag.h"
#include "StripBanner.h"

#include "Page.h"

#define PIN 4
#define PIXELS 300

// Parameter 1 = aantal LEDs in de strip
// Parameter 2 = pin nummer
// Parameter 3 = RGB LED vlaggen, combineer indien nodig:
//   NEO_KHZ800  800 KHz bitstream (de meeste NeoPixel producten met WS2812 LEDs)
//   NEO_KHZ400  400 KHz (klassieke 'v1' (niet v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     RGB LED volgens GRB bitstream (de meeste NeoPixel produkten)
//   NEO_RGB     RGB LED volgens RGB bitstream (v1 FLORA pixels, niet v2)

Strip strip = Strip(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

Page *g_Page;

int g_Pixel = 0;
long millisLast;

void setup() {
  Serial.begin (115200);
  Serial.println("Start logging for: " __FILE__ ". Version: " __DATE__ " " __TIME__);

  strip.begin();
  strip.setBrightness(32);
  strip.show(); // initialisatie van alle LEDs (resulteert in UIT zetten hier)

  StripObject *object;

#if 0
  object = new StripBanner(16, &strip, strip.Color(2, 0, 0), strip.Color(0, 2, 0), 2);
  object->SetDirection(up);
  object->SetBehavior(bouncing);

  for (int i = 1; i < 15; i++) {
    object = new StripBlock(1, &strip, strip.Color(32, 32, 64), 100 + 3 * i, 55 * i);
    if (i % 3 == 0) {
      object->SetDirection(down);
    }
  }
#endif 
  object = new StripBlock(0, &strip, strip.Color(32, 0, 0), 1);

  strip.Dump();

  g_Page = new Page(&strip);
}

void loop() {
  g_Pixel++;
  if (g_Pixel > strip.getPixelCount()) {
    // For now, show strip after every iteration
    strip.show();
    g_Pixel = 0;
  } 

  COLOR c = 0;

  StripObject *object = strip.getObjectFirst();
  while (object != 0) {
    StripObject *next = object->GetNext();       // Do it tnow in case object gets deleted
    if (object->CheckObject(g_Pixel) && object->CheckPixel(g_Pixel))
      c = object->GetPixelColor(g_Pixel, c);
 
    object = next;
  }

  strip.setPixelColor(g_Pixel, c);
}

