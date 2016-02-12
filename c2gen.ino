/*
Copyright (C) 2016 Benjamin Larsson <benjamin@southpole.se>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/* c2gen simple c2 signal driver for arduino */

#define C2D_PIN 10
#define C2CK_PIN 11
#define LED_PIN 13

void setup()
{
  pinMode(LED_PIN, OUTPUT);  // led
  pinMode(C2CK_PIN, OUTPUT);  // clock
  pinMode(12, OUTPUT);  // clock
  
  // start serial port at 115200 bps and wait for port to open:
  Serial.begin(115200);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for Leonardo only
//  }
//  Serial.println("c2gen 0.1");
//  Serial.println("Copyright (C) 2016");  
//  Serial.println("by Benjamin Larsson");
}

void loop()
{
  char c2d, inChar, t, data, ret;
  unsigned char udata;
  unsigned char udata_str[2];
  int i;

  // read serial input
  if (Serial.available() > 0) {
    inChar = (char)Serial.read();

    // handle commands
    switch(inChar) {
      case 'c':
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        break;
      case 'C':
        PORTB = PORTB | B00001000;     // pin 11 (clock)
        break;
      case 'i':
        pinMode(C2D_PIN, INPUT);
        c2d = digitalRead(C2D_PIN) + '0';
        Serial.print(c2d);
        break;
      case 'o':
        pinMode(C2D_PIN, OUTPUT);
        PORTB = PORTB & ~(B00100100);  // pin 10 (data+led)
        break;
      case 'O':
        pinMode(C2D_PIN, OUTPUT);
        PORTB = PORTB | B00100100;     // pin 10 (data+led)
        break;
      case 's':
        PORTB = PORTB | B00001000;     // pin 11 (clock)
        /* strobe pulse low for 250nS with this code */
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)
        break;
      case 'r':
        PORTB = PORTB | B00001000;     // pin 11 (clock)
        delayMicroseconds(2);
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        delayMicroseconds(20);
        PORTB = PORTB | B00001000;     // pin 11 (clock)
        delayMicroseconds(2);
        break;
      case 'm':
        Serial.println("c2gen 0.1");
        break;
      case 'D':
        /* write_dr */
        pinMode(C2D_PIN, OUTPUT);
        /* Start field */
        PORTB = PORTB | B00100100;     // 1 -> pin 10 (data+led)
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)

        /* Ins field */
//        PORTB = PORTB | B00100100;     // 1 -> pin 10 (data+led)
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)
        PORTB = PORTB & ~(B00100100);  // 0 -> pin 10 (data+led)
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)

        /* Length field */
//        PORTB = PORTB & ~(B00100100);  // 0 -> pin 10 (data+led)
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)
//        PORTB = PORTB & ~(B00100100);  // 0 -> pin 10 (data+led)
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)   

        /* Data field */
        do {
        } while (Serial.available() == 0);
        data = (char)Serial.read();

        
        for (i = 0; i < 8; i++) {
          if ((unsigned char)data & 1) {
            PORTB = PORTB | B00100100;     // 1 -> pin 10 (data+led)
            PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
            PORTB = PORTB | B00001000;     // pin 11 (clock)
//            Serial.print('1');                    
          } else {
            PORTB = PORTB & ~(B00100100);  // 0 -> pin 10 (data+led)
            PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
            PORTB = PORTB | B00001000;     // pin 11 (clock)
//            Serial.print('0');               
          }
          data = (unsigned char) data >> 1;
        }

        /* Wait field */
        PORTB = PORTB | B00100100;     // 1 -> pin 10 (data+led)
        t = 40;
        pinMode(C2D_PIN, INPUT);
        while (t>0) {     
          PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
          PORTB = PORTB | B00001000;     // pin 11 (clock)
          if (digitalRead(C2D_PIN))
            break;
          delayMicroseconds(1);
          t--;
          if (t == 0)
            Serial.print('1');           
        }

        /* Stop field */
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)

        Serial.print('0');
        break;
      case 'A':
        /* read_ar */
//        pinMode(C2D_PIN, OUTPUT);
        DDRB =   B00101100;
        /* Start field */
        PORTB = PORTB | B00100100;     // 1 -> pin 10 (data+led)
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)

        /* Ins field */
        PORTB = PORTB & ~(B00100100);  // 0 -> pin 10 (data+led)
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)
        PORTB = PORTB | B00100100;     // 1 -> pin 10 (data+led)
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)

        /* Adress field */
//        PORTB = PORTB | B00100100;     // 1 -> pin 10 (data+led)
        udata = 0;
//        pinMode(C2D_PIN, INPUT);
        DDRB =  B00101000;
        for (i = 0; i < 8; i++) {
          udata = udata >> 1;
          PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
          PORTB = PORTB | B00001000;     // pin 11 (clock)
          delayMicroseconds(1);
//          if (digitalRead(C2D_PIN))
          if (PINB & B00000100)
            udata = udata | 0x80;
        }

        /* Stop field */
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)

        Serial.print((char)udata); 
        break;
      case 'd':
        /* read dr */
        ret = '0';
        pinMode(C2D_PIN, OUTPUT);
        /* Start field */
        PORTB = PORTB | B00100100;     // 1 -> pin 10 (data+led)
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)

        /* Ins field */
        PORTB = PORTB & ~(B00100100);  // 0 -> pin 10 (data+led)
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)
//        PORTB = PORTB & ~(B00100100);  // 0 -> pin 10 (data+led)
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)

        /* Length field */
//        PORTB = PORTB & ~(B00100100);  // 0 -> pin 10 (data+led)
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)
//        PORTB = PORTB & ~(B00100100);  // 0 -> pin 10 (data+led)
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)

        /* Wait field */
        PORTB = PORTB | B00100100;     // 1 -> pin 10 (data+led)
        t = 40;
        pinMode(C2D_PIN, INPUT);
        while (t>0) {     
          PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
          PORTB = PORTB | B00001000;     // pin 11 (clock)
          if (digitalRead(C2D_PIN))
            break;
          delayMicroseconds(1);
          t--;
          if (t == 0) {
            ret = '1';
            break;
          }
        }

        /* Data field */
        udata = 0;
        for (i = 0; i < 8; i++) {
          udata = udata >> 1;
          PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
          PORTB = PORTB | B00001000;     // pin 11 (clock)
//          delayMicroseconds(100);
          if (digitalRead(C2D_PIN))
            udata = udata | 0x80;
        }
        /* Stop field */
        PORTB = PORTB & ~(B00001000);  // pin 11 (clock)
        PORTB = PORTB | B00001000;     // pin 11 (clock)

        Serial.print((char)(udata));
        Serial.print(ret);
        break;        
    }
  }
}

