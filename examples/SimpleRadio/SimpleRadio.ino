#include <TEA5767.h>
#include <Wire.h>
#include <Button.h>

TEA5767 Radio;
Button BtnForward(2, PULLUP);
Button BtnBackward(3, PULLUP);

int search_mode = 0;
int search_direction = 1;

void setup() {
  Radio.set_frequency(88.5);
  Serial.begin(9600);
}

void loop() {
  unsigned char buf[5];
  int stereo;
  int signal_level;
  double current_freq;
  unsigned long current_millis;
  
  if (Radio.read_status (buf) == 1) {
    
      current_freq = floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
      stereo = Radio.stereo(buf);
      signal_level = Radio.signal_level(buf);
      
      Serial.print("Frequency: ");
      Serial.println(current_freq);
      
      Serial.print("Stereo: ");
      Serial.println(stereo);

      Serial.print("Signal: ");
      Serial.println(signal_level);
      
      if (search_mode == 1) {
        if (Radio.process_search (buf, search_direction) == 1) {
            search_mode = 0;
         }
      }
   }

  if (BtnForward.isPressed()) {
      search_mode = 1;
      search_direction = 1;
      Radio.search_up (buf);
  }


  if (BtnBackward.isPressed()) {
      search_mode = 1;
      search_direction = 2;
      Radio.search_down (buf);
  }
  
}
