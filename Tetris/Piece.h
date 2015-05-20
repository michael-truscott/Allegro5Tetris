#pragma once

#include "global.h"

enum PIECETYPE
{
	PIECE_I,
	PIECE_S,
	PIECE_Z,
	PIECE_J,
	PIECE_L,
	PIECE_O,
	PIECE_T,
	PIECE_MAX,
};

// Piece:
// This class represents the current piece in play, using 4 4x4 int arrays (1 for each rotation state).

class Piece
{
public:
	Piece(PIECETYPE type);
	~Piece();

	void RotateL()
	{
		m_direction++;
		if (m_direction > 3)
			m_direction = 0;
	}
	void RotateR()
	{
		m_direction--;
		if (m_direction < 0)
			m_direction = 3;
	}

	BLOCKCOLOR GetBlock(int x, int y) const
	{
		return m_blockMask[m_direction] & (1 << (15 - (y * 4 + x))) ? m_color : COLOR_NONE; // hehe
	}

	bool OutOfBounds() const;
	void Kick();
	void SaveState()
	{
		m_oldX = X;
		m_oldY = Y;
		m_oldDir = m_direction;
	}
	void RevertState()
	{
		X = m_oldX;
		Y = m_oldY;
		m_direction = m_oldDir;
	}

	void Render(int x, int y) const;

	int X, Y;	// Public, since we'll be modifying these directly anyway
private:
	uint16 m_blockMask[4];
	int m_blocks[4][4][4]; // ugh
	int m_direction;
	BLOCKCOLOR m_color;

	int m_oldX, m_oldY, m_oldDir;
};

