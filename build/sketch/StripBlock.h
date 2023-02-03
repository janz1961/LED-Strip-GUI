#line 1 "D:\\OneDrive - Jan van Zeggelaar\\Arduino\\ESP3266\\LED-Strip-GUI\\StripBlock.h"
#ifndef _StripBlock_H_
#define _StripBlock_H_

#include "StripObject.h" 

class StripBlock : public StripObject {
private:
  COLOR m_Color;

public:
  StripBlock(int len, Strip *strip, COLOR color, int speed = 0, int start = 0) :
    StripObject(len, strip, speed, start) {
    m_Color = color;
    if (getLen() == 0) setLen(getStrip()->getPixelCount());
  }

  COLOR GetPixelColor(int pixel, COLOR color = 0) {
    if (!CheckObject(pixel)) return color;

    // Return pixel color unless color is black. Then return supplied color
    return m_Color;
  }

  virtual char *getId() {
    return "StripBlock";
  }  

};

#endif