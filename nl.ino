#include <FastLED.h>
#define NUM_LEDS 8
#define DATA_PIN 7
#define CLOCK_PIN 5
#define PIR_PIN 12
#define BUZZER_PIN 2
#define LED_PIN 13
int pir = 0;
int ldr = 0;

// here you can change some values, depending on your setup and needs:
int light_threshold = 50; // light threshold, usually 50 with case or 150 without case
int white_bright = 255;   // default brightness for white (0-255)
int buzz_disable = 0;     // change to 1 to switch off initial buzzer
//#define TESTLEDS        // run LED test insteaed of normal program, uncomment to activate

CRGBArray<NUM_LEDS> leds;

void setup()
{
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(12)>(leds, NUM_LEDS);
  FastLED.setBrightness(white_bright); // Global Brightness setting max 255

  pinMode(PIR_PIN, INPUT);     // PIR motion sensor read port
  pinMode(BUZZER_PIN, OUTPUT); // buzzer port
  pinMode(LED_PIN, OUTPUT);    // LED port
  if (buzz_disable == 0)
  {
    digitalWrite(BUZZER_PIN, HIGH); // buzzer ON, if not disabled
  }
  digitalWrite(LED_PIN, HIGH); // PIR LED ON
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW); // buzzer OFF
  digitalWrite(LED_PIN, LOW);    // PIR LED OFF
}

void loop()
{
#ifdef TESTLEDS
  testleds();
#else
  FastLED.clear();
  for (int i = 0; i < NUM_LEDS; i++)
  {                          // iterate through the RGB LEDs
    leds[i].setRGB(0, 0, 0); // set default colour or off
  }
  pir = digitalRead(PIR_PIN); // check PIR
  ldr = analogRead(7);        // read light sensor (0 = very dark, 1023 = very bright)

  if (pir == 1)
  {                              // If motion detected
    digitalWrite(LED_PIN, HIGH); // PIR LED ON
    delay(100);                  // Delay so when dark the LED will flash on for 0.1s
  }
  else
  {
    digitalWrite(LED_PIN, LOW); // PIR LED OFF
  }

  if (pir == 1 && ldr <= light_threshold) // if movement was detected AND dark(ish) with case
  {
    digitalWrite(LED_PIN, LOW); // Don't need PIR LED on now
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i].setRGB(white_bright, white_bright, white_bright);
    }
    FastLED.show();
    delay(18000); // lights on for 18 seconds

    for (int j = white_bright; j > -1; j--)
    {
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i].setRGB(j, j, j); // fading out the white over ~2s
      }
      FastLED.show();
      delay(10); // wait 10ms
    }

    FastLED.clear(); // reset LEDs to 0,0,0
    FastLED.show();  // lights off
  }
  else
  {
    FastLED.show(); // display current LED settings
    delay(500);     // otherwise poll PIR sensor at 2 Hertz
  }
#endif
}

#ifdef TESTLEDS
// this is a simple test program that makes the RGB-LEDS light up in their primary colours.
void testleds()
{
  FastLED.clear();

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].setRGB(255, 0, 0);
    FastLED.show();
  }
  delay(1000);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].setRGB(0, 255, 0);
  }
  FastLED.show();
  delay(1000);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].setRGB(0, 0, 255);
  }
  FastLED.show();
  delay(1000);
}
#endif
