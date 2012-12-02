#include <TEA5767.h>
#include <Wire.h>
#include <Button.h>
#include <LiquidCrystal.h>

TEA5767 Radio;
LiquidCrystal lcd(12,11,10,9,8,7);
double old_frequency;
double frequency;
int search_mode = 0;
int search_direction;
unsigned long last_pressed;

Button btn_forward(3, PULLUP);
Button btn_backward(2, PULLUP);

void setup() {  
  Wire.begin();
  Radio.init();
  Radio.set_frequency(105.4); 
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
}

void loop() {

  unsigned char buf[5];
  int stereo;
  int signal_level;
  double current_freq;
  unsigned long current_millis = millis();
  
  if (Radio.read_status(buf) == 1) {
    current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
    stereo = Radio.stereo(buf);
    signal_level = Radio.signal_level(buf);
    lcd.setCursor(0,0);
    lcd.print("FM: "); lcd.print(current_freq);
    lcd.setCursor(0,1);
    if (stereo) lcd.print("STEREO "); else lcd.print("MONO ");
    //lcd.print(signal_level);
  }
  
  if (search_mode == 1) {
      if (Radio.process_search (buf, search_direction) == 1) {
          search_mode = 0;
      }
  }
  
  if (btn_forward.isPressed()) {
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_UP;
    Radio.search_up(buf);
    delay(300);
  }
  
  if (btn_backward.isPressed()) {
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_DOWN;
    Radio.search_down(buf);
    delay(300);
  } 
  //delay(20); 
  delay(50);
}
