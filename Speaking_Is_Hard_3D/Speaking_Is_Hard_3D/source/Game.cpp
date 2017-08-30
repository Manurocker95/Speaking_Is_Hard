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
	delete m_bg;
	delete m_sprites;

}

void GameScreen::Start()
{
	m_screen = TITLE;
	m_pause = false;
	m_playedBefore = false;
	m_gameStarted = false;
	m_inTransition = false;
	m_exit = false;
	m_toTitle = false;
	m_walking = false;
	m_speaking = false;
	m_fromPuzzle = false;
	m_failed = false;
	m_showingFPS = false;
	m_score = 0;
	m_maxTime = TIMEPERPUZZLE;
	m_time = m_maxTime;
	m_timePuzzle = m_time;
	m_bgOpacity = 255;
	m_dialogOpacity = 255;
	m_scTimer = 0;
	m_level = 0;
	m_sentenceID = 0;
	m_transitionState = OPENING;
	sentence = "";
	load_sentences(SENTENCE_FILE);
	load_bad_sentences(BAD_SENTENCE_FILE);
	srand(time(NULL));

	// We will use 2 channels for sounds: 1 = BGM, 2= Sound effects so they can be played at same time. You can set as channels as you want.
	// We clear the channels
	ndspChnWaveBufClear(1);
	ndspChnWaveBufClear(2);

	// We load our font called font.ttf in data folder. Set the data folder in MakeFile
	font = sftd_load_font_mem(font_ttf, font_ttf_size);
	font2 = sftd_load_font_mem(font2_ttf, font2_ttf_size);

	// We initialize our game variables
	m_offset = 0;

	m_bg = sfil_load_PNG_file(IMG_BACKGROUND, SF2D_PLACE_RAM);
	m_sprites = sfil_load_PNG_file(IMG_SPRITES, SF2D_PLACE_RAM);

	m_president = new President(-67, 46, *m_sprites, true, 8, 67, 127);

	// We load our sounds // PATH, CHANNEL, LOOP? -> // BGM plays with loop, but sfx just one time
	m_bgm = new sound(SND_BGM_GAME, 1, true);		

	// We play our bgm
	m_bgm->play();
}

void GameScreen::goToTitle()
{
	m_screen = TITLE;
}

