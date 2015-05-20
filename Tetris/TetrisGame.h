#pragma once

// TetrisGame:
// The main game logic class
// This class contains the main input/update/render loop and handles the overall game state.

#include "Board.h"
#include "Piece.h"
#include <allegro5/allegro.h>

class TetrisGame
{
public:
	TetrisGame();
	~TetrisGame();

	bool ShouldQuit() { return m_quit; }

	void Reset();
	void Init();
	void Update();
	void Render();
	void Shutdown();

private:
	void RotatePieceL();
	void RotatePieceR();
	void MovePieceL();
	void MovePieceR();
	void MovePieceDown();
	bool PieceHitGround();
	void Quit() { m_quit = true; }

	int m_linesCleared;
	int m_autofallFrameThreshold;
	int m_framesSinceAutofall;
	int m_score;
	Board m_board;
	Piece m_curPiece, m_nextPiece;
	bool m_quit;
	bool m_gameOver;
	ALLEGRO_KEYBOARD_STATE keystate_old;
};
