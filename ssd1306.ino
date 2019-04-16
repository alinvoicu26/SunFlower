#include <avr/pgmspace.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <binary.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SPI.h>

#define OLED_ADDR   0x3C

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define max_sets 3

#define encoder0PinA  2
#define encoder0PinB  16
volatile unsigned int encoder0Pos = 0;
// salut

typedef struct
{
	uint8_t valA, valB, valC;
}set_t;

set_t seturi[3];	//values from

int8_t index = 0;	//shown index
//int valA;
//int valB;
//int valC;

byte clk;
byte menuCount = 1;
byte dir = 0;
bool runState = false;

void initVals(void)
{
	for (uint8_t i=0;i<max_sets;i++)
	{
	seturi[i].valA = EEPROM.read(3*i+1);
	seturi[i].valB = EEPROM.read(3*i+2);
	seturi[i].valC = EEPROM.read(3*i+3);
	}
}

void setup() {
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(7, INPUT_PULLUP);
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.display();
  display.clearDisplay();
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoder, RISING);  // encoder pin on interrupt 0 - pin 2
  initVals();
}

void loop() {
  clk = digitalRead(7);
  menuCheck();
  staticMenu();
  display.clearDisplay();
  delay(100);
}

void staticMenu() {
  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(10, 0);
  display.println(" GNS tech");
  //---------------------------------
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.println("Value A:");
  display.setCursor(60, 20);
  display.println(seturi[index].valA);

  display.setCursor(10, 30);
  display.println("Value B:");
  display.setCursor(60, 30);
  display.println(seturi[index].valB);

  display.setCursor(10, 40);
  display.println("Value C:");
  display.setCursor(60, 40);
  display.println(seturi[index].valC);

  /*display.setCursor(10, 50);
  display.println("Start:");
  display.setCursor(45, 50);
  if (encoder0Pos > 5 && menuCount == 4) {
    display.println("Run!");
    runState = true;
  } else {
    display.println("Idle");
    runState = false;
  }*/
  display.setCursor(10, 50);
  display.println("index:");
  display.setCursor(45, 50);
  display.println(index+1);

  display.setCursor(2, (menuCount * 10) + 10);
  display.println(">");



  display.display();
}

void menuCheck() {
  if (clk == LOW && menuCount < 5) {
    menuCount++;
    encoder0Pos = 0;
  }
  if (clk == LOW && menuCount >= 5) {
    menuCount = 1;
  }

  if (menuCount == 1) {
    seturi[index].valA = EEPROM.read(3*index+menuCount);
  }
  if (menuCount == 2) {
	  seturi[index].valB = EEPROM.read(3*index+menuCount);
  }
  if (menuCount == 3) {
	  seturi[index].valC = EEPROM.read(3*index+menuCount);
  }
}

/*void doEncoder() {
  if (digitalRead(encoder0PinA) == HIGH) {
    if (digitalRead(encoder0PinB) == LOW && encoder0Pos > 0) {
      encoder0Pos = encoder0Pos - 1;
      dir = 0;
    }
    else {
      encoder0Pos = encoder0Pos + 1;
      dir = 1;
    }
  }
  else
  {
    if (digitalRead(encoder0PinB) == LOW ) {
      encoder0Pos = encoder0Pos + 1;
      dir = 1;
    }
    else {
      if (encoder0Pos > 0) {
        encoder0Pos = encoder0Pos - 1;
        dir = 0;
      }
    }
  }
}*/

void doEncoder() {
	bool value_encoder_pinA, value_encoder_pinB;
	value_encoder_pinA=digitalRead(2);
	value_encoder_pinB=digitalRead(16);

	if (value_encoder_pinA == value_encoder_pinB)
	{
		switch (menuCount)
		{
		case 1:
			seturi[index].valA--;
		    EEPROM.write(3*index+menuCount, seturi[index].valA);
			break;
		case 2:
			seturi[index].valB--;
		    EEPROM.write(3*index+menuCount, seturi[index].valB);
		    break;
		case 3:
			seturi[index].valC--;
		    EEPROM.write(3*index+menuCount, seturi[index].valC);
		    break;
		case 4:
			index--;
			if (index<0)
				index=0;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (menuCount)
		{
		case 1:
			seturi[index].valA++;
		    EEPROM.write(3*index+menuCount, seturi[index].valA);
			break;
		case 2:
			seturi[index].valB++;
		    EEPROM.write(3*index+menuCount, seturi[index].valB);
		    break;
		case 3:
			seturi[index].valC++;
		    EEPROM.write(3*index+menuCount, seturi[index].valC);
		    break;
		case 4:
			index++;
			if (index>max_sets-1)
				index=max_sets-1;
			break;
		default:
			break;
		}

	}
}
