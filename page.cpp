#include <exception>
#include <string.h>
#include "page.h"

#include "StripObject.h"
#include "StripBanner.h"
#include "StripFlag.h"
#include "StripBlock.h"

AsyncWebServer Page::server(80);
Strip *Page::m_Strip = 0;

void Page::handleNotFound(AsyncWebServerRequest *request) {
  digitalWrite(m_Led, 1);
  String message = "Not Found\n\n";
  message += "URI: ";
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->args();
  message += "\n";

  for (uint8_t i = 0; i < request->args(); i++) {
    message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  }

  request->send(404, "text/plain", message);
  digitalWrite(m_Led, 0);
}

void Page::handleClear(AsyncWebServerRequest *request) {
 Clear();
 request->redirect("/");
}

void Page::handleRoot(AsyncWebServerRequest *request) {
  digitalWrite(m_Led, 1);
  
  Serial.println("Start Root");

  for (uint8_t i = 0; i < request->args(); i++) {
    if (request->argName(i).equalsIgnoreCase("clear")) {
      Clear();
    }
    else if (request->argName(i).equalsIgnoreCase("brightness")) {
      Serial.println("Root:Set brightness");
      Serial.printf("Brightness: %s\n", request->arg(i));

      char *sEnd;
      int brightness = (int)strtol(request->arg(i).c_str(), &sEnd, 10);      
      Serial.printf("Set brightness %i\n", brightness);
      m_Strip->setBrightness(brightness);
    }
  }
  
  char temp[2400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  digitalWrite(m_Led, 1);

  snprintf(temp, sizeof(temp),

          "<html>\n\
            <head>\n\
              <title>ESP8266 Demo</title>\n\
              <style>\n\
                body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\n\
              </style>\n\
            </head>\n\
            <body>\n\
              <h1>Hello from ESP8266 v4.1</h1>\n\
              <p>Uptime: %02d:%02d:%02d</p>\n\
              <p><a href='/list'>List</a></p>\n\
              <p><a href='/?clear' >Clear</a></p>\n\
              <p><a href='/?brightness=255' >Brightness max</a></p>\n\
              <p><a href='/?brightness=128' >Brightness 128</a></p>\n\
              <p><a href='/?brightness=64' >Brightness 64</a></p>\n\
              <p><a href='/preset?christmas' >Christmas</a></p>\n\
              <p><a href='/preset?greekparty'>Greek party</a></p>\n\
              <p><button type=\"button\" onclick=\"location.href='/create?Type=Block&Width=8&color=FF0000&Speed=10'\">Add Red Block!</button>\n\</p>\n\
              <p><button type=\"button\" onclick=\"location.href='/create?Type=Block&Width=8&color=00FF00&Speed=10'\">Add Green Block!</button>\n\</p>\n\
              <p><button type=\"button\" onclick=\"location.href='/create?Type=Block&Width=8&color=0000FF&Speed=10'\">Add Blue Block!</button>\n\</p>\n\
              <p><button type=\"button\" onclick=\"location.href='/create?Type=Banner&Width=16&color=FF0000&color=00FF00&Speed=10'\">Add Red Block!</button>\n\</p>\n\
              <p><img src=\"/test.svg\" /></p>\n\
              <p>TODO: Add filesystem and then content</p>\n\
            </body>\n\
          </html>\n",

          hr, min % 60, sec % 60
  );

  request->send(strlen(temp), "text/html", temp); 
  digitalWrite(m_Led, 0);
}

void Page::handlePreset(AsyncWebServerRequest *request) {
  digitalWrite(m_Led, 1);

  Serial.println("Start Preset");
  
  StripObject *object;

  Serial.println("Start parse");

  int j = 0;  
  if (request->args() == 1) {
    if (request->argName(j).equalsIgnoreCase("color")) {
      Serial.println("Start color");
      Serial.printf("Color: %s\n", request->arg(j));

      char *sEnd;
      COLOR color = strtoul(request->arg(j).c_str(), &sEnd, 16);      
      Serial.printf("Create Color (%08x)\n", color);
      new StripBlock(0, m_Strip, color);
    }
    else if (request->argName(j).equalsIgnoreCase("Christmas")) {
      Serial.println("Create Christmas");
      object = new StripBanner(16, m_Strip, m_Strip->Color(2, 0, 0), m_Strip->Color(0, 2, 0), 1);
      object->SetDirection(up);

      for (int i = 1; i < 15; i++) {
        object = new StripBlock(1, m_Strip, m_Strip->Color(16, 16, 16), 100 + 3 * i, 20 * i);
      }
    }
    else if (request->argName(j).equalsIgnoreCase("GreekParty")) {
      Serial.println("Create Greek Party");
      object = new StripBanner(8, m_Strip, m_Strip->Color(0, 0, 2), m_Strip->Color(2, 2, 2), 1);
      object->SetDirection(up);

      object = new StripFlag3(20, m_Strip, m_Strip->Color(64, 0, 0), m_Strip->Color(64, 64, 64), m_Strip->Color(0, 0, 64), 0);
      object->SetStart(400);
      object->SetBehavior(bouncing);

      object = new StripBlock(10, m_Strip, m_Strip->Color(255, 255, 255), 150);

      object = new StripFlag3(6, m_Strip, m_Strip->Color(64, 0, 0), m_Strip->Color(64, 64, 64), m_Strip->Color(0, 0, 64), 60);
      object->SetStart(400);
      object->SetMin(40);
      object->SetMax(875);
      object->SetBehavior(bouncing);

      object = new StripFlag3(6, m_Strip, m_Strip->Color(0, 0, 0), m_Strip->Color(64, 0, 0), m_Strip->Color(64, 64, 0), 70);
      object->SetMin(225);
      object->SetMax(675);
      object->SetBehavior(bouncing);

      object = new StripFlag2(6, m_Strip, m_Strip->Color(64, 64, 0), m_Strip->Color(0, 0, 128), 60);
      object->SetMin(125);
      object->SetMax(775);
      object->SetStart(400);
      object->SetBehavior(bouncing);

      object = new StripFlag3(6, m_Strip, m_Strip->Color(0, 0, 0), m_Strip->Color(64, 0, 0), m_Strip->Color(64, 64, 0), 30);
      object->SetMin(25);
      object->SetMax(875);
      object->SetBehavior(bouncing);
    }    
    else if (request->argName(j).equalsIgnoreCase("preset1")) {
    }    
  }
  digitalWrite(m_Led, 0);
  request->redirect("/");
}

void Page::handleCreate(AsyncWebServerRequest *request) {
  char temp[800];
  
  digitalWrite(m_Led, 1);
  
  Serial.println("Start Create");
  
  String objectType = "Block";
  int objectWidth = 1;
  int objectSpeed = 0;
  int objectPosition = 0;
  DIRECTION objectDirection = up;
  BEHAVIOR objectBehavior = looping;

  int nColor = 0;
  COLOR colors[8];

  String s;

  for (uint8_t i = 0; i < request->args(); i++) {
    if (request->argName(i).equalsIgnoreCase("type")) {
      objectType = request->arg(i);
      Serial.printf("Type: %s\n", objectType);
    }
    else if (request->argName(i).equalsIgnoreCase("width")) {
      objectWidth = request->arg(i).toInt();
      Serial.printf("Width: %i\n", objectWidth);
    }    
    else if (request->argName(i).equalsIgnoreCase("speed")) {
      objectSpeed = request->arg(i).toInt();
      Serial.printf("Width: %i\n", objectSpeed);
    }    
    else if (request->argName(i).equalsIgnoreCase("position")) {
      objectPosition = request->arg(i).toInt();
      Serial.printf("Width: %i\n", objectPosition);
    }    
    else if (request->argName(i).equalsIgnoreCase("direction")) {
      if (request->arg(i).equalsIgnoreCase("up")) {
        objectDirection = up;
      }
      else if (request->arg(i).equalsIgnoreCase("down")) {
        objectDirection = down;
      }
      Serial.printf("Direction: %s\n", (objectDirection == up) ? "Up" : "Down");
    }    
    else if (request->argName(i).equalsIgnoreCase("behavior")) {
      Serial.print("Behavior: ");
      if (request->arg(i).equalsIgnoreCase("looping")) {
        Serial.println("looping");
        objectBehavior = looping;
      }
      else if (request->arg(i).equalsIgnoreCase("bouncing")) {
        Serial.println("bouncing");
        objectBehavior = bouncing;
      }
      else if (request->arg(i).equalsIgnoreCase("once")) {
        Serial.println("once");
        objectBehavior = once;
      }
      else
        Serial.println("<unknown>");
    }    
    else if (request->argName(i) == "color" && nColor < 8) {
      Serial.printf("Color: %s\n", request->arg(i));

      char *sEnd;
      colors[nColor++] = strtoul(request->arg(i).c_str(), &sEnd, 16);      
    }    
  }

  bool isError = false;
  StripObject *object;
  if (objectType.equalsIgnoreCase("banner")) {
    if (nColor == 2) {
      Serial.println("Create Banner!");
      object = new StripBanner(objectWidth, m_Strip, colors[0], colors[1], objectSpeed);
    }
    else { isError = true; }
  }    
  else if (objectType.equalsIgnoreCase("block")){
    if (nColor == 1){
      Serial.println("Create Block!");
      object = new StripBlock(objectWidth, m_Strip, colors[0], objectSpeed);
    }
    else { isError = true; }
  }    
  else if (objectType.equalsIgnoreCase("flag")){
    if (nColor == 2){
      Serial.println("Create Flag 2");
      object = new StripFlag2(objectWidth, m_Strip, colors[0], colors[1], objectSpeed);
    }
    else if (nColor == 3){
      Serial.println("Create Flag 3");
      object = new StripFlag3(objectWidth, m_Strip, colors[0], colors[1], colors[2], objectSpeed);
    }
    else { isError = true; }
  }

  object->SetDirection(objectDirection);
  object->SetBehavior(objectBehavior);

  if (isError) {
    snprintf(temp, sizeof(temp),

            "<html>\
              <head>\
                <title>Created object</title>\
                <style>\
                  body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
                </style>\
              </head>\
              <body>\
                <h1>Error</h1>\
                <a href='/'>Home</a>\n\
                <p>%s</p>\
              </body>\
            </html>",
            "parameter error"
    );
  }
  else {
    char buf[100];
    if (nColor > 0) {
      int offset = snprintf(buf, sizeof(buf), "%06X", colors[0]);
      for (int i = 1; i < nColor; i++){
        offset += snprintf(buf+offset, sizeof(buf)-offset, ", %06X", colors[i]);
      }
    }
    else {
      strcpy(buf, "<no colors>");
    }

    snprintf(temp, sizeof(temp),

            "<html>\
              <head>\
                <title>Created object</title>\
                <style>\
                  body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
                </style>\
              </head>\
              <body>\
                <h1>Created object</h1>\
                <a href='/'>Home</a>\n\
                <h2>Parameters</h2>\
                <p>Type  = %s</p>\
                <p>Width = %i</p>\
                <p>Speed = %i</p>\
                <p>Color = %s</p>\
              </body>\
            </html>",
            objectType.c_str(), objectWidth, objectSpeed, buf
    );
  }

  request->send(strlen(temp), "text/html", temp);

  digitalWrite(m_Led, 0);
}

void Page::handleDelete(AsyncWebServerRequest *request) {
  Serial.println("Start Delete");

  char temp[800];
  
  digitalWrite(m_Led, 1);

  for (uint8_t i = 0; i < request->args(); i++) {
    if (request->argName(i).equalsIgnoreCase("id")) {
      char *sEnd;
      StripObject *object = (StripObject *)(strtoul(request->arg(i).c_str(), &sEnd, 16));      
      Serial.printf("Delete object: %s\n", object->getId());
      delete object;
    }
  }

  request->redirect("/list");
}

void Page::handleList(AsyncWebServerRequest *request) {
  Serial.println("Start List");

  char temp[2800];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  digitalWrite(m_Led, 1);

  strcpy(temp, "<html>\n\
            <head>\n\
              <title>Object list</title>\n\
              <style>\n\
                body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\\
              </style>\n\
            </head>\n\
            <body>\n\
            <h1>Object list</h1>\n\
            <a href='/'>Home</a>\n\
            <table>\n\
            <tr>\n\
            <th>ID</th>\n\
            <th>Length</th>\n\
            <th>Speed</th>\n\
            <th>Action</th>\n\
            </tr>\n");
  
  Serial.println("Start dump");              
  m_Strip->Dump();

  StripObject *object = m_Strip->getObjectFirst();
  while (object != 0) {
    char buf[32];
    char bufPtr[10];

    sprintf(bufPtr, "%08x", object);

    Serial.printf("Object: %s\n", bufPtr);              

    strcat(temp, "<tr>");

    strcat(temp, "<td>");
    strcat(temp, object->getId());
    strcat(temp, "</td>\n");

    sprintf(buf, "%i", object->getLen());
    strcat(temp, "<td>");
    strcat(temp,  buf);
    strcat(temp, "</td>\n");

    sprintf(buf, "%i", object->getSpeed());
    strcat(temp, "<td>");
    strcat(temp,  buf);
    strcat(temp, "</td>\n");

    sprintf(temp+strlen(temp), "<td><a href='/delete?id=%08x'>Delete %08x</a></td>\n", object, object);
    
    strcat(temp, "</tr>");    

    object = object->GetNext();
  }

  Serial.println("End list");              

  strcat(temp, "</table>\n\
            </body>\n\
          </html>\n");

  
  request->send(strlen(temp), "text/html", temp);
  
  digitalWrite(m_Led, 0);
}

void Page::Clear() {
  Serial.println("Start clear");
  StripObject *object;
  while (0 != (object = m_Strip->getObjectFirst())) {
    delete object;
 }
}