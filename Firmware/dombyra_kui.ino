#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <FastLED.h>

#include "song.h"
#include "webpage.h"


#define PIN_S1        18
#define PIN_S2        19
#define LEDS_PER_STR  18           

#define S1_REVERSED   false
#define S2_REVERSED   false


#define LED_CHIPSET   SK6812
#define COLOR_ORDER   GRB

const char* AP_SSID = "DOMBYRA";
const char* AP_PASS = "dombyra123";     

const CRGB COL_S1     = CRGB(255, 120,  20);   
const CRGB COL_S2     = CRGB( 20, 200, 190);   
const CRGB COL_OPEN   = CRGB(255,   0,   0);   
const uint8_t PREVIEW_SCALE = 26;              
const uint8_t OPEN_SCALE    = 22;              
const uint8_t IDLE_SCALE    = 70;              

CRGB leds1[LEDS_PER_STR];
CRGB leds2[LEDS_PER_STR];
WebServer server(80);

enum Mode : uint8_t { MODE_AUTO = 0, MODE_MIC = 1 };

bool     playing   = false;
Mode     mode      = MODE_AUTO;
uint16_t idx       = 0;             
uint16_t bpm       = 120;
uint8_t  bright    = 70;
bool     loopSong  = true;
uint32_t noteStart = 0;
bool     dirty     = true;

inline uint32_t eighthMs() { return 30000UL / bpm; }   


inline uint8_t ledIndex(uint8_t fret, bool reversed) {
  uint8_t i = fret - 1;                       
  return reversed ? (LEDS_PER_STR - 1 - i) : i;
}

void paintString(CRGB* leds, bool reversed, int8_t fret, CRGB color, uint8_t scale) {
  if (fret < 0) return;                      
  if (fret == 0) {                            
    CRGB c = COL_OPEN;
    c.nscale8(scale8(OPEN_SCALE, scale));
    for (uint8_t i = 0; i < LEDS_PER_STR; i++) leds[i] += c;
    return;
  }
  if (fret > LEDS_PER_STR) return;           
  CRGB c = color;
  c.nscale8(scale);
  leds[ledIndex(fret, reversed)] = c;
}

void render() {
  fill_solid(leds1, LEDS_PER_STR, CRGB::Black);
  fill_solid(leds2, LEDS_PER_STR, CRGB::Black);

  Note cur = noteAt(idx);


  if (idx + 1 < songLength()) {
    Note nx = noteAt(idx + 1);
    paintString(leds1, S1_REVERSED, nx.s1, COL_S1, PREVIEW_SCALE);
    paintString(leds2, S2_REVERSED, nx.s2, COL_S2, PREVIEW_SCALE);
  }

  uint8_t s = playing ? 255 : IDLE_SCALE;
  paintString(leds1, S1_REVERSED, cur.s1, COL_S1, s);
  paintString(leds2, S2_REVERSED, cur.s2, COL_S2, s);

  FastLED.show();
}

void bootSweep() {
  for (uint8_t i = 0; i < LEDS_PER_STR; i++) {
    fill_solid(leds1, LEDS_PER_STR, CRGB::Black);
    fill_solid(leds2, LEDS_PER_STR, CRGB::Black);
    leds1[i] = COL_S1;
    leds2[LEDS_PER_STR - 1 - i] = COL_S2;
    FastLED.show();
    delay(25);
  }
}

void advance() {
  idx++;
  if (idx >= songLength()) {
    if (loopSong) {
      idx = 0;
    } else {
      idx = songLength() - 1;
      playing = false;
    }
  }
  noteStart = millis();
  dirty = true;
}

void restart() {
  idx = 0;
  noteStart = millis();
  dirty = true;
}

// ----------------------------------------------------------------- HTTP ---
void sendState() {
  Note cur = noteAt(idx);

  char n1[8] = "null", n2[8] = "null";
  if (idx + 1 < songLength()) {
    Note nx = noteAt(idx + 1);
    snprintf(n1, sizeof(n1), "%d", nx.s1);
    snprintf(n2, sizeof(n2), "%d", nx.s2);
  }

  char json[240];
  snprintf(json, sizeof(json),
    "{\"playing\":%s,\"mode\":%u,\"bpm\":%u,\"bright\":%u,"
    "\"idx\":%u,\"total\":%u,\"bar\":%u,\"bars\":%u,"
    "\"s1\":%d,\"s2\":%d,\"dir\":%u,\"n1\":%s,\"n2\":%s}",
    playing ? "true" : "false", (unsigned)mode, bpm, bright,
    idx, songLength(), cur.bar, (unsigned)(PART_BARS * SECTION_REPEATS),
    cur.s1, cur.s2, (unsigned)cur.dir, n1, n2);

  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", json);
}

void handleCmd() {
  String c = server.arg("c");
  long   v = server.arg("v").toInt();

  if (c == "start") {
    playing   = true;
    noteStart = millis();
  } else if (c == "stop") {
    playing = false;
  } else if (c == "restart") {
    restart();
    playing = true;
    noteStart = millis();
  } else if (c == "next") {
    if (!playing) playing = true;
    advance();
  } else if (c == "prev") {
    idx = (idx == 0) ? songLength() - 1 : idx - 1;
    noteStart = millis();
  } else if (c == "mode") {
    mode = (v == 1) ? MODE_MIC : MODE_AUTO;
    noteStart = millis();
  } else if (c == "bpm") {
    bpm = constrain(v, 40, 200);
  } else if (c == "bright") {
    bright = constrain(v, 5, 255);
    FastLED.setBrightness(bright);
  } else if (c == "loop") {
    loopSong = (v != 0);
  }
  dirty = true;
  sendState();
}

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<LED_CHIPSET, PIN_S1, COLOR_ORDER>(leds1, LEDS_PER_STR);
  FastLED.addLeds<LED_CHIPSET, PIN_S2, COLOR_ORDER>(leds2, LEDS_PER_STR);
  FastLED.setBrightness(bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);   
  bootSweep();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.printf("AP \"%s\"  ->  http://%s\n",
                AP_SSID, WiFi.softAPIP().toString().c_str());
  if (MDNS.begin("dombyra")) Serial.println("also http://dombyra.local");

  server.on("/", HTTP_GET, []() {
    server.sendHeader("Cache-Control", "no-store");
    server.send_P(200, "text/html", INDEX_HTML);
  });
  server.on("/api/state", HTTP_GET, sendState);
  server.on("/api/cmd",   HTTP_GET, handleCmd);
  server.onNotFound([]() { server.send(404, "text/plain", "404"); });
  server.begin();

  restart();
}

void loop() {
  server.handleClient();

  if (playing && mode == MODE_AUTO) {
    uint32_t due = (uint32_t)noteAt(idx).units * eighthMs();
    if (millis() - noteStart >= due) advance();
  }

  if (dirty) {
    dirty = false;
    render();
  }
}