void GameScreen::Draw()
{
	m_offset = 0;

	switch (m_screen)
	{
	case TITLE:
		// Top Screen
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture_part_blend(m_bg, 0, 0, TOP_WIDTH, 0, TOP_WIDTH, HEIGHT, RGBA8(255, 255, 255, m_bgOpacity));
		sf2d_draw_texture_part_blend(m_sprites, 70, 40, 696, 254, 242, 155, RGBA8(255, 255, 255, m_bgOpacity));
		sf2d_end_frame();

		// If we have activated 3D in Settings
		if (STEREOSCOPIC_3D_ACTIVATED)
		{
			// We check the offset by the slider
			m_offset = CONFIG_3D_SLIDERSTATE * MULTIPLIER_3D;
			sf2d_start_frame(GFX_TOP, GFX_RIGHT);
			sf2d_draw_texture_part_blend(m_bg, 0, 0, TOP_WIDTH, 0, TOP_WIDTH, HEIGHT, RGBA8(255, 255, 255, m_bgOpacity));
			sf2d_draw_texture_part_blend(m_sprites, 70 + m_offset, 40, 696, 254,242, 155, RGBA8(255, 255, 255, m_bgOpacity));
			sf2d_end_frame();
		}

		// Bottom Screen
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture_part_blend(m_bg, 0, 0, TOP_WIDTH, HEIGHT, BOTTOM_WIDTH, HEIGHT, RGBA8(255, 255, 255, m_bgOpacity));
		
		//sftd_draw_text(font, 20, 20, C_WHITE, 15, m_sentences.at(0).c_str());

		if (!m_inTransition)
		{
			sftd_draw_text(font, 20, 8, C_WHITE, 15, "BEST SCORE: ");
			sftd_draw_text(font, 85, 8, C_WHITE, 15, std::to_string(m_maxScore).c_str());
			sftd_draw_text(font, 20, HEIGHT - 30, C_WHITE, 15, "Manurocker95 (C) 2017");
			sftd_draw_text(font, 230, HEIGHT - 30, C_WHITE, 15, "VERSION: ");
			sftd_draw_text(font, 280, HEIGHT - 30, C_WHITE, 15, VERSION);

			if (DEBUGMODE)
			{
				sftd_draw_text(font, 230, 8, C_WHITE, 15, "DEBUG MODE");

				if (m_showingFPS)
				{
					sftd_draw_text(font, 230, 20, C_WHITE, 15, "FPS:");
					sftd_draw_text(font, 250, 20, C_WHITE, 15, std::to_string(sf2d_get_fps()).c_str());
				}
			}
				
		}

		sf2d_end_frame();
		break;
	case GAME:
		// Top Screen
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		
		// Background
		sf2d_draw_texture_part_blend(m_bg, 0, 0, 0, 480, TOP_WIDTH, HEIGHT, RGBA8(255, 255, 255, m_bgOpacity));
		
		// President
		m_president->Draw(0, RGBA8(255, 255, 255, m_bgOpacity));

		// Table
		sf2d_draw_texture_part_blend(m_sprites, 300, 50, 838, 133, 100, 121, RGBA8(255, 255, 255, m_bgOpacity));

		// Seats
		sf2d_draw_texture_part_blend(m_sprites, 0, 171, 485, 409, 453, 69, RGBA8(255, 255, 255, m_bgOpacity));

		// Dialog
		if (m_speaking)
		{
			sf2d_draw_texture_part_blend(m_sprites, 40, 7, 582, 129, 256, 125, RGBA8(255, 255, 255, m_dialogOpacity));
			sftd_draw_text(font, 70, 49, C_BLACK, 20, sentence.c_str());
		}

		if (m_pause)
		{
			sf2d_draw_rectangle(0, 0, 420, 240, RGBA8(0x00, 0x00, 0x00, 125));
			sftd_draw_text(font2, 130, HEIGHT / 2 - 50, C_BLUE, 30, "PAUSE");
			sftd_draw_text(font, 100, HEIGHT / 2 - 10, C_BLUE, 15, "Tap or press Start to Resume the game");
		}

		sf2d_end_frame();

		// If we have activated 3D in Settings
		if (STEREOSCOPIC_3D_ACTIVATED)
		{
			// We check the offset by the slider
			m_offset = CONFIG_3D_SLIDERSTATE * MULTIPLIER_3D;
			sf2d_start_frame(GFX_TOP, GFX_RIGHT);

			// Background
			sf2d_draw_texture_part_blend(m_bg, 0+m_offset, 0, 0, 480, TOP_WIDTH, HEIGHT, RGBA8(255, 255, 255, m_bgOpacity));

			// President
			m_president->Draw(-m_offset/2, RGBA8(255, 255, 255, m_bgOpacity));

			// Table
			sf2d_draw_texture_part_blend(m_sprites, 300+m_offset/4, 50, 838, 133, 100, 121, RGBA8(255, 255, 255, m_bgOpacity));
			
			// Seats			
			sf2d_draw_texture_part_blend(m_sprites, 0, 171, 485, 409, 453, 69, RGBA8(255, 255, 255, m_bgOpacity));

			// Dialog
			if (m_speaking)
			{
				sf2d_draw_texture_part_blend(m_sprites, 40+ m_offset / 2, 7, 582, 129, 256, 125, RGBA8(255, 255, 255, m_dialogOpacity));
				sftd_draw_text(font, 70 + m_offset / 2, 49, C_BLACK, 20, sentence.c_str());
			}

			if (m_pause)
			{
				sf2d_draw_rectangle(0 - m_offset, 0, 420, 240, RGBA8(0x00, 0x00, 0x00, 125));
				sftd_draw_text(font2, 130 - m_offset, HEIGHT / 2 - 50, C_BLUE, 30, "PAUSE");
				sftd_draw_text(font, 100 - m_offset, HEIGHT / 2 - 10, C_BLUE, 15, "Tap or press Start to Resume the game");
			}

			sf2d_end_frame();
		}

		// Bottom Screen
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		
		if (!m_inTransition)
		{
			sftd_draw_text(font, 20, 8, C_WHITE, 15, "BEST SCORE: ");
			sftd_draw_text(font, 85, 8, C_WHITE, 15, std::to_string(m_maxScore).c_str());
			//sftd_draw_text(font, 20, HEIGHT - 30, C_WHITE, 15, "Manurocker95 (C) 2017");
			//sftd_draw_text(font, 230, HEIGHT - 30, C_WHITE, 15, "VERSION: ");
			//sftd_draw_text(font, 280, HEIGHT - 30, C_WHITE, 15, VERSION);

			if (DEBUGMODE)
			{
				sftd_draw_text(font, 230, 8, C_WHITE, 15, "DEBUG MODE");

				if (m_showingFPS)
				{
					sftd_draw_text(font, 230, 20, C_WHITE, 15, "FPS:");
					sftd_draw_text(font, 250, 20, C_WHITE, 15, std::to_string(sf2d_get_fps()).c_str());
				}
			}
		}

		sf2d_end_frame();
		break;
	case PUZZLE:
		// Top Screen
		sf2d_start_frame(GFX_TOP, GFX_LEFT);

		// Background
		sf2d_draw_texture_part_blend(m_bg, 0, 0, TOP_WIDTH, 480, TOP_WIDTH, HEIGHT, RGBA8(255, 255, 255, m_bgOpacity));

		// Reference image of the puzzle
		sf2d_draw_texture_part_blend(m_sprites, 136, 55, m_level*135, 508, 135, 135, RGBA8(255, 255, 255, m_bgOpacity));

		//y=

		if (!m_inTransition)
		{
			sftd_draw_text(font, 20, 5, C_BROWN, 30, "TIME:");
			sftd_draw_text(font, 80, 5, C_BROWN, 30, std::to_string(m_time).c_str());
			sftd_draw_text(font, 280, 5, C_BROWN, 30, "SCORE:");
			sftd_draw_text(font, 350, 5, C_BROWN, 30, std::to_string(m_score).c_str());
		}

		if (m_pause)
		{
			sf2d_draw_rectangle(0, 0, 420, 240, RGBA8(0x00, 0x00, 0x00, 125));
			sftd_draw_text(font2, 130, HEIGHT / 2 - 50, C_BLUE, 30, "PAUSE");
			sftd_draw_text(font, 100, HEIGHT / 2 - 10, C_BLUE, 15, "Tap or press Start to Resume the game");
		}

		sf2d_end_frame();

		// If we have activated 3D in Settings
		if (STEREOSCOPIC_3D_ACTIVATED)
		{
			// We check the offset by the slider
			m_offset = CONFIG_3D_SLIDERSTATE * MULTIPLIER_3D;
			sf2d_start_frame(GFX_TOP, GFX_RIGHT);

			// Background
			sf2d_draw_texture_part_blend(m_bg, 0 + m_offset, 0, TOP_WIDTH, 480, TOP_WIDTH, HEIGHT, RGBA8(255, 255, 255, m_bgOpacity));

			// Reference image of the puzzle
			sf2d_draw_texture_part_blend(m_sprites, 136+m_offset, 55, m_level * 135, 508, 135, 135, RGBA8(255, 255, 255, m_bgOpacity));

			if (!m_inTransition)
			{
				sftd_draw_text(font, 20 + m_offset / 2, 5, C_BROWN, 30, "TIME:");
				sftd_draw_text(font, 80 + m_offset / 2, 5, C_BROWN, 30, std::to_string(m_time).c_str());
				sftd_draw_text(font, 280 + m_offset / 2, 5, C_BROWN, 30, "SCORE:");
				sftd_draw_text(font, 350 + m_offset / 2, 5, C_BROWN, 30, std::to_string(m_score).c_str());
			}

			if (m_pause)
			{
				sf2d_draw_rectangle(0 - m_offset, 0, 420, 240, RGBA8(0x00, 0x00, 0x00, 125));
				sftd_draw_text(font2, 130 - m_offset, HEIGHT / 2 - 50, C_BLUE, 30, "PAUSE");
				sftd_draw_text(font, 100 - m_offset, HEIGHT / 2 - 10, C_BLUE, 15, "Tap or press Start to Resume the game");
			}

			sf2d_end_frame();
		}

		// Bottom Screen
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture_part_blend(m_bg, 0, 0, TOP_WIDTH, 480 + HEIGHT, BOTTOM_WIDTH, HEIGHT, RGBA8(255, 255, 255, m_bgOpacity));

		if (!m_inTransition)
		{
			sftd_draw_text(font, 20, 8, C_BROWN, 15, "BEST SCORE: ");
			sftd_draw_text(font, 85, 8, C_BROWN, 15, std::to_string(m_maxScore).c_str());
			//sftd_draw_text(font, 20, HEIGHT - 30, C_BROWN, 15, "Manurocker95 (C) 2017");
			//sftd_draw_text(font, 230, HEIGHT - 30, C_BROWN, 15, "VERSION: ");
			//sftd_draw_text(font, 280, HEIGHT - 30, C_BROWN, 15, VERSION);

			if (DEBUGMODE)
			{
				sftd_draw_text(font, 230, 8, C_BROWN, 15, "DEBUG MODE");

				if (m_showingFPS)
				{
					sftd_draw_text(font, 230, 20, C_BROWN, 15, "FPS:");
					sftd_draw_text(font, 250, 20, C_BROWN, 15, std::to_string(sf2d_get_fps()).c_str());
				}
			}
		}

		sf2d_end_frame();
		break;
	case END:
		break;
	}
}

