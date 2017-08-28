/* This file is part of T-Rekt 3D!

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

#include "font_ttf.h"			//Don't worry if it seems to have error here
#include "font2_ttf.h"
#include "sound.h"
#include "scene.h"
#include "SceneManager.h"

#include "Settings.h"

using std::vector;

class GameScreen : public Scene
{

public:

	enum SCREEN {TITLE, GAME, END};

	GameScreen(u32 score);				// Constructor
	~GameScreen();						// Destructor
	void Start() override;				// initialize
	void Draw() override;				// Draw
	void CheckInputs() override;		// CheckInput
	void Update() override;				// Update
	void CheckScore();
	void ResetGame();
	void goToTitle();
	void EndGame();

private:

	SCREEN m_screen;
	bool m_gameStarted;
	bool m_pause;						// Boolean for pausing the game
	bool m_playedBefore;
	float m_offset;						// Offset for 3D
	u32 m_score, m_maxScore;			// Current score and Max 
	u32 m_time, m_maxTime;
	u32 m_bgOpacity;
	u32 held;
	touchPosition touch;


	sf2d_texture * m_bg, * m_sprites;
	sound * m_bgm;						// Sounds
	sftd_font* font, *font2;			// Main Font, Font 2
};

#endif