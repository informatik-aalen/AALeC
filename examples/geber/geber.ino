#include <AALeC.h>
void setup() {
  aalec.init();
  aalec.print_line(3, "Lange drücken: Reset");
  aalec.print_line(5, "AALeC: Hochschule Aalen");
}

void loop() {
  static uint32_t b, t;
  aalec.print_line(1, "Analog: " + (String) aalec.get_analog());
  aalec.print_line(2, (String) aalec.get_button() + " " + (String) aalec.get_rotate());
  if (b != aalec.get_button()) {
    b = aalec.get_button();
    if (b)
      t = millis();
  }
  if (b & millis() - t > 1000)
      aalec.reset_rotate();
}

