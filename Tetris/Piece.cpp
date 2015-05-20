#include "Piece.h"
#include "global.h"
#include <allegro5/allegro_primitives.h>

//-----------------------------------------------------------------------------
// Piece definitions:
// heeeeeeere we go
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Sneaky bit level storage magic:
// If you read these 16 bit ints backward (highest to lowest) 4 bits at a time,
// they should form a mask in the shape of the piece.
//
// e.g. for the "I" block:
// 0x2222 = 0 0 1 0
//			0 0 1 0
//			0 0 1 0
//			0 0 1 0
//-----------------------------------------------------------------------------
static const uint16 pieces[PIECE_MAX][4] =
{
	{ 0x2222, 0x0F00, 0x4444, 0x00F0 }, // PIECE_I
	{ 0x6C00, 0x8C40, 0x6C00, 0x8C40 }, // PIECE_S
	{ 0xC600, 0x4C80, 0xC600, 0x4C80 }, // PIECE_Z
	{ 0x8E00, 0x44C0, 0xE200, 0xC880 }, // PIECE_J
	{ 0x2E00, 0xC440, 0x0E80, 0x88C0 }, // PIECE_L
	{ 0x0660, 0x0660, 0x0660, 0x0660 }, // PIECE_O
	{ 0x4E00, 0x4C40, 0x0E40, 0x4640 }, // PIECE_T
};

//-----------------------------------------------------------------------------
// Piece class
//-----------------------------------------------------------------------------
Piece::Piece(PIECETYPE type)
{
	memcpy(m_blockMask, pieces[type], sizeof(m_blockMask));
	switch (type)
	{
	case PIECE_I: m_color = COLOR_CYAN; break;
	case PIECE_S: m_color = COLOR_GREEN; break;
	case PIECE_Z: m_color = COLOR_RED; break;
	case PIECE_J: m_color = COLOR_BLUE; break;
	case PIECE_L: m_color = COLOR_ORANGE; break;
	case PIECE_O: m_color = COLOR_YELLOW; break;
	case PIECE_T: m_color = COLOR_PURPLE; break;
	}
	X = 3;
	Y = 0;
	m_direction = 0;
}


Piece::~Piece()
{
}

bool Piece::OutOfBounds() const
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (GetBlock(i, j))
			{
				int x = X + i;
				int y = Y + j;
				if (x < 0 || x >= BOARD_W || y < 0 || y >= BOARD_H)
					return true;
			}
		}
	}
	return false;
}

// Push the piece back away out from the wall/floor
void Piece::Kick()
{
	bool foundCol;
	do
	{
		foundCol = false;
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				if (GetBlock(i, j))
				{
					int x = X + i;
					int y = Y + j;
					if (x < 0)
					{
						this->X++;
						foundCol = true;
					}
					else if (x >= BOARD_W)
					{
						this->X--;
						foundCol = true;
					}

					if (y >= BOARD_H)
					{
						this->Y--;
						foundCol = true;
					}
				}
			}
		}
	} while (foundCol);
}

extern ALLEGRO_COLOR GetColor(BLOCKCOLOR colour);

void Piece::Render(int x, int y) const
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			int blockx = x + (i+X)*BLOCK_SIZE;
			int blocky = y + (j+Y)*BLOCK_SIZE;
			if (BLOCKCOLOR color = GetBlock(i, j))
			{
				al_draw_filled_rectangle(blockx, blocky, blockx + BLOCK_SIZE, blocky + BLOCK_SIZE, GetColor(color));
				al_draw_rectangle(blockx, blocky, blockx + BLOCK_SIZE, blocky + BLOCK_SIZE, al_map_rgb(255, 255, 255), 1);
			}
		}
	}
}
