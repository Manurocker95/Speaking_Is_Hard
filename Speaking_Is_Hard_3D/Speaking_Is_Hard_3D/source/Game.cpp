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

#include "Game.h"

GameScreen::GameScreen(u32 score) : Scene ()
{
	m_maxScore = score;
	// we initialize data
	Start();
}

GameScreen::~GameScreen()
{

	delete m_bgm;
	delete font;
	delete font2;

}

void GameScreen::Start()
{
	m_screen = TITLE;
	m_pause = false;
	m_playedBefore = false;
	m_gameStarted = false;
	m_timerToGame = 0;
	m_score = 0;

	// We will use 2 channels for sounds: 1 = BGM, 2= Sound effects so they can be played at same time. You can set as channels as you want.
	// We clear the channels
	ndspChnWaveBufClear(1);
	ndspChnWaveBufClear(2);

	// We load our font called font.ttf in data folder. Set the data folder in MakeFile
	font = sftd_load_font_mem(font_ttf, font_ttf_size);
	font2 = sftd_load_font_mem(font2_ttf, font2_ttf_size);

	// We initialize our game variables
	m_offset = 0;
	// We load our sounds // PATH, CHANNEL, LOOP? -> // BGM plays with loop, but sfx just one time
	m_bgm = new sound(SND_BGM_GAME, 1, true);		

	// We play our bgm
	m_bgm->play();
}

void GameScreen::goToTitle()
{
	ResetGame();
	m_screen = TITLE;
}

void GameScreen::Draw()
{

}

void GameScreen::Update()
{
	
}

void GameScreen::CheckInputs()
{
	hidScanInput();
	held = hidKeysHeld();
	

	if (hidKeysDown() & KEY_START)
	{
		m_pause = !m_pause;
	}

	// We go to tile when pressing Select
	if ((hidKeysDown() & KEY_SELECT))
	{
		
	}
}

void GameScreen::EndGame()
{

}

void GameScreen::ResetGame()
{
	
}

void GameScreen::CheckScore()
{
	if (m_score > m_maxScore)
	{
		m_maxScore = m_score;

		// We overwrite our .sav file with new data
		std::ofstream outfile(DATA_FILE);
		outfile << m_score;
		outfile.close();
	}
}
