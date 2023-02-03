#ifndef _StripObject_h_
#define _StripObject_h_

#include <Adafruit_NeoPixel.h>
#include "Strip.h"

class Strip;

typedef enum _DIRECTION {up, down} DIRECTION;
typedef enum _BEHAVIOR {looping, bouncing, once} BEHAVIOR;
typedef enum _LIFETIME {single, limited, forever} LIFETIME;
typedef uint32_t COLOR;

class StripObject {
protected:
  StripObject(Strip *strip, int speed = 10, int start = 0) {
    Init(strip, strip->getPixelCount(), speed, start);
  }

  StripObject(int len, Strip *strip, int speed = 10, int start = 0) {
    Init(strip, len, speed, start);
  }

  Strip *getStrip() {
    return m_Strip;
  }   
  
public:
  ~StripObject() {
    if (GetNext() == 0) {
      m_Strip->setObjectLast(GetPrev());
    }
    else {
      GetNext()->SetPrev(GetPrev());
    }

    if (GetPrev() == 0) {
      // If no previous, Objectlist must point to next of deleted item
      m_Strip->setObjectFirst(GetNext());
    }
    else {
      // Remove item from list
      GetPrev()->SetNext(GetNext());
    }
  }

  virtual bool CheckObject(int pixel);
  virtual bool CheckPixel(int pixel);
  
private:
  Strip *m_Strip;

  int m_Len;

  int m_Min;
  int m_Max;

  int m_Start;
  int m_Speed; // pixels/s 
  DIRECTION m_Direction;
  BEHAVIOR m_Behavior;
  LIFETIME m_Lifetime;
  bool m_IsCentered;

  unsigned long m_MillisLast;

  StripObject *m_SOPrev;
  StripObject *m_SONext;

public:  
  int getSpeed() {
    return m_Speed;
  }   
  int getStart() {
    return m_Start;
  }     
  
  int getLen() {
    return m_Len;
  }   
  
  int setLen(int len) {
    return m_Len = len;
  }   

  virtual char *getId() {
    return "StripObject";
  }  

public:
  StripObject *GetNext() {
    return m_SONext;
  }    

  void SetNext(StripObject *object) {
    m_SONext = object;
  }    

  StripObject *GetPrev() {
    return m_SOPrev;
  }    

  void SetPrev(StripObject *object) {
    m_SOPrev = object;
  }    

  virtual COLOR GetPixelColor(int pixel, COLOR color = 0) = 0;

  virtual void SetDirection(DIRECTION direction) {
    m_Direction = direction;
  }
  
  virtual void SetBehavior(BEHAVIOR behavior) {
    m_Behavior = behavior;
  }

  virtual void SetLifetime(LIFETIME lifetime) {
    m_Lifetime = lifetime;
  }

  virtual void SetStart(int start);
  virtual void SetMin(int min = 0);
  virtual void SetMax(int max = 10000);
  
protected:
  void virtual SetCentered(bool b = true) {
    m_IsCentered = b;
  }

private:
  void Init(Strip *strip, int len, int speed, int start) {
    m_Strip = strip;
    m_Len = len;
    m_Start = start;
    m_Speed = speed;
    m_Direction = down;
    m_Behavior = looping;
    SetMin();
    SetMax();
    m_MillisLast = millis();

    if (m_Strip->getObjectFirst() == 0) {
      m_Strip->setObjectFirst(this);
      m_Strip->setObjectLast(this);
      m_SOPrev = 0;
    }
    else {
      m_SOPrev = m_Strip->getObjectLast();
      m_Strip->getObjectLast()->SetNext(this);
      m_Strip->setObjectLast(this);
    }

    m_SONext = 0;
  }
};
#endif