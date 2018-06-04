#include <AALeC.h>
void setup() {
  aalec.init();
}

void loop() {
  const static RgbColor ampel[4][3] = { // Achtung LED 0 ist unten und LED 2 ist oben!
    {c_off, c_off, c_red},
    {c_off, c_yellow, c_red},
    {c_green, c_off, c_off},
    {c_off, c_yellow, c_off}
  };
  static uint32_t ampelnr = 0;
  aalec.set_rgb_strip(ampel[ampelnr][0], ampel[ampelnr][1], ampel[ampelnr][2]);
  ampelnr = (ampelnr + 1) % 4;
  delay(2000);
}

