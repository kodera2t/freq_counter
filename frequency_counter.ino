#include <FreqCounter.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 7
Adafruit_SSD1306 display(OLED_RESET);


int terminal_1  = 2;
int terminal_2  = 4;
volatile int encorder_val = 0;
volatile int mode_set = 0;
volatile int band_mode = LOW;
volatile char old_state = 0;
boolean event = 0;

void setup() {
  pinMode(terminal_1, INPUT);
  pinMode(terminal_2, INPUT);
  attachInterrupt(0, Rotary_encorder, CHANGE);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(32,32);
    display.display();
}
  int valnum=4;
long int frq;
double measfreq;
int gate_time=2000;
void loop() {

 FreqCounter::f_comp= 0;             
 FreqCounter::start(gate_time);            
 while (FreqCounter::f_ready == 0)      
 
 frq=FreqCounter::f_freq;            // read result
 measfreq=double((frq)/(1.000*gate_time+0.0061));
// measfreq=frq;
 delay(200);
    display.fillRect(0,0,128,64,BLACK); 
    display.setTextColor(WHITE);
    display.setCursor(16,16);
    display.print("GPS trained");
    display.setCursor(14,30);
     display.print("freq. counter");   
    display.setCursor(0,45);
    display.print("gating:");
    display.print(gate_time);
    display.print(" ms");
    display.setCursor(0,63);
    display.print(measfreq,valnum); 
    display.print(" kHz");
    display.display();

    if(event == 1){
        display.clearDisplay();
    switch (encorder_val) {
      case 0:
    gate_time=100;
    display.setCursor(0,45);
    display.print("gating:");
    display.print(gate_time);
    display.print(" ms");
    valnum=2;
        display.display();
        break;
      case 1:
        gate_time=1000;  
    display.setCursor(0,45);
    display.print("gating:");
    display.print(gate_time);
    display.print(" ms");
        valnum=4;
        display.display();
        break;
      case 2:
        gate_time=2000;  
    display.setCursor(0,45);
    display.print("gating:");
    display.print(gate_time);
    display.print(" ms");
            valnum=4;
        display.display();
        break;

      case 3:
        gate_time=3000;  
    display.setCursor(0,45);
    display.print("gating:");
    display.print(gate_time);
    display.print(" ms");
            valnum=4;
        display.display();
        break;
      default:
      break;   
    }     
      event = 0;
    }
}


void Rotary_encorder(void)
{
  if (!digitalRead(terminal_1)) {
    if (digitalRead(terminal_2)) {
      old_state = 'R';
    } else {
      old_state = 'L';
    }
  } else {
    if (digitalRead(terminal_2)) {
      if (old_state == 'L') {
        encorder_val++;
      }
    } else {
      if (old_state == 'R') {
        encorder_val--;
      }
    }

    if (encorder_val < 0) {
      encorder_val = 3;
    }
    if (encorder_val > 3) {
      encorder_val = 0;
    }
    old_state = 0;
    event = 1;
  }

}
