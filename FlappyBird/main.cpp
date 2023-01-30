#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "UTFT/UTFT.h"
#include "UTFT/color.h"
#include "UTFT/DefaultFonts.h"
#include "UTFT/ssd1289.h"
#include <time.h>
#include <stdio.h>


/*
* RGB colors
*/

#define BLUE 0, 200, 204
#define YELLOW 246, 255, 0
#define WHITE 255, 255, 255
#define RED 255, 0, 0
#define GREEN 0, 255, 0



/*
* Global variables
*/

UTFT display; //Display class

//lcd display is 320x240


class Pipe
{
	public:
	
	int y;
	int speed;
	int upperHeight;
	int dontUse;
	int passed;

	Pipe()
	{
		
		y = 220;
		speed = 1;
		upperHeight = rand() % 300;
		passed = 0;
		dontUse = 0;
	}

	//create drawPipe function that will draw the pipe with pipes current x and y coordinates
	void drawPipe()
	{
		display.setColor(GREEN);
		//display.fillRect(200, 150, 0, 60); //prve dve koordinate za top left tocku
		// druge dvije za bottom right tocku
		//display.fillRect(200,150,0, 210);
		display.fillRect(319, this->y, 320 - this->upperHeight, this->y + 10);
		display.fillRect(320 - this->upperHeight - 80, this->y, 0, this->y + 10);
		//draw second pipe
		//display.fillRect(x, height + 100, width, 240 - height);
	}
	void update(){
		this->y = this->y - this->speed;
		if(this->y < 0) this->dontUse = 1;

	}
};

Pipe pipes[50];
int pixelCounter = 0;
int pipeCount = 1;
int score = 0;

class Bird
{
	public:
	int x;
	int y;
	Bird()
	{
		x = 150;
		y = 60;
	}
	//create drawBird function that will draw the bird with birds current x and y coordinates
	void drawBird()
	{
		display.setColor(WHITE);
		display.fillCircle(x, y, 10);
	}
	//create updateBird function that will update the x coordinate of the bird when button is pressed else the bird will fall at a constant rate determined by gravity
	void updateBird()
	{
		if((PINB & (1 << PB0)) == 0)
		{
			x += 10;
			_delay_ms(5);
			if(x + 10 > 315){
				x=310;
			}
		}
		else
		{
			x -= 4;
			_delay_ms(5);
			if(x - 10 < 5){
				x = 5;
			}
			
			
		}

		//add collision detection
		for (int i = 0; i < pipeCount; i++){
			if(pipes[i].dontUse == 0){
				if((320 - pipes[i].upperHeight < x + 10 || 320 - pipes[i].upperHeight - 80 > x - 10) && pipes[i].y < y + 10 && pipes[i].y + 10 > y - 10){
					for (int i = 0; i < pipeCount; i++){
						if(pipes[i].passed == 1){
							score++;
						}
					}
					//take integer score and make it string
					char scoreString[10];
					sprintf(scoreString, "%d", score);

					display.clrScr();
					display.InitLCD(PORTRAIT);
					display.setColor(WHITE);
					display.setFont(BigFont);
					display.print(scoreString, CENTER, 150);
					display.setColor(RED);
					display.setFont(BigFont);
					//rotate the display to landscape mode
					display.print("GAME OVER", CENTER, CENTER);
					while(1);
				}
			}
		}

		
	}
	//add function that will check if bird has passed the pipe and add to score
	void checkScore(){
		for (int i = 0; i < pipeCount; i++){
			if(pipes[i].dontUse == 0){
				if(pipes[i].y + 10 < this->y - 10){
					pipes[i].passed = 1;
				}
			}
		}
	}
};

int main(void)
{
	
	//SET PORTS
	PORTB = 0xff;
	DDRB = 0x00;
	
	//LCD INIT
	display.clrScr();
	display.InitLCD(LANDSCAPE);

	
	Bird bird;
	
	for(int i = 0; i < 50; i++) {
		pipes[i] = Pipe();
	}

	
	while (1)
	{
		//if((PINB & (1 << PB0)) == 0)
		for (int i = 0; i < pipeCount; i++){
			if(pipes[i].dontUse == 0){
				pipes[i].drawPipe();
				pipes[i].update();
			}
		}
		pixelCounter++;
		if(pixelCounter == 80){
			pipeCount++;
			if(pipeCount > 50) pipeCount = 50;
			pixelCounter = 0;
		}
		bird.drawBird();
		bird.updateBird();
		bird.checkScore();
		_delay_ms(2);
		display.clrScr();
	}
}