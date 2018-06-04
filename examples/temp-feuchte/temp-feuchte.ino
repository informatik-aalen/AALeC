#include <AALeC.h>
void setup() {
  aalec.init();
  aalec.print_line(5, "AALeC: Hochschule Aalen");
}

void loop() {
  aalec.print_line(1, "Temperatur: " + (String) aalec.get_temp() + "°C");
  aalec.print_line(2, "Feuchte: " + (String) aalec.get_humidity() + "%");
  delay(100);
}

