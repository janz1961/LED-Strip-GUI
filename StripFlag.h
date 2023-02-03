#ifndef _StripFlag_H_
#define _StripFlag_H_

#include "StripObject.h" 

class StripFlag2 : public StripObject {
private:
  int m_Width;
  COLOR m_Color[4];

public:
  StripFlag2(int width, Strip *strip, COLOR color1, COLOR color2, int speed = 10) :
    StripObject(4 * width, strip, speed) {
      m_Color[0] = strip->Color(0, 0, 0);
      m_Color[1] = color1;
      m_Color[2] = color2;
      m_Color[3] = strip->Color(0, 0, 0);
      m_Width = width;
  }

  COLOR GetPixelColor(int pixel, COLOR color = 0) {
    if (!CheckObject(pixel)) return color;

    pixel =  getLen() + pixel - getStart();   // pixel# within flag
    pixel = pixel / m_Width;    
  
    return m_Color[pixel%3];
  }

  virtual char *getId() {
    return "StripFlag2";
  }  

};

class StripFlag3 : public StripObject {
private:
  int m_Width;
  COLOR m_Color[5];

public:
  StripFlag3(int width, Strip *strip, COLOR color1, COLOR color2, COLOR color3, int speed = 10) :
    StripObject(strip, 5 * width, speed) {
      m_Color[0] = strip->Color(0, 0, 0);
      m_Color[1] = color1;
      m_Color[2] = color2;
      m_Color[3] = color3;
      m_Color[4] = strip->Color(0, 0, 0);
      m_Width = width;
  }

  COLOR GetPixelColor(int pixel, COLOR color = 0) {
    if (!CheckObject(pixel)) return color;

    pixel =  getLen() + pixel - getStart();   // pixel# within flag
    pixel = pixel / m_Width;    
  
    return m_Color[pixel%4];
  }

  virtual char *getId() {
    return "StripFlag3";
  }  

};

#endif