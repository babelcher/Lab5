Lab5
====

Game on the LCD in the Black Box. Accomplishes B Functionality.

## Documentation
I used the buttons library provided by Capt Branchflower as well as the game_shell library that Capt Branchflower provided
and then I modified the code accordingly. C2C Caleb Ziegler helped me identify why my game was resetting into the wrong location 
and suggested that I turn off the interrupts for a short time period. This solved my issue. Other than that, everything 
else was completed by myself.


## Functions
- 'void endOfGame()'
    - Checks the condition for the end of the game
- 'void cleartimer()'
    - Clears the timer
- 'void movePlayerInResponseToButtonPush(char buttonToTest)
    - Moves the player according to the button that was pressed
- 'void testAndRespondToButtonPush(char buttonToTest)'
    - Identifies which button was pressed
- 'void init_timer()'
    - Initializes the timer within the MSP430
- 'void init_buttons()'
    - Initializes the buttons 1 through 4 on pins 1.0, 1.1, 1.2, and 1.4
