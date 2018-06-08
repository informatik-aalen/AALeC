#ifndef __AALEC_H
#define __AALEC_H
#include <NeoPixelBus.h>
#include <SimpleDHT.h>
#include "SSD1306.h"

#define PIN_BUTTON           0
#define PIN_TX               1
#define PIN_RGB_STRIP        2
#define PIN_RX               3
#define PIN_SDA              4
#define PIN_SDC              5
#define PIN_ENCODER_TRACK_1 12
#define PIN_ENCODER_TRACK_2 13
#define PIN_DHT11           14
#define PIN_BEEPER          15
#define PIN_LED_RESET       16

/* Predefined colors for the three rgb LED's */
const RgbColor c_red = RgbColor(200, 0, 0),
               c_green = RgbColor(0, 200, 0),
               c_blue = RgbColor(0, 0, 200),
               c_yellow = RgbColor(100, 100, 0),
               c_white = RgbColor(66, 66, 66),
               c_cyan = RgbColor(0, 100, 100),
               c_purple = RgbColor(100, 0, 100),
               c_off = RgbColor(0, 0, 0);


/* Some predefined frequencies of tones for the beeper */
enum {t_off = 0, t_c_1 = 262, t_d_1 = 294, t_e_1 = 330, t_f_1 = 349,
      t_g_1 = 392, t_a_1 = 440, t_h_1 = 494, t_c_2 = 523, t_a_2 = 880
     };

class c_AALeC {
  public:
    c_AALeC();
    void init();

    /* Methods for the three rgb LED's */
    void set_rgb_strip(int, uint8_t, uint8_t, uint8_t);
    void set_rgb_strip(int, const RgbColor &);
    void set_rgb_strip(int, unsigned int);
    void set_rgb_strip(const RgbColor &, const RgbColor &, const RgbColor &);

    /* Methods for the single LED */
    void set_led(int);
    int get_led();
    void dim_led(int);

    /* Method for the press button on the encoder */
    int get_button();

    /* Methods for the value of the encoder */
    int get_rotate();
    void reset_rotate();

    /* Methods for the temperature and humidity sensor */
    uint8_t get_temp();
    uint8_t get_humidity();

    /* Method for the potentiomether */
    uint16_t get_analog();

    /* Methods for the display */
    template <class T>    void print_line(unsigned int, const T &);
    void clear_display();

    /* Methods for the beeper */
    void play(unsigned int freq);
    void play(unsigned int freq, unsigned int dur);

    /* Interupt service routine for the encoder */
    friend void AALeC_ISR_DREH();

    /* Attribut for the display */
    SSD1306  display = SSD1306  (0x3c, 4, 5);

  private:
    void dht11_mess();

    int drehgeber_int;
    NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart800KbpsMethod> * strip;
    SimpleDHT11 dht11;
    uint8_t temp_int, hum_int;
    uint32_t last_dht11 = -1010;
} ;

template <class T> void c_AALeC::print_line(unsigned int line, const T & s) {
  display.setColor(BLACK);
  display.fillRect(0, ((line - 1) % 5) * 12, 128, 12);
  display.setColor(WHITE);
  display.drawString(0, ((line - 1) % 5) * 12 ,  String(s));
  display.display();
}

/* Default instance of the c_AALeC class */
extern c_AALeC  aalec;

#endif

