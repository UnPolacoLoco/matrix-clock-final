// MatrixClock.h

#ifndef _MATRIXCLOCK_h
#define _MATRIXCLOCK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//DI pin for Matrix
#define MATRIX_PIN 2

//RTC pins
#define SCL A5
#define SDA A4

//Joystick pins
#define JOYSTICK_X A1
#define JOYSTICK_Y A2
#define JOYSTICK_BTTN 13

//increase when modes increase
#define NUM_OF_MODES 8




#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Fonts\TomThumb.h>
#include <DS3231.h>
#include <EEPROM.h>
#include "Joystick.h"

struct
{
	//ball starts off mroe or less in the middle of the display.
	int8_t x = 14;
	int8_t y = 4;

	//ball starts off going away from the paddle.
	int8_t momentumX = -1;
	int8_t momentumY = 1;

} ball;

struct paddle
{
	int8_t paddleBlocks[2] = { 2,3 };
	int8_t startX;
	char color;
	uint8_t score = 0;
	int8_t paddleLenght = 2;

};

class MatrixClock
{


public:

	void initialize();

	//RTC Functionality
	void showDate();
	void showFullDate();
	void showTimeAndDate();
	void showTime();
	void showTemp();
	
	//Showing text on the matrices
	void scrollText(String textToScroll, int howFastToScroll = 150);
	void showText(String textToShow);

	//Text and Pixel appearance manupulation
	void changeTextColor();
	void changeBrightess();
	
	//manipulate mode selection. Logic(e.g: change mode when button is pressed) has to be applied in main()
	void changeMode();
	const uint8_t getMode();

	//display buffer to facilitate calling drawPixel methods on each pixel from an array of chars.
	void drawDisplayBuffer(); //TODO possibly a private function
	void clearDisplayBuffer();
	

	void PlayPong();

private:

	//Main modules of the operation. All get created alongside the MatrixClock class.
	Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 3, 1, MATRIX_PIN,
		NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE +
		NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
		NEO_GRB + NEO_KHZ800);
	DS3231 rtc = DS3231(A4, A5);
	Joystick joystick = Joystick(JOYSTICK_X, JOYSTICK_Y, JOYSTICK_BTTN);
	
	//Address of the set flag in the EEPROM. See initialize()
	const uint8_t setAddress = 0x02; 

	//needed to scroll text on matrix
	int x = matrix.width(); 

	//Strings to build an manipulate RTC module responses
	String date;
	String temp;
	String time;

	uint8_t modeCounter = 0;
	uint8_t MAX_BRIGHTNESS = 25;

	
	//list of typical colors to use;
	const uint16_t RED = matrix.Color(255, 0, 0);
	const uint16_t GREEN = matrix.Color(0, 255, 0);
	const uint16_t BLUE = matrix.Color(0, 0, 255);
	const uint16_t YELLOW = matrix.Color(255, 255, 0);
	const uint16_t CYAN = matrix.Color(0, 255, 255);
	const uint16_t MAGENTA = matrix.Color(255, 0, 255);
	const uint16_t WHITE = matrix.Color(255, 255, 255);
	const uint16_t BLACK = matrix.Color(0, 0, 0);

	char displayBuffer[24][8];

	paddle paddle1;
	paddle paddle2;

	void movePaddle(paddle &paddle, int8_t direction);
	void moveBall();
	void resetBall();
	bool didPaddleHitBall(paddle& paddle);

};



#endif

