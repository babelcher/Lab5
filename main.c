#include <msp430.h> 
#include "game_shell/game.h"
#include "buttons/button.h"
#include "LCD/LCD.h"

void init_timer();
void init_buttons();
unsigned char player;
/*
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	char *messageYou = "You";
	char *messageWon = "Won!";

	player = initPlayer();

	initSPI();
	LCDinit();
	LCDclear();

	init_timer();
	init_buttons();
	__enable_interrupt();
	printPlayer(player);

	while (1) {
		while (!didPlayerWin(player)) {

		}
		cursorToLineOne();
		writeString(messageYou);
		cursorToLineTwo();
		writeString(messageWon);
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

void movePlayerInResponseToButtonPush(char buttonToTest) {
	clearPlayer(player);
	player = movePlayer(player, buttonToTest);
	printPlayer(player);
}

void testAndRespondToButtonPush(char buttonToTest) {
	if (buttonToTest & P1IFG) {
		if (buttonToTest & P1IES) {
			movePlayerInResponseToButtonPush(buttonToTest);
			//           clearTimer();
		} else {
			debounce();
		}

		P1IES ^= buttonToTest;
		P1IFG &= ~buttonToTest;
	}
}

//
// YOUR TIMER A ISR GOES HERE
//
void init_timer() {
	// do timer initialization work
}
void init_buttons() {
	configureP1PinAsButton(BIT0 | BIT1 | BIT2 | BIT4);
	P1IE |= BIT0 | BIT1 | BIT2 | BIT4;                 // enable the interrupts
	P1IES |= BIT0 | BIT1 | BIT2 | BIT4; // configure interrupt to sense falling edges

	//	P1REN |= BIT1 | BIT2 | BIT3;    // enable internal pull-up/pull-down network
	//	P1OUT |= BIT1 | BIT2 | BIT3;                   // configure as pull-up

	P1IFG &= ~(BIT0 | BIT1 | BIT2 | BIT4);
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void) {
	testAndRespondToButtonPush(BIT0);
	testAndRespondToButtonPush(BIT1);
	testAndRespondToButtonPush(BIT2);
	testAndRespondToButtonPush(BIT4);
}
