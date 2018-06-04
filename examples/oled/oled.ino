#include <AALeC.h>
void setup() {
  aalec.init();
  aalec.print_line(5, "AALeC: Hochschule Aalen");
}

void loop() {
  aalec.print_line(1, millis());
  delay(1);
}

