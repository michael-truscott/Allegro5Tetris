#ifndef GLOBAL_H
#define GLOBAL_H

typedef unsigned short int uint16;

#define SCREEN_W 640
#define SCREEN_H 480

#define BOARD_W 10
#define BOARD_H 20

#define FRAMES_PER_SEC 20.0f

#define BLOCK_SIZE 20

#define AUTOFALL_FRAMES_DEFAULT 20

// TODO: define some block colours
enum BLOCKCOLOR
{
	COLOR_NONE = 0,
	COLOR_CYAN,
	COLOR_YELLOW,
	COLOR_PURPLE,
	COLOR_GREEN,
	COLOR_RED,
	COLOR_BLUE,
	COLOR_ORANGE,
};

#endif