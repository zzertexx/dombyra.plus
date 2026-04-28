#include <FastLED.h>
#include "strokes.h"
#include <driver/i2s.h>
#include <WiFi.h>
#include <WebServer.h>

#define NUM_LEDS 19
#define GPIO_UPPER 23
#define GPIO_LOWER 22

#define I2S_WS 25
#define I2S_SCK 32
#define I2S_SD 33

#define SAMPLE_SIZE 512
int32_t samples[SAMPLE_SIZE];

float previousVolume = 0;
#define ONSET_THRESHOLD 0.02  // tune this later

const char* ssid = "Dombyra";
const char* password ="qwerty1234";
WebServer server(80);

CRGB upper[NUM_LEDS];
CRGB lower[NUM_LEDS];

int currentKui = -1;
int totalKuis = sizeof(kuis) / sizeof(kuis[0]);
int currentStroke = 0;
bool isPlaying = false;
unsigned long lastOnsetTime = 0;
const int RESET_TIMEOUT = 30000;

void updateLEDs() {
  if (currentKui == -1) return;
  FastLED.clear();
  Stroke s = kuis[currentKui].strokes[currentStroke];
  CRGB color;  
    if (currentStroke + 1 < kuis[currentKui].totalStrokes) {
      Stroke next = kuis[currentKui].strokes[currentStroke + 1];
      if (next.isUp) {
        color = CRGB::Red;
      } else {
        color = CRGB::Green;
      }

      color.nscale8(60);

        if (next.upper != 0) {
        upper[next.upper - 1]  = color; 
        }
        if (next.bottom != 0) {
        lower[next.bottom - 1]  = color; 
        }
    }

  if (s.isUp) {
    color = CRGB::Red;
  } else {
    color = CRGB::Green;
  }

    if (s.upper != 0) {
    upper[s.upper - 1]  = color; 
    }
    if (s.bottom != 0) {
    lower[s.bottom - 1]  = color; 
    }
  FastLED.show();
}

void readAudio() {
  size_t bytesRead;
  i2s_read(I2S_NUM_0, samples, sizeof(samples), &bytesRead, portMAX_DELAY);
}

float calculateVolume() {
  float sum = 0;
  for (int i = 0; i < SAMPLE_SIZE; i++) {
    float sample = (float)samples[i] / INT32_MAX;
    sum += sample * sample; 
  }
  return sqrt(sum / SAMPLE_SIZE); 
}

bool detectOnset() {
  float currentVolume = calculateVolume();
  bool onset = (currentVolume - previousVolume) > ONSET_THRESHOLD;
  previousVolume = currentVolume;
  return onset;
}

void handleRoot() {
  String html = "<h1>Dombyra</h1>";

  for (int i = 0; i < totalKuis; i++) {
    html += "<button>";
    html += kuis[i].name;
    html += " — ";
    html += kuis[i].totalStrokes;
    html += " notes</button>";
  }

  html += "<button onclick=\"location.href='/launch'\">Launch</button>";
  server.send(200, "text/html", html);  
}

void handleSelect() {
  if (server.hasArg("kui")) {
    currentKui = server.arg("kui").toInt();
    isPlaying = true;
    currentStroke = 0;
    lastOnsetTime = millis();
    server.send(200, "text/html", "Started!");
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  server.on("/", handleRoot);
  server.on("/select", handleSelect);
  server.begin();

  FastLED.addLeds<SK6812, GPIO_UPPER>(upper, NUM_LEDS);
  FastLED.addLeds<SK6812, GPIO_LOWER>(lower, NUM_LEDS);

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
    };

i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
i2s_set_pin(I2S_NUM_0, NULL); 
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

  if (isPlaying) {
    readAudio();
    if (detectOnset()) {
      currentStroke++;
      updateLEDs();
      lastOnsetTime = millis();
    }
    if (millis() - lastOnsetTime > RESET_TIMEOUT) {
      isPlaying = false;
      currentKui = -1;
      currentStroke = 0;
      FastLED.clear();
      FastLED.show();
    }
  }
}
  