/* This file is part of Speaking Is Hard 3D!

Copyright (C) 2017 Manuel Rodríguez Matesanz
>    This program is free software: you can redistribute it and/or modify
>    it under the terms of the GNU General Public License as published by
>    the Free Software Foundation, either version 3 of the License, or
>    (at your option) any later version.
>
>    This program is distributed in the hope that it will be useful,
>    but WITHOUT ANY WARRANTY; without even the implied warranty of
>    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>    GNU General Public License for more details.
>
>    You should have received a copy of the GNU General Public License
>    along with this program.  If not, see <http://www.gnu.org/licenses/>.
>    See LICENSE for information.
*/

#pragma once
#ifndef _GAME_SCREEN_H_
#define _GAME_SCREEN_H_

#include "sfil.h"
#include "sftd.h"
#include "sf2d.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <stdio.h>
#include <vector>  //for std::vector
#include <string>  //for std::string
#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include <random>
#include <chrono>       // std::chrono::system_clock
#include "font_ttf.h"			//Don't worry if it seems to have error here
#include "font2_ttf.h"
#include "sound.h"
#include "scene.h"
#include "president.h"
#include "Token.h"
#include "SceneManager.h"

#include "Settings.h"

using std::vector;

class GameScreen : public Scene
{

public:

	enum SCREEN {TITLE, GAME, END, PUZZLE };
	enum TRANSITION_STATE { OPENING, STAY, ENDING };

	GameScreen(u32 score);				// Constructor
	~GameScreen();						// Destructor
	void Start() override;				// initialize
	void Draw() override;				// Draw
	void CheckInputs() override;		// CheckInput
	void Update() override;				// Update
	void CheckScore();
	void ResetGame();
	void ResetPuzzle(bool ingame = false);
	void goToTitle();
	void EndGame();
	void load_sentences(const char* path);
	void load_bad_sentences(const char* path);
	void passedPuzzle(bool isTrue);
	void CheckPuzzle();
private:

	SCREEN m_screen;
	bool m_gameStarted;
	bool m_pause;						// Boolean for pausing the game
	bool m_playedBefore;
	bool m_toTitle;
	bool m_fromPuzzle;
	bool m_inTransition;
	bool m_goToGame;
	bool m_exit;
	bool m_speaking;
	bool m_walking;
	bool m_failed;
	bool m_showingFPS;
	float m_offset;						// Offset for 3D
	float m_timePuzzle;
	//

	//
	u32 m_score, m_maxScore;			// Current score and Max 
	u32 m_time, m_maxTime;
	u32 m_bgOpacity, m_dialogOpacity;
	u32 held;
	u32 m_scTimer;
	u16 m_sentenceID;
	u16 m_level;

	touchPosition touch;
	TRANSITION_STATE m_transitionState;
	std::vector <std::string> m_sentences;
	std::vector <std::string> m_badSentences;
	std::vector< std::vector<Token*> > puzzle;
	std::string sentence;
	President * m_president;
	sf2d_texture * m_bg, * m_sprites;
	sound * m_bgm;						// Sounds
	sftd_font* font, *font2;			// Main Font, Font 2
};

#endif