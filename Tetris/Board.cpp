#include "Board.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

Board::Board()
{
	for (int j = 0; j < BOARD_H; j++)
	{
		for (int i = 0; i < BOARD_W; i++)
		{
			m_blocks[j][i] = 0;
		}
	}
}


Board::~Board()
{
}

void Board::AddPieceBlocks(Piece piece)
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			int temp = piece.GetBlock(i, j);
			if (temp)
				SetBlock(piece.X + i, piece.Y + j, temp);
		}
	}
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
		SetBlock(x, line, 0);
		// Move the other blocks down
		for (int i = line; i > 0; i--)
			SetBlock(x, i, GetBlock(x, i - 1));
	}
}

void Board::Render(int x, int y)
{
	for (int j = 0; j < BOARD_H; j++)
	{
		for (int i = 0; i < BOARD_W; i++)
		{
			int blockx = x + i*BLOCK_SIZE;
			int blocky = y + j*BLOCK_SIZE;
			if (GetBlock(i,j))
				al_draw_filled_rectangle(blockx, blocky, blockx + BLOCK_SIZE, blocky + BLOCK_SIZE, al_map_rgb(255, 255, 0));
			al_draw_rectangle(blockx, blocky, blockx + BLOCK_SIZE, blocky + BLOCK_SIZE, al_map_rgb(255, 255, 255), 1);
		}
	}
}
