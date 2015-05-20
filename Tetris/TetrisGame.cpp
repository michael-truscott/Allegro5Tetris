#include "TetrisGame.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "global.h"


ALLEGRO_DISPLAY *display;
ALLEGRO_FONT *font;

TetrisGame::TetrisGame() :
	m_curPiece((PIECETYPE)(rand() % PIECE_MAX)),
	m_nextPiece((PIECETYPE)(rand() % PIECE_MAX))
{
	m_quit = false;
	m_gameOver = false;
	m_autofallFrameThreshold = AUTOFALL_FRAMES_DEFAULT;
	m_framesSinceAutofall = 0;
	m_linesCleared = 0;
	m_score = 0;
	Init();
}


TetrisGame::~TetrisGame()
{
	Shutdown();
}

void TetrisGame::Init()
{
	// Initialise allegro and its addons
	al_init();
	display = al_create_display(SCREEN_W, SCREEN_H);
	al_install_keyboard();
	al_init_primitives_addon();
	al_init_font_addon();
	font = al_create_builtin_font();

	al_get_keyboard_state(&keystate_old);
}

void TetrisGame::Update()
{
	ALLEGRO_KEYBOARD_STATE keystate;
	al_get_keyboard_state(&keystate);
	if (al_key_down(&keystate, ALLEGRO_KEY_ESCAPE))
		Quit();

	if (!m_gameOver)
	{
		++m_framesSinceAutofall;
		// Process player input
		// For moving/rotation, check that the key was only pushed down on this exact frame to prevent key bouncing
		if (al_key_down(&keystate, ALLEGRO_KEY_Z) && !al_key_down(&keystate_old, ALLEGRO_KEY_Z))
			RotatePieceL();
		else if (al_key_down(&keystate, ALLEGRO_KEY_X) && !al_key_down(&keystate_old, ALLEGRO_KEY_X))
			RotatePieceR();

		if (al_key_down(&keystate, ALLEGRO_KEY_LEFT) && !al_key_down(&keystate_old, ALLEGRO_KEY_LEFT))
			MovePieceL();
		else if (al_key_down(&keystate, ALLEGRO_KEY_RIGHT) && !al_key_down(&keystate_old, ALLEGRO_KEY_RIGHT))
			MovePieceR();

		if (al_key_down(&keystate, ALLEGRO_KEY_DOWN) || m_framesSinceAutofall >= m_autofallFrameThreshold)
		{
			MovePieceDown();
			m_framesSinceAutofall = 0;
		}

		// Clear lines/add to score
		int linesCleared = m_board.ClearFullLines();
		if (linesCleared > 0)
		{
			int levelBefore = m_linesCleared / 10;
			m_linesCleared += linesCleared;
			int levelAfter = m_linesCleared / 10;
			// Increase autofall speed
			if (levelAfter > levelBefore && m_autofallFrameThreshold > 2)
				m_autofallFrameThreshold -= 2;
			// Add to score
			int basescore;
			switch (linesCleared)
			{
			case 1: basescore = 40; break;
			case 2: basescore = 100; break;
			case 3: basescore = 300; break;
			case 4: basescore = 1200; break;
			default: basescore = 0; break;
			}
			m_score += basescore * (levelBefore + 1);
		}
	}
	else
	{
		if (al_key_down(&keystate, ALLEGRO_KEY_ENTER))
			Reset();
	}

	al_get_keyboard_state(&keystate_old);
}

void TetrisGame::Reset()
{
	m_gameOver = false;
	m_curPiece = m_nextPiece;
	m_nextPiece = Piece((PIECETYPE)(rand() % PIECE_MAX));
	m_board = Board();
	m_autofallFrameThreshold = AUTOFALL_FRAMES_DEFAULT;
	m_framesSinceAutofall = 0;
	m_linesCleared = 0;
	m_score = 0;
}

