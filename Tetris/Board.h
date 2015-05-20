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

	BLOCKCOLOR GetBlock(int x, int y) const { return m_blocks[y][x]; }
	void SetBlock(int x, int y, BLOCKCOLOR value) { m_blocks[y][x] = value; }

	void AddPieceBlocks(const Piece& piece);
	int ClearFullLines();
	void Render(int x, int y) const;
	bool PieceIntersects(const Piece& piece) const;

private:
	void RemoveLine(int line);
	BLOCKCOLOR m_blocks[BOARD_H][BOARD_W];
};

