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
* LCD resolution is 320x240
*/


//RGB colors
#define BLUE 0, 200, 204
#define YELLOW 246, 255, 0
#define WHITE 255, 255, 255
#define RED 255, 0, 0
#define GREEN 0, 255, 0



/*
* UTFT is a library that allows user to display images and text on various TFT (Thin-Film Transistor) displays
* UTFT display object will be used to draw components of the game on TFT LCD
*/

UTFT display; 



/**
 * The Pipe class is used to create pipe elements in a game. 
 * The class has properties for the y-coordinate of the pipe, its speed, the height of the upper pipe, 
 * a flag for marking pipes that should not be used, and a flag for marking pipes that have been passed by the player.
 */

class Pipe
{
	public:
	
	/** The y-coordinate of the pipe */
	int y;
	
	/** The speed of the pipe */
	int speed;
	
	/** The height of the upper pipe */
	int upperHeight;
	
	/** A flag for marking pipes that should not be used */
	int dontUse;
	
	/** A flag for marking pipes that have been passed by the player */
	int passed;


	/**
	 * Constructor for the Pipe class.
	 * Initializes the y-coordinate, speed, upper height, passed flag, and dontUse flag of the pipe.
	 */
	Pipe()
	{
		
		y = 220;
		speed = 1;
		upperHeight = rand() % 300;
		passed = 0;
		dontUse = 0;
	}

	/**
	 * Draws the pipe using the current x and y coordinates.
	 * This function sets the color to green and uses the `fillRect` function of the `display` object to draw two rectangles, one for the upper and one for the lower pipe.
	 */
	void drawPipe()
	{
		display.setColor(GREEN);
		display.fillRect(319, this->y, 320 - this->upperHeight, this->y + 10);
		display.fillRect(320 - this->upperHeight - 80, this->y, 0, this->y + 10);
	}
	
	/**
	 * Updates the y-coordinate of the pipe by subtracting the speed from it.
	 * If the y-coordinate becomes less than 0, the dontUse flag is set to 1.
	 */
	void update(){
		this->y = this->y - this->speed;
		if(this->y < 0) this->dontUse = 1;
	}
};


/**
 * An array of 50 Pipe objects.
 */
Pipe pipes[50];

/**
 * A counter for the number of pixels the player has traveled.
 */
int pixelCounter = 0;

/**
 * A counter for the number of pipes that have been generated.
 */
int pipeCount = 1;

/**
 * The current score of the player.
 */
int score = 0;



/**
 * The Bird class is used to create and manage the bird in a game.
 * It has properties for the x- and y-coordinates of the bird, and several functions that update the bird's position and check for collisions with pipes.
 * The class also has a function for drawing the bird and a function for checking if the bird has passed a pipe, updating the score accordingly.
 */

class Bird
{
	public:
	
	/**
     * The x-coordinate of the bird
     */
	int x;
	
	/**
     * The y-coordinate of the bird
     */
	int y;
	
	/**
     * Constructor for the Bird class.
     * Initializes the x and y coordinates to starting position of the bird
     */
	Bird()
	{
		x = 150;
		y = 60;
	}
	
	
	/**
     * Draws the bird using its x and y coordinates
     * Uses the display object's setColor and fillCircle functions to draw a white circle at the bird's position.
     */
	void drawBird()
	{
		display.setColor(WHITE);
		display.fillCircle(x, y, 10);
	}
	
	
	/**
     * Updates the bird's x coordinate based on button presses and gravity.
     * If the button is pressed, the x coordinate is increased by 10. 
     * If the button is not pressed, the x coordinate decreases by 4, simulating gravity.
     * The function also implements collision detection, checking if the bird has collided with a pipe.
     * If there is a collision, the game is ended and the score is displayed.
     */
	void updateBird()
	{
		if((PINB & (1 << PB0)) == 0)
		{
			x += 10;
			_delay_ms(5);
			if(x + 10 > 315){
				x = 310;
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

		//Collision detection
		for (int i = 0; i < pipeCount; i++){
			if(pipes[i].dontUse == 0){
				if((320 - pipes[i].upperHeight < x + 10 || 320 - pipes[i].upperHeight - 80 > x - 10) && pipes[i].y < y + 10 && pipes[i].y + 10 > y - 10){
					
					 // Update the score
					for (int i = 0; i < pipeCount; i++){
						if(pipes[i].passed == 1){
							score++;
						}
					}
					
					 // Display the score and "game over" message
					char scoreString[10];
					sprintf(scoreString, "%d", score);
					display.clrScr();
					display.InitLCD(PORTRAIT);
					display.setColor(WHITE);
					display.setFont(BigFont);
					display.print(scoreString, CENTER, 150);
					display.setColor(RED);
					display.setFont(BigFont);
					display.print("GAME OVER", CENTER, CENTER);
					while(1);
				}
			}
		}

		
	}
	
	/**
     * Checks if the bird has passed a pipe and updates the score if necessary.
     * Goes through the array of pipes and marks the ones that have been passed by the bird.
     */
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
	
	// Button ports
	PORTB = 0xff;
	DDRB = 0x00;
	
	// LCD init
	display.clrScr();
	display.InitLCD(LANDSCAPE);

	// Instance of Bird class
	Bird bird;
	
	// Loading random generated pipes into Pipe buffer
	for(int i = 0; i < 50; i++) {
		pipes[i] = Pipe();
	}

	// The while loop is the main game loop that updates and renders the game elements
	while (1)
	{
		
		// The loop iterates through an array of pipes and draws and updates each pipe if it has not been marked as unused
		for (int i = 0; i < pipeCount; i++){
			if(pipes[i].dontUse == 0){
				pipes[i].drawPipe();
				pipes[i].update();
			}
		}
		
		// The pixelCounter variable is used to keep track of how many pixels the pipes have moved and add new pipes when it reaches a certain value
		pixelCounter++;
		
		if(pixelCounter == 80){
			pipeCount++;
			if(pipeCount > 50) pipeCount = 50;
			pixelCounter = 0;
		}
		
		// The drawBird and updateBird functions are called on the bird object to draw and update the bird's position
		bird.drawBird();
		bird.updateBird();
		
		// The checkScore function is also called on the bird object to see if the bird has passed any pipes
		bird.checkScore();
		
		//The delay function is called to slow down the loop to control the frame rate of the game
		_delay_ms(2);
		
		// The display's clear screen function is called at the end of each iteration to clear the screen for the next frame
		display.clrScr();
	}
}