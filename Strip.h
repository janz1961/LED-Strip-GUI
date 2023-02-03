#ifndef _Strip_h_
#define _Strip_h_

#include <Adafruit_NeoPixel.h>
class StripObject;

typedef uint32_t COLOR;

class Strip : public Adafruit_NeoPixel
{
private:
  static int g_CountStrips;

  StripObject *m_ObjectFirst;
  StripObject *m_ObjectLast;
  int m_PixelCount;

public:
  Strip(int pixelCount, int pin, int brightness) :
    Adafruit_NeoPixel(pixelCount, pin, NEO_GRB + NEO_KHZ800)
  {
    m_ObjectFirst = 0;
    m_ObjectLast = 0;
    m_PixelCount = pixelCount;

    begin();
    setBrightness(brightness);
    show(); // initialisatie van alle LEDs (resulteert in UIT zetten hier)
  }

  int           getPixelCount();
  
  void          setObjectFirst(StripObject *object);
  StripObject  *getObjectFirst();
  void          setObjectLast(StripObject *object);
  StripObject  *getObjectLast();

  void Dump();
};

#endif