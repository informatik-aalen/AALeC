#include <Arduino.h>
#include <AALeC.h>
#include <NeoPixelBus.h>
#include <SimpleDHT.h>
#include "SSD1306.h"

c_AALeC aalec;


void ICACHE_RAM_ATTR AALeC_ISR_DREH() {
  static int16_t val, z, n;
  const static int8_t delta[4][4] = {
    {0, 1, 2, 0},
    {0, 1, 0, 3},
    {0, 0, 2, 3},
    {0, 1, 2, 3}
  },
  lambda[4][4] = {
    {  0,  1, -1,  0},
    { -1,  0,  0,  1},
    {  1,  0,  0, -1},
    {  0, -1,  1,  0}
  };
  noInterrupts();
  val = ~GPIO_REG_READ(GPIO_IN_ADDRESS) >> PIN_ENCODER_TRACK_1 & 3;
  n += lambda[z][val], z = delta[z][val];
  if (z == 0) {
    if (n == 4)
      aalec.drehgeber_int--;
    else if (n == -4)
      aalec.drehgeber_int++;
    n = 0;
  }
  interrupts();
}


c_AALeC::c_AALeC() {
}


void c_AALeC::init() {
  pinMode(PIN_BEEPER, OUTPUT);
  pinMode(PIN_LED_RESET, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_ENCODER_TRACK_1, INPUT_PULLUP);
  pinMode(PIN_ENCODER_TRACK_2, INPUT_PULLUP);

  GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, 0xFFFF);
  attachInterrupt(PIN_ENCODER_TRACK_1, AALeC_ISR_DREH, CHANGE);
  attachInterrupt(PIN_ENCODER_TRACK_2, AALeC_ISR_DREH, CHANGE);

  Serial.begin(115200);
  while (!Serial);
  delay(100);
  Serial.println("AALeC init");
  Serial.flush();

  Wire.begin();
  strip = new NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart1Ws2812xMethod>(3, PIN_RGB_STRIP);
  strip->Begin();
  strip->ClearTo(RgbColor(0, 0, 0));
  strip->Show();

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}


void c_AALeC::set_rgb_strip(int led, uint8_t r, uint8_t g, uint8_t b) {
  set_rgb_strip(led, RgbColor(r, g, b));
}


void c_AALeC::set_rgb_strip(int led, const RgbColor & c) {
    if (led >= 0 && led < 3) {
        strip->SetPixelColor(led, c);
        strip->Show();
    }
}


void c_AALeC::set_rgb_strip(int led, unsigned int x) {
  uint16_t r = x % 768, g = (x + 256) % 768, b = (x + 512) % 768;
  RgbColor c(
    (r < 256) ? r : ((r < 512) ? 511 - r : 0),
    (g < 256) ? g : ((g < 512) ? 511 - g : 0),
    (b < 256) ? r : ((b < 512) ? 511 - b : 0)
  );
  set_rgb_strip(led, c);
}


void c_AALeC::set_rgb_strip(const RgbColor & c0, const RgbColor & c1, const RgbColor & c2) {
  strip->SetPixelColor(0, c0);
  strip->SetPixelColor(1, c1);
  strip->SetPixelColor(2, c2);
  strip->Show();
}


void c_AALeC::set_led(int a) {
  digitalWrite(PIN_LED_RESET, a);
}


int c_AALeC::get_led() {
  return digitalRead(PIN_LED_RESET);
}


void c_AALeC::dim_led(int a) {
  analogWrite(PIN_LED_RESET, a);
}


int c_AALeC::get_button() {
  return  ~GPIO_REG_READ(GPIO_IN_ADDRESS)  & 1;
}


int c_AALeC::get_rotate() {
  return drehgeber_int;
}


int c_AALeC::rotate_changed() {
  int rc = 0;
  if (drehgeber_int !=drehgeber_int_alt)
    rc = 1, drehgeber_int_alt = drehgeber_int;
  return rc;
}


void c_AALeC::reset_rotate() {
  drehgeber_int = 0;
}


uint8_t c_AALeC::get_temp() {
  dht11_mess();
  return temp_int;
}


uint8_t c_AALeC::get_humidity() {
  dht11_mess();
  return hum_int;
}


uint16_t c_AALeC::get_analog() {
  return analogRead(A0);
}


void c_AALeC::clear_display() {
  display.clear();
}


void c_AALeC::play(unsigned int f) {
  if (f)
    tone(PIN_BEEPER, f);
  else
    noTone(PIN_BEEPER);
}


void c_AALeC::play(unsigned int f, unsigned int dur) {
  tone(PIN_BEEPER, f, dur);
}


void c_AALeC::dht11_mess() {
  uint32_t t = millis();
  if (t - last_dht11 >= 1000) {
    dht11.read(PIN_DHT11, &temp_int, &hum_int, NULL);
    last_dht11 = t;
  }
}
