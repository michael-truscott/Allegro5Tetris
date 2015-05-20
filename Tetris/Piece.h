#pragma once

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

	int GetBlock(int x, int y) const
	{
		return m_blocks[m_direction][y][x];
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
	int m_blocks[4][4][4]; // ugh
	int m_direction;

	int m_oldX, m_oldY, m_oldDir;
};

