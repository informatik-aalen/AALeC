#include <AALeC.h>
void setup() {
  aalec.init();
}

void loop() {
  static int nr;
  const static int toene [][2] = {
    {t_c_1, 500}, {t_e_1, 500}, {t_g_1, 250}, {t_a_1, 750},  {t_c_2, 2000},
  };
  aalec.play(toene[nr][0], toene[nr][1] - 20);
  delay(toene[nr][1]);
  nr = (nr + 1) % 5;
}
