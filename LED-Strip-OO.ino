// ESP32 version
#include <Adafruit_NeoPixel.h>

#define PIN 4
#define PIXELS 900
#define WITHDUMP

// Parameter 1 = aantal LEDs in de strip
// Parameter 2 = pin nummer
// Parameter 3 = RGB LED vlaggen, combineer indien nodig:
//   NEO_KHZ800  800 KHz bitstream (de meeste NeoPixel producten met WS2812 LEDs)
//   NEO_KHZ400  400 KHz (klassieke 'v1' (niet v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     RGB LED volgens GRB bitstream (de meeste NeoPixel produkten)
//   NEO_RGB     RGB LED volgens RGB bitstream (v1 FLORA pixels, niet v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

int g_pixel;
typedef enum _DIRECTION {up, down} DIRECTION;
typedef enum _BEHAVIOR {looping, bouncing} BEHAVIOR;
typedef uint32_t COLOR;

class StripObject {
  public:
    static StripObject* g_ObjectFirst;
    static StripObject* g_ObjectLast;

#ifdef WITHDUMP
    static void Dump() {
      // Leave this dump in. Add descriptions to classes to dump alongside
      Serial.println("Start dump");
      Serial.printf("First: 0x%lx, Last: 0x%lx\n", g_ObjectFirst, g_ObjectLast);

      StripObject *object = g_ObjectFirst;
      
      int n = 0;
      while (object != 0) {
        n++;
        Serial.printf("Object #%i - 0x%lx:0x%lx|0x%lx\n", n, object, object->m_SOPrev, object->m_SONext);

        object = object->m_SONext;
      }
      Serial.println("End dump");
    }
#endif

  private:
    StripObject* m_SOPrev;
    StripObject* m_SONext;

  private:
    int m_Len;

    int m_Min;
    int m_Max;

    int m_Start;
    int m_Speed; // pixels/s 
    DIRECTION m_Direction;
    BEHAVIOR m_Behavior;

    unsigned long m_MillisLast;

  protected:  
    int get_Speed() {
      return m_Speed;
    }   
    int get_Start() {
      return m_Start;
    }     
    int get_Len() {
      return m_Len;
    }   

  protected:
    StripObject(int len, int speed = 10, int start = 0) {
      m_Len = len;
      m_Start = start;
      m_Speed = speed;
      m_Direction = down;
      m_Behavior = looping;
      SetMin();
      SetMax();
      m_MillisLast = millis();

      if (g_ObjectFirst == 0) {
        g_ObjectFirst = this;
        g_ObjectLast = this;
        m_SOPrev = 0;
      }
      else {
        g_ObjectLast->m_SONext = this;
        m_SOPrev = g_ObjectLast;
        g_ObjectLast = this;
      }

      m_SONext = 0;
    }

    ~StripObject() {
      if (m_SONext == 0) {
        g_ObjectLast = m_SOPrev;
      }

      if (m_SOPrev == 0) {
        // If no previous, Objectlist must opint to next of deleted item
        g_ObjectFirst = m_SONext;
      }
      else {
        // Remove item from list
        m_SOPrev->m_SONext = m_SONext;
        m_SONext->m_SOPrev = m_SOPrev;
      }
    }

    bool CheckObject(int pixel) {
      // First, update position if needed (check timer)
      unsigned long now = millis();
      
      if (get_Speed() > 0 && (now - m_MillisLast) > (1000 / get_Speed())) {
        // increase start depending on duration
        switch (m_Direction)
        {
          case up:
            m_Start++;
            if (m_Start > m_Max) {
              if (m_Behavior == looping)
                m_Start = m_Min;
              else
                m_Direction = down;
            }
            break;

          case down:
            m_Start--;
            if (m_Start < m_Min) {
              if (m_Behavior == looping)
                m_Start = m_Max;
              else
                m_Direction = up;
            }
            break;
        }
        m_MillisLast = millis();
      }

      // Then return true if pixel is within object
      int end = get_Start() + m_Len;
      return (pixel >= get_Start() && pixel <= end);
   };

  public:  
    StripObject *GetNext() {
      return m_SONext;
    }    

    virtual COLOR GetPixelColor(int pixel, COLOR color = 0) = 0;

    virtual void SetDirection(DIRECTION direction) {
      m_Direction = direction;
    }
    virtual void SetBehavior(BEHAVIOR behavior) {
      m_Behavior = behavior;
    }
    void SetStart(int start) {
      if (start > (PIXELS-1)) 
        start = PIXELS-1;
      
      m_Start = start;
    }

    void SetMin(int min = 0) {
      if (min > (PIXELS-1)) min = 0;
      m_Min = min;
    }
    void SetMax(int max = PIXELS-1) {
      if (max > (PIXELS-1)) max = PIXELS - 1;
      m_Max = max;
    }
};

class StripBlock : public StripObject {
private:
  COLOR m_Color;

public:
  StripBlock(int len, COLOR color, int speed = 10, int start = 0) :
    StripObject(len, speed, start) {
      m_Color = color;
  }

