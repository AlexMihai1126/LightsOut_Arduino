# Lights Out Arduino puzzle game
Task given was to implement a game on an 8x8 LED matrix with an Arduino that has a user interface menu on an LCD screen. Controls available: joystick and pushbutton. I decided to implement a Lights Out clone (a retro handheld console, see video of it here: https://www.youtube.com/watch?v=bkr_npwf2JU). It features a random board generator, a free drawing mode (the whole matrix is available for you to draw whatever you want using the joystick) and a short demo mode so you can familiarize yourself with the game mechanics and practice on a simple board.

## How to play
To start, press the RED button while in the Start Game menu.

In random board mode, the LED matrix will display a 5x5 game board that has random LEDs turned ON (between 20% and 80% of them are turned on at random). Your goal is to find a way to turn off ALL the LEDs on the board. The game ends when the board is completely off. The "highscore" is when you achieve solving the puzzle in the fewest number of moves. Game mechanics: you select a pixel (its X and Y coordinates are also shown on the screen) and when you click it, the pixel and its neighbors (UP, DOWN, LEFT, RIGHT) will toggle states. It's quite a challenge to turn off all the LEDs and will put your mind to work! You can quit the game at any time by clicking on the red button.

In free drawing mode, you select a pixel with the joystick, then click it to toggle it ON or OFF. No rules, just an 8x8 canvas to draw whatever you'd like. There is no goal in this mode, just be creative! The game ends whenever you click the red button, your creation will be displayed as long as you want before clicking again to reset the game.

In demo mode, you get a very very simple lights out board to practice the game before entering the real challenge in random mode. Same rules, just extremely simplified.

## Picture of the setup:

![](https://github.com/AlexMihai1126/LightsOut_Arduino/blob/main/Images/20231218_210305153_iOS.jpg)
![](https://github.com/AlexMihai1126/LightsOut_Arduino/blob/main/Images/20231218_210237389_iOS.jpg)

## Video showcasing the functionality: 

https://youtu.be/M0Rja9_QZQ0

## Components used:
8x8 LED matrix with MAX7219 driver, 16x2 LCD module (and 330 ohm resistor for current limiting the backlight LED), joystick, red pushbutton, Arduino UNO, breadboard for the internal wiring, a 10k potentiometer for contrast adjusting the display, passive buzzer (with 100 ohm limit resistor), light sensor (for auto brightness of the displays) and lots of foamboard for making the enclosure.

## Known issues:

LEDs do not blink properly when selected

Auto brightness controller does not work as expected (and is currently disabled)
