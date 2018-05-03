#include <AALeC.h>

void setup() {
  aalec.init();
  aalec.print_line(5, "AALeC: Hochschule Aalen");
}

void loop() {
  // Ansteuern der drei RGB LEDs als Ampel
  const static RgbColor ampel[4][3] = { // Achtung LED 0 ist unten und LED 2 ist oben!
    {c_off, c_off, c_red},
    {c_off, c_yellow, c_red},
    {c_green, c_off, c_off},
    {c_off, c_yellow, c_off}
  };
  static uint32_t t = millis(), ampelnr, b, t_ampel = 1;

  if (millis() - t >= t_ampel) {
    aalec.set_rgb_strip(ampel[ampelnr][0], ampel[ampelnr][1], ampel[ampelnr][2]);
    Serial.println(ampelnr);
    t += t_ampel, ampelnr = (ampelnr + 1) % 4;
  }
  
  // Geschwindigkeit der Ampel ist am Poti einstellbar
  t_ampel = 3 * aalec.get_analog();

  // OLED-Ausgabe über AALeC-Library: Einfach
  aalec.print_line(1, (String) aalec.get_temp() + "°C " + (String) aalec.get_humidity() + "%");
  aalec.print_line(2, (String) aalec.get_button() + " " + (String) aalec.get_rotate());

  // Direktzugriff auf display: Nicht einfach
  aalec.display.setColor(BLACK);
  aalec.display.fillRect(0, 30, 128, 12);
  aalec.display.drawProgressBar(0, 30, 128, 12, (int) (aalec.get_analog() / 10.24));
  aalec.display.display();

  // Lautsprecher und LED test
  if (b != aalec.get_button())
    b = !b, aalec.play((b) ? (aalec.reset_rotate(), t_a_1) : t_off), aalec.set_led(b);
}