void TetrisGame::RotatePieceL()
{
	m_curPiece.SaveState();
	m_curPiece.RotateL();
	if (m_board.PieceIntersects(m_curPiece))
		m_curPiece.RevertState();
	else if (m_curPiece.OutOfBounds())
	{
		m_curPiece.Kick();
		// A kicked piece can push its way inside existing blocks.
		// Check if it's going to cause an collision and revert if so.
		if (m_board.PieceIntersects(m_curPiece))
			m_curPiece.RevertState();
	}
}

void TetrisGame::RotatePieceR()
{
	m_curPiece.SaveState();
	m_curPiece.RotateR();
	if (m_board.PieceIntersects(m_curPiece))
		m_curPiece.RevertState();
	else if (m_curPiece.OutOfBounds())
	{
		m_curPiece.Kick();
		if (m_board.PieceIntersects(m_curPiece))
			m_curPiece.RevertState();
	}
}

void TetrisGame::MovePieceL()
{
	m_curPiece.SaveState();
	// Move the piece left
	m_curPiece.X--;
	// If it intersects the board, or goes out of bounds, move it back
	if (m_curPiece.OutOfBounds() || m_board.PieceIntersects(m_curPiece))
		m_curPiece.RevertState();
}

void TetrisGame::MovePieceR()
{
	m_curPiece.SaveState();
	// Move the piece right
	m_curPiece.X++;
	// If it intersects the board, or goes out of bounds, move it back
	if (m_curPiece.OutOfBounds() || m_board.PieceIntersects(m_curPiece))
		m_curPiece.RevertState();
}

void TetrisGame::MovePieceDown()
{
	m_curPiece.SaveState();
	m_curPiece.Y++;
	if (PieceHitGround() || m_board.PieceIntersects(m_curPiece))
	{
		m_curPiece.RevertState();
		m_board.AddPieceBlocks(m_curPiece);
		m_curPiece = m_nextPiece;
		m_nextPiece = Piece((PIECETYPE)(rand() % PIECE_MAX));
		// If the new piece collides with the board right away, the player has lost the game
		if (m_board.PieceIntersects(m_curPiece))
			m_gameOver = true;
	}
}

// Function to check whether the current piece's projected movement will cause it to hit the ground
bool TetrisGame::PieceHitGround()
{
	if (m_curPiece.Y < BOARD_H - 4)
		return false;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_curPiece.GetBlock(i, j))
			{
				int y = m_curPiece.Y + j;
				if (y >= BOARD_H)
					return true;
			}
		}
	}
	return false;
}

void TetrisGame::Render()
{
	// Clear to black
	al_clear_to_color(al_map_rgb(0, 0, 0));

	// Draw the board and the current piece
	m_board.Render((SCREEN_W / 2) - (BOARD_W / 2) * BLOCK_SIZE, 0);
	m_curPiece.Render((SCREEN_W / 2) - (BOARD_W / 2) * BLOCK_SIZE, 0);

	// Draw the next piece preview
	m_nextPiece.Render(450, 50);
	al_draw_text(font, al_map_rgb(0, 255, 0), 530, 150, ALLEGRO_ALIGN_CENTRE, "NEXT");

	// Draw the score display
	int level = m_linesCleared / 10;
	al_draw_textf(font, al_map_rgb(0, 255, 0), 50, 100, 0, "SCORE: %d", m_score);
	al_draw_textf(font, al_map_rgb(0, 255, 0), 50, 110, 0, "LINES: %d", m_linesCleared);
	al_draw_textf(font, al_map_rgb(0, 255, 0), 50, 120, 0, "LEVEL: %d", level);

	// Draw the game over message
	if (m_gameOver)
		al_draw_text(font, al_map_rgb(0, 255, 0), SCREEN_W / 2, SCREEN_H - 50, ALLEGRO_ALIGN_CENTRE, "GAME OVER!");

	al_flip_display();
}

void TetrisGame::Shutdown()
{
	al_destroy_display(display);
	al_destroy_font(font);
}
