



#define MATRIX_PIN 5
#define SCL A5
#define SDA A4

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <DS3231.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <WString.h>


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 3, 1, MATRIX_PIN,
	NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE +
	NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
	NEO_GRB + NEO_KHZ800);

DS3231 rtc(SDA, SCL);



const int RED = matrix.Color(255, 0, 0);
const int GREEN = matrix.Color(0, 255, 0);
const int BLUE = matrix.Color(0, 0, 255);

byte mode = 0;
byte setAddress = 0x02; //Address of the set flag in the EEPROM
int x = matrix.width();

template<typename T> scrollTextOnMatrix(T text) //scroll text on the matrix from right to left. See Adafruit Print function for all possible types of arguments
{
	matrix.fillScreen(0);
	matrix.setCursor(x, 0);
	matrix.print(text);
	if (--x < -70) {
		x = matrix.width();
	}
	matrix.show();
	delay(100);
}

template <typename T> showTextOnMatrix(T text, int cursorX = 0) //show text on matrix without scrolling, starting at cursorX position (default 0)
{
	matrix.fillScreen(0);
	matrix.setCursor(cursorX, 0);
	matrix.print(text);
	matrix.show();
	delay(100);
}



void setup() {
  // put your setup code here, to run once:
	matrix.begin();
	matrix.setTextWrap(false);
	matrix.setBrightness(10);
	matrix.setTextColor(BLUE);

	rtc.begin();

	if(EEPROM.read(setAddress) != 1)
	{ 
		rtc.setDOW(THURSDAY);     // Set Day-of-Week to Thursday
		rtc.setTime(21, 00, 0);     // Set the time to 21:00:00 (24hr format)
		rtc.setDate(22, 2, 2018);   //Set date to 22nd of Feb, 2018
		EEPROM.write(setAddress, 1);
	}

	
	
	
	Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

	switch (mode)
	{
	
	case 0:
		Serial.println("in mode 0 - 24hr clock");
		scrollTextOnMatrix(rtc.getTimeStr());
		break;

	case 1:
		Serial.println("in mode 1 - date");
		scrollTextOnMatrix(rtc.getDateStr());
		break;

	case 2:
		Serial.println("In mode 2, Temperature");
		showTextOnMatrix(rtc.getTemp());
		break;
	case 3:
		break;
	}

}
