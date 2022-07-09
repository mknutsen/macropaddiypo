#include <Adafruit_SH110X.h>
#include <Adafruit_NeoPixel.h>
#include <RotaryEncoder.h>
#include <Wire.h>
#include <SPI.h>
#define PIN_ROTA 17
#define PIN_ROTB 18
#define PIN_NEOPIXEL 19
#define PIN_SPEAKER 16
#define PIN_SPEAKER_ENABLE 14
#define PIN_SWITCH 0
#define NUM_NEOPIXEL 12
#define OLED_DC 24
#define OLED_RST 23
#define OLED_CS 22

#define mosi_pin 27
#define sclk_pin 26
#define dc_pin 24
#define rst_pin 23
#define cs_pin 22

// Create the neopixel strip with the built in definitions NUM_NEOPIXEL and PIN_NEOPIXEL
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

uint8_t j = 0;
String reading = "";

// Create the OLED display
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, mosi_pin, sclk_pin, dc_pin, rst_pin, cs_pin);

// Create the rotary encoder
RotaryEncoder encoder(PIN_ROTA, PIN_ROTB, RotaryEncoder::LatchMode::FOUR3);
// void checkPosition() {  
//   Serial.print("checking\n");
//   for (int i = 1; i <= 12; i++)
//   {
//     if (digitalRead(i) == LOW)
//     {
//       // pixels.setPixelColor((i - 1), 124,252,0);

//       pixels.setPixelColor((i - 1), 0,0,0);
//     }
//     else
//     {
//       pixels.setPixelColor((i - 1), 0,0,0);
//     }
    
//   }
//   pixels.show();
//  }
// our encoder position state
int encoder_pos = 0;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  delay(100);  // RP2040 delay is not a bad idea
  Serial.print("hi\n");
  //while (!Serial) { delay(10); }     // wait till serial port is opened
//  int availableBytes = Serial.available();
  // start pixels!
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'

  for (uint8_t i=0; i<=12; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  pinMode(PIN_ROTA, INPUT_PULLUP);
  pinMode(PIN_ROTB, INPUT_PULLUP);
  
  // Start OLED
  display.begin(0, true); // we dont use the i2c address but we will reset!
  display.display();
}

void loop()
{
  if (Serial.available() > 0) {
      reading.concat(Serial.readString());
      int newLinePos = reading.indexOf("\n");
      int index = reading.indexOf("abc123");
      if (newLinePos >= 0 && index < newLinePos)
      {
        int num = reading.substring(index, newLinePos).substring(9).toInt();
        if (reading.indexOf("abc123-i"))
        {
          pixels.setPixelColor(num - 1, 124,252,0);
        }
        else if (reading.indexOf("abc123-d"))
        {
          pixels.setPixelColor(num - 1, 0, 0, 0);
          
        }
        
        reading = "";
        
      }
      else if (index > newLinePos)
      {
        reading = "";
        
      }
      
    
  }

  for (int i = 1; i <= 12; i++)
  {
    if (digitalRead(i) == LOW)
    {
       pixels.setPixelColor((i - 1), 124,252,0);
       
       char buffer[14];
       sprintf(buffer, "abc123-p-%d", i);
       Serial.println(buffer);
    }
    else
    {
      pixels.setPixelColor((i - 1), 0,0,0);
    }
    
  }
  pixels.show();

  pixels.show();
  
  display.display();
}
