#include "Board.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

Board::Board()
{
	memset(m_blocks, 0, sizeof(m_blocks));
}


Board::~Board()
{
}

void Board::AddPieceBlocks(const Piece& piece)
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			BLOCKCOLOR temp = piece.GetBlock(i, j);
			if (temp)
				SetBlock(piece.X + i, piece.Y + j, temp);
		}
	}
}

bool Board::PieceIntersects(const Piece& piece) const
{
	// Warning: will probably compare out-of-bounds memory if the piece is outside the game area,
	// so make sure any bound-correcting adjustment is done beforehand
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (piece.GetBlock(i, j))
			{
				int x = piece.X + i;
				int y = piece.Y + j;
				if (this->GetBlock(x, y))
					return true;
			}
		}
	}
	return false;
}

int Board::ClearFullLines()
{
	int result = 0;
	for (int j = 0; j < BOARD_H; j++)
	{
		bool linefull = true;
		for (int i = 0; i < BOARD_W; i++)
		{
			if (!GetBlock(i, j))
			{
				linefull = false;
				break;
			}
		}

		if (linefull)
		{
			RemoveLine(j);
			result++;
		}
	}
	return result;
}

void Board::RemoveLine(int line)
{
	// Erase all the blocks on this lines and move all lines on top of this one down
	for (int x = 0; x < BOARD_W; x++)
	{
		// Erase the block in the line
		SetBlock(x, line, COLOR_NONE);
		// Move the other blocks down
		for (int i = line; i > 0; i--)
			SetBlock(x, i, GetBlock(x, i - 1));
	}
}

ALLEGRO_COLOR GetColor(BLOCKCOLOR colour)
{
	switch (colour)
	{
	case COLOR_NONE:
		return al_map_rgb(0, 0, 0);
		break;
	case COLOR_CYAN:
		return al_map_rgb(0, 255, 255);
		break;
	case COLOR_YELLOW:
		return al_map_rgb(255, 255, 0);
		break;
	case COLOR_PURPLE:
		return al_map_rgb(255, 0, 255);
		break;
	case COLOR_GREEN:
		return al_map_rgb(0, 255, 0);
		break;
	case COLOR_RED:
		return al_map_rgb(255, 0, 0);
		break;
	case COLOR_BLUE:
		return al_map_rgb(0, 0, 255);
		break;
	case COLOR_ORANGE:
		return al_map_rgb(255, 0xA5, 0);
		break;
	default:
		return al_map_rgb(0, 0, 0);
		break;
	}
}

void Board::Render(int x, int y) const
{
	for (int j = 0; j < BOARD_H; j++)
	{
		for (int i = 0; i < BOARD_W; i++)
		{
			int blockx = x + i*BLOCK_SIZE;
			int blocky = y + j*BLOCK_SIZE;
			if (BLOCKCOLOR color = GetBlock(i, j))
				al_draw_filled_rectangle(blockx, blocky, blockx + BLOCK_SIZE, blocky + BLOCK_SIZE, GetColor(color));
			al_draw_rectangle(blockx, blocky, blockx + BLOCK_SIZE, blocky + BLOCK_SIZE, al_map_rgb(255, 255, 255), 1);
		}
	}
}