void GameScreen::Update()
{
	switch (m_screen)
	{
	case TITLE:
		if (m_inTransition)
		{
			switch (m_transitionState)
			{
			case OPENING:

				m_bgOpacity -= 3;

				if (m_bgOpacity <= 0)
				{
					m_transitionState = STAY;
					m_bgOpacity = 0;
					ResetGame();
				}

				break;
			case STAY:

				m_scTimer += 5;

				if (m_scTimer >= 300)
				{
					m_transitionState = ENDING;
				}

				break;
			case ENDING:

				m_bgOpacity += 3;

				if (m_bgOpacity >= 255)
				{
					m_bgOpacity = 255;
					m_transitionState = OPENING;
					m_inTransition = false;
				}
				break;
			}
		}
		break;
	case GAME:
		if (!m_pause)
		{
			m_president->Update();

			if (m_inTransition)
			{
				switch (m_transitionState)
				{
				case OPENING:

					m_bgOpacity -= 3;

					if (m_bgOpacity <= 0)
					{
						m_transitionState = STAY;
						m_bgOpacity = 0;

						if (m_toTitle)
							m_screen = TITLE;
						else
						{
							m_screen = PUZZLE;
							m_level = rand() % NUMOFLEVELS;
							ResetPuzzle();
						}
							
					}

					break;
				case STAY:

					m_scTimer += 5;

					if (m_scTimer >= 300)
					{
						m_transitionState = ENDING;

						if (!m_fromPuzzle)
							m_president->moving(true, 0);
					}

					break;
				case ENDING:

					m_bgOpacity += 3;

					if (m_bgOpacity >= 255)
					{
						m_bgOpacity = 255;
						m_transitionState = OPENING;
						m_inTransition = false;
						m_scTimer = 0;

						if (m_fromPuzzle)
						{
							if (m_failed)
							{
								CheckScore();
								m_president->mistake();
							}
							else
							{
								m_president->speak();
								m_level = rand() % NUMOFLEVELS;
							}
						}
							
					}
					break;
				}
			}
		}

		if (m_speaking)
		{
			// Stopped speaking
			if (m_president->isWaiting())
			{
				m_speaking = false;
				m_toTitle = false;
				m_inTransition = true;

				if (m_fromPuzzle && m_failed)
				{
					m_president->moving(true, 1);
					m_fromPuzzle = false;
					m_toTitle = true;
					m_failed = false;
				}
					
			}
		}
		else
		{
			if (m_president->isSpeaking())
			{
				m_sentenceID = rand() % NUMOFSENTENCES;
				m_speaking = true;
				sentence = m_sentences.at(m_sentenceID);
			}
			else if (m_president->isMistaking())
			{
				m_speaking = true;
				sentence = m_badSentences.at(m_sentenceID);
			}
		}

		break;
	case PUZZLE:
		if (!m_pause)
		{
			if (m_inTransition)
			{
				switch (m_transitionState)
				{
				case OPENING:

					m_bgOpacity -= 3;

					if (m_bgOpacity <= 0)
					{
						m_transitionState = STAY;
						m_bgOpacity = 0;
						m_screen = GAME;	
					}

					break;
				case STAY:

					m_scTimer += 5;

					if (m_scTimer >= 300)
					{
						m_transitionState = ENDING;
					}

					break;
				case ENDING:

					m_bgOpacity += 3;

					if (m_bgOpacity >= 255)
					{
						m_bgOpacity = 255;
						m_transitionState = OPENING;
						m_inTransition = false;
						m_scTimer = 0;
					}
					break;
				}
			}
			else
			{
				m_timePuzzle -= 1 / sf2d_get_fps();
				m_time = (u32)m_timePuzzle;
			}
		}
		break;
	}
}

