#line 1 "D:\\OneDrive - Jan van Zeggelaar\\Arduino\\ESP3266\\LED-Strip-GUI\\Strip.cpp"
#include <Adafruit_NeoPixel.h>
#include "stripobject.h"
#include "strip.h"

int Strip::getPixelCount()
{
  return m_PixelCount;
}

StripObject *Strip::getObjectFirst() {
    return m_ObjectFirst;
}

void Strip::setObjectFirst(StripObject *object) {
    m_ObjectFirst = object;
}

StripObject *Strip::getObjectLast() {
  return m_ObjectLast;
}

void  Strip::setObjectLast(StripObject *object) {
  m_ObjectLast = object;
}

void Strip::Dump() {
  // Leave this dump in. Add descriptions to classes to dump alongside
  Serial.println("Start dump");
  Serial.printf("First: 0x%lx, Last: 0x%lx\n", m_ObjectFirst, m_ObjectLast);

  StripObject *object = m_ObjectFirst;

  int n = 0;
  while (object != 0)
  {
    n++;
    Serial.printf("Object #%i - 0x%lx:0x%lx|0x%lx\n", n, object, object->GetPrev(), object->GetNext());

    object = object->GetNext();
  }
  Serial.println("End dump");
}

int Strip::g_CountStrips = 0;
