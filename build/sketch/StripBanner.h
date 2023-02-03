#line 1 "D:\\OneDrive - Jan van Zeggelaar\\Arduino\\ESP3266\\LED-Strip-GUI\\StripBanner.h"
#ifndef _StripBanner_H_
#define _StripBanner_H_

#include "StripObject.h" 

class StripBanner : public StripObject {
private:
  int m_Width;
  int m_Stripes;
  COLOR m_Color[3];

public:
  StripBanner(int width, Strip *strip, COLOR color1, COLOR color2, COLOR color3, int speed = 0) :
    StripObject(strip, speed) {
      m_Color[0] = color1;
      m_Color[1] = color2;
      m_Color[2] = color3;
      m_Width = width;
      m_Stripes = 3;

      Serial.printf("Create Banner: Width = %i, Color1 = 0x%08lx, Color2 = 0x%08lx\n", m_Width, m_Color[0], m_Color[2]);
  }

  StripBanner(int width, Strip *strip, COLOR color1, COLOR color2, int speed = 0) :
    StripObject(strip, speed) {
      m_Color[0] = color1;
      m_Color[1] = color2;
      m_Width = width;
      m_Stripes = 2;

      Serial.printf("Create Banner: Width = %i, Color1 = 0x%08lx, Color2 = 0x%08lx\n", m_Width, m_Color[0], m_Color[2]);
  }

  virtual COLOR GetPixelColor(int pixel, COLOR color = 0) {
    CheckObject(pixel);    // no need to check, object is full size. So only move

    int i = ((getStrip()->getPixelCount() + pixel - getStart()) / m_Width) % (m_Stripes);
  
    COLOR c = m_Color[i];
    return (c == 0) ? color : c;
  }

  virtual char *getId() {
    return "StripBanner";
  }  

};
#endif