void GameScreen::CheckInputs()
{
	hidScanInput();
	held = hidKeysHeld();
	

	switch (m_screen)
	{
	case TITLE:
		if (hidKeysDown() & KEY_A)
		{
			m_inTransition = true;
			return;
		}
		if (hidKeysDown() & KEY_TOUCH)
		{
			hidTouchRead(&touch);
			if ((touch.px > 115 && touch.px < 225) && (touch.py > 50 && touch.py < 86))
			{
				m_inTransition = true;
				return;
			}
			if ((touch.px > 120 && touch.px < 205) && (touch.py > 120 && touch.py < 156))
			{
				m_exit = true;
			}
		}
		break;
	case GAME:
		
		if (!m_inTransition && (hidKeysDown() & KEY_B))
		{
			m_speaking = false;
			m_toTitle = true;
			m_inTransition = true;
			m_president->moving(true, 1);
		}

		if (hidKeysDown() & KEY_START)
		{
			m_pause = !m_pause;
		}

		break;
	case PUZZLE:

		if (hidKeysDown() & KEY_B)
		{
			passedPuzzle(false);
		}

		if (DEBUGMODE && (hidKeysDown() & KEY_X))
		{
			passedPuzzle(true);
		}

		if (hidKeysDown() & KEY_TOUCH)
		{
			hidTouchRead(&touch);
			if ((touch.px > 248 && touch.px < 312) && (touch.py > 76 && touch.py < 140)) 
			{
				passedPuzzle(false);
			}

			if ((touch.px > 16 && touch.px < 80) && (touch.py > 76 && touch.py < 140))
			{
				ResetPuzzle(true);
			}
		}

		break;
	}

	if (DEBUGMODE)
	{
		if (hidKeysDown() & KEY_L)
		{
			m_showingFPS = !m_showingFPS;
		}

		if (hidKeysDown() & KEY_R)
		{
			m_score += 10;
		}
	}

	// We go to tile when pressing Select
	if ((hidKeysDown() & KEY_SELECT))
	{
		m_exit = true;
	}

	if (m_exit)
	{
		SceneManager::instance()->exitGame();
	}
}

