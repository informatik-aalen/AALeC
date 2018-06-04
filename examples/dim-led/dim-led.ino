#include <AALeC.h>
void setup() {
  aalec.init();
}

void loop() {  
  aalec.dim_led(aalec.get_analog());
}

