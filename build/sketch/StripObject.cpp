#line 1 "D:\\OneDrive - Jan van Zeggelaar\\Arduino\\ESP3266\\LED-Strip-GUI\\StripObject.cpp"
#include "core_esp8266_features.h"
#include <Adafruit_NeoPixel.h>
#include "Strip.h"
#include "StripObject.h"

bool StripObject::CheckObject(int pixel) {
  // For now, stationary object returns true
  // This could change when adding lifetime
  if (getSpeed() == 0) return true;

  // First, update position if needed (check timer)
  unsigned long now = millis();

  unsigned long diff = now - m_MillisLast;
  int interval = 1000 / getSpeed();  


  if (diff > interval) {
    m_MillisLast = millis();

    // increase start depending on duration
    // Serial.printf("%lu >= %i is %s\n", diff, interval, (diff > interval) ? "true" : "false");    
    switch (m_Direction)
    {
      case up:
        m_Start++;
        if (m_Start > m_Max) {
          switch (m_Behavior)
          {
            case looping:
              m_Start = m_Min;
              break;

            case bouncing: 
              m_Direction = down;
              break;

            case once:
              delete this;
              return false;
           }
        }
        break;

      case down:
        m_Start--;
        if (m_Start < m_Min) {
          switch (m_Behavior)
          {
            case looping:
              m_Start = m_Max;  
              
              break;

            case bouncing: 
              m_Direction = up;
              break;

            case once:
              delete this;
              return false;
        }
        break;
      }
    }
  }

  return true;
}

bool StripObject::CheckPixel(int pixel) {
  // Then return true if pixel is within object
  int start = getStart();
  
  if (m_IsCentered) start -= getLen() / 2;

  int end = getStart() + getLen() - 1;

  if (end > m_Strip->getPixelCount()) {
      end -= m_Strip->getPixelCount();
      return (pixel >= start || pixel <= end);
  };

  return (pixel >= start && pixel <= end);
};

void StripObject::SetStart(int start) {
  if (start > (m_Strip->getPixelCount()-1)) 
    start = m_Strip->getPixelCount()-1;
  
  m_Start = start;
}

void StripObject::SetMin(int min) {
  if (min > (m_Strip->getPixelCount()-1)) min = 0;

  m_Min = min;
}

void StripObject::SetMax(int max) {
  if (max > (m_Strip->getPixelCount()-1)) max = m_Strip->getPixelCount() - 1;

  m_Max = max;
}
