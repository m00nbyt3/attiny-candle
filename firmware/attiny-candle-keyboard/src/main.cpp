
#include "DigiKeyboard.h"

#ifdef RGB_ENABLED
#include <light_ws2812.h>

#define MAX_BRT 0x20

cRGB RGB_COLOR_GREEN = {MAX_BRT, 0, 0};
cRGB RGB_COLOR_WHITE = {MAX_BRT, MAX_BRT, MAX_BRT};
cRGB RGB_COLOR_BLACK = {0, 0, 0};
cRGB RGB_COLOR_BLUE = {0, 0, MAX_BRT};
cRGB RGB_COLOR_RED = {0, MAX_BRT, 0};

cRGB leds[NUM_PIXELS];
#endif

#ifdef OLED_ENABLED
#include <TinyWireM.h>
#include <Tiny4kOLED_TinyWireM.h>

#define OLED_WIDTH 128
#define OLED_WIDTH_COLS (OLED_WIDTH >> 2)
#define OLED_HEIGHT 32
#define OLED_HEIGHT_ROWS (OLED_HEIGHT >> 3)

#endif

void setup()
{
#ifdef PIN_BTN_1
  pinMode(PIN_BTN_1, INPUT);
#endif
#ifdef OLED_ENABLED
  TinyWireM.begin();
  oled.begin(OLED_WIDTH, OLED_HEIGHT, sizeof(tiny4koled_init_128x32br), tiny4koled_init_128x32br);
  oled.setFont(FONT6X8CAPS);
  oled.on();
  oled.switchRenderFrame();
#endif
}

uint8_t btn_1_state = HIGH;
uint8_t enabled_state = true;
uint8_t seq = 0;

void update_visual()
{
#ifdef RGB_ENABLED
  if (enabled_state)
  {
    for (uint8_t i = 0; i < NUM_PIXELS; i++)
    {
      leds[i] = (seq % NUM_PIXELS) == i ? RGB_COLOR_GREEN : RGB_COLOR_BLACK;
    }
  }
  else
  {
    for (uint8_t i = 0; i < NUM_PIXELS; i++)
    {
      leds[i] = (seq % NUM_PIXELS) == i ? RGB_COLOR_RED : RGB_COLOR_BLACK;
    }
  }
  ws2812_setleds(leds, NUM_PIXELS);
#endif

#ifdef OLED_ENABLED
  oled.clear();
  if (enabled_state)
  {
    oled.setCursor((seq % OLED_WIDTH_COLS) << 2, seq % OLED_HEIGHT_ROWS);
    oled.print("ON");
  }
  else
  {
    oled.setCursor((OLED_WIDTH_COLS >> 1) << 2, OLED_HEIGHT_ROWS >> 1);
    oled.print("OFF");
  }
  oled.switchFrame();
#endif
}

void loop()
{
#ifdef PIN_BTN_1
  uint8_t btn_state = digitalRead(PIN_BTN_1);
  if (btn_state != btn_1_state)
  {
    if (btn_1_state == LOW)
      enabled_state = !enabled_state;
    btn_1_state = btn_state;
  }
#endif

  DigiKeyboard.sendKeyStroke(0);

  if (enabled_state)
  {
    switch (seq++ % 4)
    {
    case 0:
      DigiKeyboard.sendKeyStroke(KEY_F11);
      break;
    case 1:
      DigiKeyboard.sendKeyStroke(KEY_F11);
      break;
    case 2:
      DigiKeyboard.sendKeyStroke(KEY_F11);
      break;
    case 3:
      DigiKeyboard.sendKeyStroke(KEY_F11);
      break;
    }
  }

  update_visual();

  DigiKeyboard.delay(150);
}

// void loop() {
//   // this is generally not necessary but with some older systems it seems to
//   // prevent missing the first character after a delay:
//

//   // Type out this string letter by letter on the computer (assumes US-style
//   // keyboard)
//   DigiKeyboard.println("Hello Digispark!");

//   // It's better to use DigiKeyboard.delay() over the regular Arduino delay()
//   // if doing keyboard stuff because it keeps talking to the computer to make
//   // sure the computer knows the keyboard is alive and connected
//   DigiKeyboard.delay(5000);
// }
