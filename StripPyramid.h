#ifndef _StripPyramid_H_
#define _StripPyramid_H_

#include "StripObject.h" 

class StripPyramid : public StripObject {
private:
  COLOR m_Color;
  int m_StepR;
  int m_StepG;
  int m_StepB;

public:
  StripPyramid(Strip *strip, COLOR color, int speed = 10, int start = 0) :
    StripObject(strip, speed, start) {
      Serial.printf("Init StripPyramid (%i, 0x%08lx, %i, %i)\n", len, color, speed, start);
      m_Color = color;
      byte* c = (byte *) &m_Color;
      SetCentered();
      Serial.printf("Init StripPyramid - Color: 0x%02x r=0x%02x g=0x%02x b=0x%02x\n", c[3], c[2], c[1], c[0]);

      len = len / 2;
      if (len == 0) len = 1;

      m_StepR = c[2] / len;
      m_StepG = c[1] / len;
      m_StepB = c[0] / len;
      
      if (m_StepR == 0 && c[2] > 0) m_StepR = 1;
      if (m_StepG == 0 && c[1] > 0) m_StepG = 1;
      if (m_StepB == 0 && c[0] > 0) m_StepB = 1;
      
      Serial.printf("Init StripPyramid - Steps: %i, %i, %i\n", m_StepR, m_StepG, m_StepB);
  }

  COLOR GetPixelColor(int pixel, COLOR color = 0) {
    if (!CheckObject(pixel)) return color;

    int r;
    int g;
    int b;
    
    // Return pixel color unless color is black. Then return supplied color
    int p = pixel - getStart();
    
    if (p > (getLen() / 2)) p = getLen() - p;

    r = (p * m_StepR);
    g = (p * m_StepG);
    b = (p * m_StepB);

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    return StripObject::g_Strip.Color(r, g, b);
  }

  virtual char *getId() {
    return "StripPyramid";
  }  

};

#endif