#include<FastLED.h>
#define NUM_LEDS 8
#define DATA_PIN 7
#define CLOCK_PIN 5
int pir = 0;
int ldr = 0;
int white_bright = 255;       // default brightness for white (0-255)
int buzz_disable = 0;         // change to 0 to switch on initial buzzer

CRGBArray<NUM_LEDS> leds;

void setup() { 
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(12)>(leds, NUM_LEDS);
  FastLED.setBrightness(white_bright); // Global Brightness setting max 255 

  pinMode(12, INPUT);         // PIR motion sensor read port 
  pinMode(2, OUTPUT);         // buzzer port
  pinMode(13, OUTPUT);        // LED port
  if (buzz_disable == 0) {
     digitalWrite(2, HIGH);   // buzzer ON, if not disabled        
  }
  digitalWrite(13, HIGH);     // PIR LED ON
  delay(1000);
  digitalWrite(2, LOW);       // buzzer OFF
  digitalWrite(13, LOW);      // PIR LED OFF
}

void loop() {
      FastLED.clear();
      for(int i=0; i<NUM_LEDS; i++){           // iterate through the RGB LEDs
         leds[i].setRGB(0, 0, 0);              // set default colour or off
      }
      pir = digitalRead(12);                   // check PIR
      ldr = analogRead(7);                     // read light sensor
      if (pir == 1){                           // If motion detected
        digitalWrite(13, HIGH);                // PIR LED ON
        delay(100);                            // Delay so when dark the LED will flash on for 0.1s
      }
      else{
        digitalWrite(13, LOW);                 // PIR LED OFF
      }
      //if (pir == 1 && ldr <= 150 ) {         // if someone there AND dark(ish) no case
      if (pir == 1 && ldr <= 50 ) {            // if someone there AND dark(ish) with case
        digitalWrite(13, LOW);                 // Don't need PIR LED on now
        for(int i=0; i<NUM_LEDS; i++){
            leds[i].setRGB(white_bright, white_bright, white_bright);     // half-bright white
            //leds[i].setRGB(255, 255, 255);   // full-bright white sometimes gives PIR false-positives
        }                                      // depends on LED position, PIR orientation and case location
        FastLED.show();
        delay(18000);                          // lights on for about 20 seconds

        for(int j=white_bright; j>-1; j--){ 
           for(int i=0; i<NUM_LEDS; i++){
              leds[i].setRGB(j, j, j);         // fading out the white over ~2s
           }
          FastLED.show();
          delay(10);                           // wait 10ms
        }

        FastLED.clear();                       // reset LEDs to 0,0,0
        FastLED.show();                        // lights off
        //delay(2000);                         // don't really need a pause here but can help with PIR fps
      }
      else {
        FastLED.show();                        // display current LED settings
        delay(500);                            // otherwise poll read PIR at 2 Hertz
      }
}