void GameScreen::passedPuzzle(bool isTrue)
{
	if (isTrue)
	{
		m_fromPuzzle = true;
		m_speaking = false;
		m_toTitle = false;
		m_inTransition = true;
		m_failed = false;
		m_score += 1;
	}
	else
	{
		m_fromPuzzle = true;
		m_speaking = false;
		m_toTitle = true;
		m_inTransition = true;
		m_failed = true;
	}
}

void GameScreen::ResetPuzzle(bool ingame)
{
	m_time = m_maxTime;
	m_timePuzzle = m_time;

	if (ingame)
	{
		m_score -= PUZZLEPENALTY;
		if (m_score < 0)
			m_score = 0;
	}
}

void GameScreen::load_sentences(const char* path)
{
	FILE* f = fopen(path, "r");
	if (f)
	{
		char mystring[100];
		int sentence = 0;
		while (fgets(mystring, sizeof(mystring), f))
		{
			int a = strlen(mystring);
			if (mystring[a - 1] == '\n')
			{
				mystring[a - 1] = 0;
				if (mystring[a - 2] == '\r')
					mystring[a - 2] = 0;
			}

			m_sentences.push_back(mystring);
			sentence++;
		}
		
		m_sentences.push_back("Aaaaaand... that's it.");
		fclose(f);
	}
	else
	{
		m_sentences.push_back("Can't load the sentence file.");
	}
}

void GameScreen::load_bad_sentences(const char* path)
{
	FILE* f = fopen(path, "r");
	if (f)
	{
		char mystring[100];
		int sentence = 0;
		while (fgets(mystring, sizeof(mystring), f))
		{
			int a = strlen(mystring);
			if (mystring[a - 1] == '\n')
			{
				mystring[a - 1] = 0;
				if (mystring[a - 2] == '\r')
					mystring[a - 2] = 0;
			}

			//m_sentences.push_back(std::to_string(sentence));
			m_badSentences.push_back(mystring);
			//puts(mystring);
			sentence++;
		}

		m_badSentences.push_back("Aaaaaand... that's it.");
		//printf(">>EOF<<\n");
		fclose(f);
	}
	else
	{
		m_badSentences.push_back("Can't load the sentence file.");
	}
}

void GameScreen::EndGame()
{

}

void GameScreen::ResetGame()
{
	m_score = 0;
	m_screen = GAME;
	m_pause = false;
	m_fromPuzzle = false;
	m_failed = false;
	m_toTitle = false;
	m_president->setX(-67);
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
