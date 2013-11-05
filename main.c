#include <msp430.h> 
#include "game.h"
#include "buttons/button.h"
#include "LCD/LCD.h"

/*
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	unsigned char player = initPlayer();

	initSPI();
	LCDinit();
	LCDclear();

	init_timer();

	configureP1PinAsButton(BIT0 | BIT1 | BIT2 | BIT4);
	P1IE |= BIT0 | BIT1 | BIT2 | BIT4;                 // enable the interrupts
	P1IES |= BIT0 | BIT1 | BIT2 | BIT4; // configure interrupt to sense falling edges

//	P1REN |= BIT1 | BIT2 | BIT3;    // enable internal pull-up/pull-down network
//	P1OUT |= BIT1 | BIT2 | BIT3;                   // configure as pull-up

	P1IFG &= ~(BIT0 | BIT1 | BIT2 | BIT4);
	__enable_interrupt();
	printPlayer(player);

	while (1) {
		while (didPlayerWin(player) != 0xC7) {

		}
		/*
		 *
		 * {
		 * 		check if button is pushed (you don't want to block here, so don't poll!)
		 * 		if button is pushed:
		 * 			clear current player marker
		 * 			update player position based on direction
		 * 			print new player
		 * 			clear two second timer
		 * 			wait for button release (you can poll here)
		 * }
		 *
		 * print game over or you won, depending on game result
		 *
		 * wait for button press to begin new game (you can poll here)
		 * wait for release before starting again
		 */
	}

	return 0;
}