  COLOR GetPixelColor(int pixel, COLOR color = 0) {
    if (!CheckObject(pixel)) return color;

    // Return pixel color unless color is black. Then return supplied color
    return m_Color;
  }
};

class StripBanner : public StripObject {
private:
  int m_Width;
  COLOR m_Color[2];

public:
  StripBanner(COLOR color1, COLOR color2, int width, int speed = 10) :
    StripObject(PIXELS, speed) {
      m_Color[0] = color1;
      m_Color[1] = color2;
      m_Width = width;

  }

  COLOR GetPixelColor(int pixel, COLOR color = 0) {
    CheckObject(pixel);    // no need to check, object is full size. So only move

    int p1 = PIXELS + pixel - get_Start();
    int p2 = p1 / m_Width;
  
    COLOR c2 = m_Color[p2 % 2];
    return (c2 == 0) ? color : c2;
  }
};

class StripFlag2 : public StripObject {
private:
  int m_Width;
  COLOR m_Color[4];

public:
  StripFlag2(COLOR color1, COLOR color2, int width, int speed = 10) :
    StripObject(4 * width, speed) {
      m_Color[0] = strip.Color(0, 0, 0);
      m_Color[1] = color1;
      m_Color[2] = color2;
      m_Color[3] = strip.Color(0, 0, 0);
      m_Width = width;
  }

  COLOR GetPixelColor(int pixel, COLOR color = 0) {
    if (!CheckObject(pixel)) return color;

    pixel =  get_Len() + pixel - get_Start();   // pixel# within flag
    pixel = pixel / m_Width;    
  
    return m_Color[pixel%4];
  }
};

class StripFlag3 : public StripObject {
private:
  int m_Width;
  COLOR m_Color[5];

public:
  StripFlag3(COLOR color1, COLOR color2, COLOR color3, int width, int speed = 10) :
    StripObject(5 * width, speed) {
      m_Color[0] = strip.Color(0, 0, 0);
      m_Color[1] = color1;
      m_Color[2] = color2;
      m_Color[3] = color3;
      m_Color[4] = strip.Color(0, 0, 0);
      m_Width = width;
  }

  COLOR GetPixelColor(int pixel, COLOR color = 0) {
    if (!CheckObject(pixel)) return color;

    pixel =  get_Len() + pixel - get_Start();   // pixel# within flag
    pixel = pixel / m_Width;    
  
    return m_Color[pixel%5];
  }
};

StripObject *StripObject::g_ObjectFirst = 0;
StripObject *StripObject::g_ObjectLast = 0;

void setup() {
  Serial.begin (115200);
  Serial.println("Start logging");

  strip.begin();
  strip.setBrightness(255);
  strip.show(); // initialisatie van alle LEDs (resulteert in UIT zetten hier)

  StripObject *object;

  object = new StripBanner(strip.Color(0, 0, 2), strip.Color(2, 2, 2), 8, 1);
  object->SetDirection(up);

  object = new StripFlag3(strip.Color(64, 0, 0), strip.Color(64, 64, 64), strip.Color(0, 0, 64), 20, 0);
  object->SetStart(400);
  object->SetBehavior(bouncing);

  object = new StripBlock(10, strip.Color(255, 255, 255), 150);

  object = new StripFlag3(strip.Color(64, 0, 0), strip.Color(64, 64, 64), strip.Color(0, 0, 64), 6, 60);
  object->SetStart(400);
  object->SetMin(40);
  object->SetMax(875);
  object->SetBehavior(bouncing);

  object = new StripFlag3(strip.Color(0, 0, 0), strip.Color(64, 0, 0), strip.Color(64, 64, 0), 6, 70);
  object->SetMin(225);
  object->SetMax(675);
  object->SetBehavior(bouncing);

  object = new StripFlag2(strip.Color(64, 64, 0), strip.Color(0, 0, 128), 6, 60);
  object->SetMin(125);
  object->SetMax(775);
  object->SetStart(400);
  object->SetBehavior(bouncing);

  object = new StripFlag3(strip.Color(64, 0, 0), strip.Color(64, 64, 64), strip.Color(0, 0, 64), 6, 30);
  object->SetMin(25);
  object->SetMax(875);
  object->SetBehavior(bouncing);

#ifdef WITHDUMP
  StripObject::g_ObjectFirst->Dump();
#endif
  g_pixel = PIXELS;
}

void loop() {
  // Slowdown
  g_pixel++;
  if (g_pixel > PIXELS) {
    // For now, show strip after every iteration
    strip.show();

    g_pixel = 0;
  } 

  COLOR c = 0;

  StripObject *object = StripObject::g_ObjectFirst;
  while (object != 0) {
    c = object->GetPixelColor(g_pixel, c);

    object = object->GetNext();
  }

  strip.setPixelColor(g_pixel, c);
 }
