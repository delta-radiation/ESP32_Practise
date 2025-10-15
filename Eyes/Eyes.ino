#include <U8g2lib.h>
#include <Wire.h>

#define LED_PIN 8
#define BUTTON_PIN 5

// Software I2C on SDA=5, SCL=6
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 6, /* data=*/ 5, /* reset=*/ U8X8_PIN_NONE);

struct Eye {
  int x, y;         // center
  int w, h;         // full width & height
  bool isLeft;        // true if left eye, false if right
	int currentH = 0;   // current height for animation
	bool closing = false;     // are we closing the eye
  unsigned long lastUpdate = 0;
  int blinkInterval = 1;  // time between blinks
  int blinkSpeed = 500;     // how fast the eye closes/opens

  void update() {
    unsigned long now = millis();

    // start a blink after interval
    if (!closing && now - lastUpdate > blinkInterval) {
      closing = true;
      lastUpdate = now;
    }

    if (closing) {
      // animate closing
      if (currentH < 25) {
        currentH += 1;
        if (currentH < 1) currentH = 1;
      } else {
        closing = false; // start opening
        lastUpdate = now;
      }
    } else {
      // animate opening
      if (currentH > 0) {
        currentH -= 1;
      }
    }
  }


  void draw(U8G2 &display) { 
		
		// draw big white oval 
		display.setDrawColor(1); // 1 = white 
		u8g2.drawFilledEllipse(x, y, 15, 12); 

		if (!isLeft) { 
			display.drawBox(x + w/4 - 1, y - h / 2 - 6, 11, 10); 
			display.setDrawColor(0);   // black
    	display.drawBox(x + w/4 - 21, y - h / 2 - 7, 31, 1 + currentH);
		} else { 
			display.drawBox(x + w/4 - 21, y - h / 2 - 6, 11, 10); 
			display.setDrawColor(0);   // black
    	display.drawBox(x + w/4 - 21, y - h / 2 - 7, 31, 1);
		}

		

	}
};

Eye leftEye(40, 32, 24, 12, true, 12);
Eye rightEye(88, 32, 24, 12, false, 12);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // internal pull-up
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  u8g2.begin();
}

void loop() {
  u8g2.clearBuffer();

  // Update eyes
  leftEye.update();
  rightEye.update();

  // Draw eyes
  leftEye.draw(u8g2);
  rightEye.draw(u8g2);

  u8g2.sendBuffer();
  delay(20); // smooth refresh
}