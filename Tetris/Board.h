#pragma once

#include "global.h"
#include "Piece.h"

// Board:
// This class is responsible for holding the state of all blocks except the current piece's blocks,
// and clearing full lines out of the game

class Board
{
public:
	Board();
	~Board();

	int GetBlock(int x, int y) { return m_blocks[y][x]; }
	void SetBlock(int x, int y, int value) { m_blocks[y][x] = value; }

	void AddPieceBlocks(Piece piece);
	int ClearFullLines();
	void Render(int x, int y);

private:
	void RemoveLine(int line);
	int m_blocks[BOARD_H][BOARD_W];
};

