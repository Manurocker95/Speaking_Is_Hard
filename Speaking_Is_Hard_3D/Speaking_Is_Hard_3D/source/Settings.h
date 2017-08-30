#pragma once

// Screen size: Top Screen = 400x240; Bottom Screen: 320x240
#define TOP_WIDTH 400
#define BOTTOM_WIDTH 320
#define HEIGHT 240

// Version of our Homebrew and if we want to set a debugmode for us. We can disable it from here.
#define VERSION "1.0"
#define DEBUGMODE true

// 3D Configuration
#define STEREOSCOPIC_3D_ACTIVATED true					// If 3D effect is activated
#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)	// Offset of the 3D slidebar in the console
#define MULTIPLIER_3D 10								// Number of pixels moved by the offset. offset = Multiplier x sliderstate

// Color definition
#define C_BLACK RGBA8(0, 0, 0, 255)
#define C_WHITE RGBA8(255, 255, 255, 255)
#define C_GREY RGBA8(127, 127, 127, 255)
#define C_RED RGBA8(255, 0, 0, 255)
#define C_BLUE RGBA8(0, 0, 255, 255)
#define C_SEMI_RED RGBA8(255, 0, 0, 120) 
#define C_BROWN RGBA8(67, 40, 28, 255)

// Some global variables for texts
#define TAPTEXTSIZE 40
#define MESSAGESIZE 40

// President stuff
#define PRESIDENTMOVEMENT 10
#define NUMOFSENTENCES 4


// Puzzle stuff
#define NUMOFLEVELS 3
#define TIMEPERPUZZLE 300
#define PUZZLEDIFFICULTY 3
#define PUZZLEPENALTY 2