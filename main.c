#include <msp430.h> 
#include "game_shell/game.h"
#include "buttons/button.h"
#include "LCD/LCD.h"

void init_timer();
void init_buttons();
void endOfGame();
void clearTimer();
unsigned char player;
char timerCount = 0;
char flag = 0;
char buttons[] = { BIT1, BIT2, BIT4, BIT0 };


/*
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	char *messageYou = "You";
	char *messageWon = "Won!";
	char *messageGame = "Game";
	char *messageOver = "Over!";

	player = initPlayer();

	initSPI();
	LCDinit();
	LCDclear();

	init_timer();
	init_buttons();
	__enable_interrupt();
	printPlayer(player);

	while (1) {
		if (didPlayerWin(player)) {
			__disable_interrupt();
			clearPlayer(player);
			cursorToLineOne();
			writeString(messageYou);
			cursorToLineTwo();
			writeString(messageWon);
			__delay_cycles(1000000);
			endOfGame();
		}
		if (timerCount >= 4) {
			__disable_interrupt();
			clearPlayer(player);
			cursorToLineOne();
			writeString(messageGame);
			cursorToLineTwo();
			writeString(messageOver);
			__delay_cycles(1000000);
			endOfGame();
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

void endOfGame(){
	char buttonPressed = pollP1Buttons(buttons, 4);
	char stillWaiting = 0;
	while (stillWaiting == 0){
		if(buttonPressed == BIT0 || buttonPressed == BIT1 || buttonPressed == BIT2 || buttonPressed == BIT4){
			waitForP1ButtonRelease(BIT0 | BIT1 | BIT2 | BIT4);
			LCDclear();
			player = initPlayer();
			printPlayer(player);
			stillWaiting = 1;
			__enable_interrupt();
		}
	}
}

void clearTimer() {
	timerCount = 0;
	TACTL |= TACLR;

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
			clearTimer();
			debounce();
		} else {
			debounce();
		}

		P1IES ^= buttonToTest;
		P1IFG &= ~buttonToTest;
	}
}

// Flag for continuous counting is TAIFG
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR() {
	TACTL &= ~TAIFG;            // clear interrupt flag
//	flag = 1;
	timerCount++;

}

void init_timer() {
	TACTL &= ~(MC1 | MC0);        // stop timer

	TACTL |= TACLR;             // clear TAR

	TACTL |= TASSEL1;   // configure for SMCLK - what's the frequency (roughly)?

	TACTL |= ID1 | ID0; // divide clock by 8 - what's the frequency of interrupt?

	TACTL &= ~TAIFG;            // clear interrupt flag

	TACTL |= MC1;               // set count mode to continuous

	TACTL |= TAIE;              // enable interrupt
}

void init_buttons() {
	configureP1PinAsButton(BIT0 | BIT1 | BIT2 | BIT4);
	P1IE |= BIT0 | BIT1 | BIT2 | BIT4;                 // enable the interrupts
	P1IES |= BIT0 | BIT1 | BIT2 | BIT4; // configure interrupt to sense falling edges

	P1IFG &= ~(BIT0 | BIT1 | BIT2 | BIT4);
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void) {
	testAndRespondToButtonPush(BIT0);
	testAndRespondToButtonPush(BIT1);
	testAndRespondToButtonPush(BIT2);
	testAndRespondToButtonPush(BIT4);
}
