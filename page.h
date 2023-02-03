#ifndef _Page_H_
#define _Page_H_

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "StripObject.h"

#ifndef STASSID
#define STASSID "Alusz IoT"
#define STAPSK ""
#endif


class Page {
private:
  static const int m_Led = 13;

  const char *ssid = STASSID;
  const char *password = STAPSK;

  static AsyncWebServer server;

  static Strip *m_Strip;

  static void Clear();

public:
  Page(Strip *strip) {
    m_Strip = strip;

    pinMode(m_Led, OUTPUT);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Wait for connection
    server.on("/", HTTP_GET, handleRoot);
    server.on("/list", HTTP_GET, handleList);
    server.on("/clear", HTTP_GET, handleClear);
    server.on("/preset", HTTP_GET, handlePreset);
    server.on("/create", HTTP_GET, handleCreate);
    server.on("/delete", HTTP_GET, handleDelete);
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
  }

  static void handleNotFound(AsyncWebServerRequest *request);
  static void handleRoot(AsyncWebServerRequest *request);
  static void handlePreset(AsyncWebServerRequest *request);
  static void handleCreate(AsyncWebServerRequest *request);
  static void handleDelete(AsyncWebServerRequest *request);
  static void handleList(AsyncWebServerRequest *request);
  static void handleClear(AsyncWebServerRequest *request);
};

#endif _Page_H_