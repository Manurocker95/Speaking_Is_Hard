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

#include "Game.h"

// * Constructor - We create the scene - It calls the Start method
GameScreen::GameScreen(u32 score) : Scene ()
{
	m_maxScore = score;
	// we initialize data
	Start();
}

// * Destructor - We delete pointers and stuff
GameScreen::~GameScreen()
{

	delete m_bgm;
	delete m_bgm2;
	delete font;
	delete font2;
	delete m_bg;
	delete m_sprites;

}

// * Start - We Initialize the variables
void GameScreen::Start()
{
	m_screen = TITLE;
	m_transitionState = OPENING;

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
	sentence = "";
	debug = "";
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

	u16 x = 96;
	u16 y = 45;
	u16 counter = 0;

	for (int i = 0; i < PUZZLEDIFFICULTY; i++) {
		std::vector<Token*> row; 
		for (int j = 0; j < PUZZLEDIFFICULTY; j++) {
			row.push_back(new Token(x + j*45,y+i*45, *m_sprites, (m_level * 135)+ (j * 45), 508+(i*45), 45, 45, counter));
			counter++;
		}
		puzzle.push_back(row); 
		original_puzzle.push_back(row);
	}

	// We load our sounds // PATH, CHANNEL, LOOP? -> // BGM plays with loop, but sfx just one time
	m_bgm = new sound(SND_BGM_GAME, 1, true);		
	m_bgm2 = new sound(SND_BGM_GAME2, 1, true);

	// We play our bgm
	m_bgm->play();
}

// * Go To Title - We Go To 
void GameScreen::goToTitle()
{
	m_screen = TITLE;
}

// * Draw - We draw every  
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
		
		// the puzzle
		for (int i = 0; i < PUZZLEDIFFICULTY; i++)
		{
			for (int j = 0; j < PUZZLEDIFFICULTY; j++)
			{
				puzzle[i][j]->Draw(0, RGBA8(255, 255, 255, m_bgOpacity));
			}
		}

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

// * Update - We update the game
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
							m_bgm->stop();
							m_bgm2->play();
							m_screen = PUZZLE;
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
						m_bgm2->stop();
						m_bgm->play();
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

				if (m_time <= 0)
				{
					passedPuzzle(false);
				}
			}
		}
		break;
	}
}

// * Check Inputs - We check what inputs are pressed
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

		if (!m_pause)
		{
			if (hidKeysDown() & KEY_B)
			{
				passedPuzzle(false);
			}

			if (hidKeysDown() & KEY_A)
			{
				CheckPuzzle();
			}

			if (hidKeysDown() & KEY_Y)
			{
				ResetPuzzle(true);
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
					CheckPuzzle();
				}

				if ((touch.px > 16 && touch.px < 80) && (touch.py > 76 && touch.py < 140))
				{
					passedPuzzle(false);
				}

				//
				// * The puzzle
				//
				MovePuzzle(touch);
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

// * Method for moving each token 
void GameScreen::MovePuzzle(touchPosition touch)
{
	for (u16 i = 0; i < PUZZLEDIFFICULTY; i++)
	{
		for (u16 j = 0; j < PUZZLEDIFFICULTY; j++)
		{
			if (puzzle[i][j]->isPressed(touch) && !puzzle[i][j]->isBlank())
			{
				if (i == 0)
				{
					// First position in the matrix [0][0]
					if (j == 0)
					{
						// We check [1] [0]
						if (puzzle[i+1][j]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i + 1][j]->getX());
							puzzle[i][j]->setY(puzzle[i + 1][j]->getY());
							puzzle[i][j]->setID(puzzle[i + 1][j]->getID());
							puzzle[i][j] = puzzle[i + 1][j];

							puzzle[i + 1][j]->setX(tempX);
							puzzle[i + 1][j]->setY(tempY);
							puzzle[i + 1][j]->setID(tempID);
							puzzle[i + 1][j] = tempToken;
							
							return;
						}
						// We check [0] [1]
						else if (puzzle[i][j+1]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i][j + 1]->getX());
							puzzle[i][j]->setY(puzzle[i][j + 1]->getY());
							puzzle[i][j]->setID(puzzle[i][j + 1]->getID());
							puzzle[i][j] = puzzle[i][j + 1];

							puzzle[i][j + 1]->setX(tempX);
							puzzle[i][j + 1]->setY(tempY);
							puzzle[i][j + 1]->setID(tempID);
							puzzle[i][j + 1] = tempToken;
							debug = std::to_string(i) + " es i y j es: "+std::to_string(j);
							return;
						}
					}
					// Last position in the matrix [0][LAST]
					else if (j == PUZZLEDIFFICULTY - 1)
					{
						// We check [0] [Last]
						if (puzzle[i + 1][j]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i + 1][j]->getX());
							puzzle[i][j]->setY(puzzle[i + 1][j]->getY());
							puzzle[i][j]->setID(puzzle[i + 1][j]->getID());
							puzzle[i][j] = puzzle[i + 1][j];

							puzzle[i + 1][j]->setX(tempX);
							puzzle[i + 1][j]->setY(tempY);
							puzzle[i + 1][j]->setID(tempID);
							puzzle[i + 1][j] = tempToken;
							return;
						}
						// We check [0] [Last-1]
						else if (puzzle[i][j - 1]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i][j - 1]->getX());
							puzzle[i][j]->setY(puzzle[i][j - 1]->getY());
							puzzle[i][j]->setID(puzzle[i][j - 1]->getID());
							puzzle[i][j] = puzzle[i][j - 1];

							puzzle[i][j - 1]->setX(tempX);
							puzzle[i][j - 1]->setY(tempY);
							puzzle[i][j - 1]->setID(tempID);
							puzzle[i][j - 1] = tempToken;
							return;
						}
					}
					else
					{
						// We check [1] [n]
						if (puzzle[i + 1][j]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i + 1][j]->getX());
							puzzle[i][j]->setY(puzzle[i + 1][j]->getY());
							puzzle[i][j]->setID(puzzle[i + 1][j]->getID());
							puzzle[i][j] = puzzle[i + 1][j];

							puzzle[i + 1][j]->setX(tempX);
							puzzle[i + 1][j]->setY(tempY);
							puzzle[i + 1][j]->setID(tempID);
							puzzle[i + 1][j] = tempToken;
							return;
						}
						// We check [0] [n-1]
						else if (puzzle[i][j - 1]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i][j - 1]->getX());
							puzzle[i][j]->setY(puzzle[i][j - 1]->getY());
							puzzle[i][j]->setID(puzzle[i][j - 1]->getID());
							puzzle[i][j] = puzzle[i][j - 1];

							puzzle[i][j - 1]->setX(tempX);
							puzzle[i][j - 1]->setY(tempY);
							puzzle[i][j - 1]->setID(tempID);
							puzzle[i][j - 1] = tempToken;
							return;
						}
						// We check [0] [n+1]
						else if (puzzle[i][j + 1]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i][j + 1]->getX());
							puzzle[i][j]->setY(puzzle[i][j + 1]->getY());
							puzzle[i][j]->setID(puzzle[i][j + 1]->getID());
							puzzle[i][j] = puzzle[i][j + 1];

							puzzle[i][j + 1]->setX(tempX);
							puzzle[i][j + 1]->setY(tempY);
							puzzle[i][j + 1]->setID(tempID);
							puzzle[i][j + 1] = tempToken;
							return;
						}
					}
				}
				else if (i == PUZZLEDIFFICULTY-1)
				{
					// First position in the matrix [LAST][0]
					if (j == 0)
					{
						// We check [1] [0]
						if (puzzle[i - 1][j]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i - 1][j]->getX());
							puzzle[i][j]->setY(puzzle[i - 1][j]->getY());
							puzzle[i][j]->setID(puzzle[i - 1][j]->getID());
							puzzle[i][j] = puzzle[i - 1][j];

							puzzle[i - 1][j]->setX(tempX);
							puzzle[i - 1][j]->setY(tempY);
							puzzle[i - 1][j]->setID(tempID);
							puzzle[i - 1][j] = tempToken;
							return;
						}
						// We check [0] [1]
						else if (puzzle[i][j + 1]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i][j + 1]->getX());
							puzzle[i][j]->setY(puzzle[i][j + 1]->getY());
							puzzle[i][j]->setID(puzzle[i][j + 1]->getID());
							puzzle[i][j] = puzzle[i][j + 1];

							puzzle[i][j + 1]->setX(tempX);
							puzzle[i][j + 1]->setY(tempY);
							puzzle[i][j + 1]->setID(tempID);
							puzzle[i][j + 1] = tempToken;
							return;
						}
					}
					// Last position in the matrix [LAST][LAST]
					else if (j == PUZZLEDIFFICULTY - 1)
					{
						// We check [0] [Last]
						if (puzzle[i - 1][j]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i - 1][j]->getX());
							puzzle[i][j]->setY(puzzle[i - 1][j]->getY());
							puzzle[i][j]->setID(puzzle[i - 1][j]->getID());
							puzzle[i][j] = puzzle[i - 1][j];

							puzzle[i - 1][j]->setX(tempX);
							puzzle[i - 1][j]->setY(tempY);
							puzzle[i - 1][j]->setID(tempID);
							puzzle[i - 1][j] = tempToken;
							return;
						}
						// We check [0] [Last-1]
						else if (puzzle[i][j - 1]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i][j - 1]->getX());
							puzzle[i][j]->setY(puzzle[i][j - 1]->getY());
							puzzle[i][j]->setID(puzzle[i][j - 1]->getID());
							puzzle[i][j] = puzzle[i][j - 1];

							puzzle[i][j - 1]->setX(tempX);
							puzzle[i][j - 1]->setY(tempY);
							puzzle[i][j - 1]->setID(tempID);
							puzzle[i][j - 1] = tempToken;
							return;
						}
					}
					else
					{
						// We check [1] [n]
						if (puzzle[i - 1][j]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i - 1][j]->getX());
							puzzle[i][j]->setY(puzzle[i - 1][j]->getY());
							puzzle[i][j]->setID(puzzle[i - 1][j]->getID());
							puzzle[i][j] = puzzle[i - 1][j];

							puzzle[i - 1][j]->setX(tempX);
							puzzle[i - 1][j]->setY(tempY);
							puzzle[i - 1][j]->setID(tempID);
							puzzle[i - 1][j] = tempToken;
							return;
						}
						// We check [0] [n-1]
						else if (puzzle[i][j - 1]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i][j - 1]->getX());
							puzzle[i][j]->setY(puzzle[i][j - 1]->getY());
							puzzle[i][j]->setID(puzzle[i][j - 1]->getID());
							puzzle[i][j] = puzzle[i][j - 1];

							puzzle[i][j - 1]->setX(tempX);
							puzzle[i][j - 1]->setY(tempY);
							puzzle[i][j - 1]->setID(tempID);
							puzzle[i][j - 1] = tempToken;
							return;
						}
						// We check [0] [n+1]
						else if (puzzle[i][j + 1]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i][j + 1]->getX());
							puzzle[i][j]->setY(puzzle[i][j + 1]->getY());
							puzzle[i][j]->setID(puzzle[i][j + 1]->getID());
							puzzle[i][j] = puzzle[i][j + 1];

							puzzle[i][j + 1]->setX(tempX);
							puzzle[i][j + 1]->setY(tempY);
							puzzle[i][j + 1]->setID(tempID);
							puzzle[i][j + 1] = tempToken;
							return;
						}
					}
				}
				else
				{
					// First position in the matrix [n][0]
					if (j == 0)
					{
						// We check [n+1] [0]
						if (puzzle[i + 1][j]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i + 1][j]->getX());
							puzzle[i][j]->setY(puzzle[i + 1][j]->getY());
							puzzle[i][j]->setID(puzzle[i + 1][j]->getID());
							puzzle[i][j] = puzzle[i + 1][j];

							puzzle[i + 1][j]->setX(tempX);
							puzzle[i + 1][j]->setY(tempY);
							puzzle[i + 1][j]->setID(tempID);
							puzzle[i + 1][j] = tempToken;
							return;
						}
						// We check [n-1] [0]
						else if (puzzle[i - 1][j]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i - 1][j]->getX());
							puzzle[i][j]->setY(puzzle[i - 1][j]->getY());
							puzzle[i][j]->setID(puzzle[i - 1][j]->getID());
							puzzle[i][j] = puzzle[i - 1][j];

							puzzle[i - 1][j]->setX(tempX);
							puzzle[i - 1][j]->setY(tempY);
							puzzle[i - 1][j]->setID(tempID);
							puzzle[i - 1][j] = tempToken;
							return;
						}
						// We check [n] [1]
						else if (puzzle[i][j + 1]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i][j + 1]->getX());
							puzzle[i][j]->setY(puzzle[i][j + 1]->getY());
							puzzle[i][j]->setID(puzzle[i][j + 1]->getID());
							puzzle[i][j] = puzzle[i][j + 1];

							puzzle[i][j + 1]->setX(tempX);
							puzzle[i][j + 1]->setY(tempY);
							puzzle[i][j + 1]->setID(tempID);
							puzzle[i][j + 1] = tempToken;
							return;
						}
					}
					// Last position in the matrix [n][LAST]
					else if (j == PUZZLEDIFFICULTY - 1)
					{
						// We check [0] [Last]
						if (puzzle[i + 1][j]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i + 1][j]->getX());
							puzzle[i][j]->setY(puzzle[i + 1][j]->getY());
							puzzle[i][j]->setID(puzzle[i + 1][j]->getID());
							puzzle[i][j] = puzzle[i + 1][j];

							puzzle[i + 1][j]->setX(tempX);
							puzzle[i + 1][j]->setY(tempY);
							puzzle[i + 1][j]->setID(tempID);
							puzzle[i + 1][j] = tempToken;
							return;
						}
						else if (puzzle[i - 1][j]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i - 1][j]->getX());
							puzzle[i][j]->setY(puzzle[i - 1][j]->getY());
							puzzle[i][j]->setID(puzzle[i - 1][j]->getID());
							puzzle[i][j] = puzzle[i - 1][j];

							puzzle[i - 1][j]->setX(tempX);
							puzzle[i - 1][j]->setY(tempY);
							puzzle[i - 1][j]->setID(tempID);
							puzzle[i - 1][j] = tempToken;
							return;
						}
						// We check [0] [Last-1]
						else if (puzzle[i][j - 1]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i][j - 1]->getX());
							puzzle[i][j]->setY(puzzle[i][j - 1]->getY());
							puzzle[i][j]->setID(puzzle[i][j - 1]->getID());
							puzzle[i][j] = puzzle[i][j - 1];

							puzzle[i][j - 1]->setX(tempX);
							puzzle[i][j - 1]->setY(tempY);
							puzzle[i][j - 1]->setID(tempID);
							puzzle[i][j - 1] = tempToken;
							return;
						}
					}
					// Position[n][m]
					else
					{
						// We check [n+1] [m]
						if (puzzle[i + 1][j]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i + 1][j]->getX());
							puzzle[i][j]->setY(puzzle[i + 1][j]->getY());
							puzzle[i][j]->setID(puzzle[i + 1][j]->getID());
							puzzle[i][j] = puzzle[i + 1][j];

							puzzle[i + 1][j]->setX(tempX);
							puzzle[i + 1][j]->setY(tempY);
							puzzle[i + 1][j]->setID(tempID);
							puzzle[i + 1][j] = tempToken;
							return;
						}
						// We check [n-1] [m]
						if (puzzle[i - 1][j]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i - 1][j]->getX());
							puzzle[i][j]->setY(puzzle[i - 1][j]->getY());
							puzzle[i][j]->setID(puzzle[i - 1][j]->getID());
							puzzle[i][j] = puzzle[i - 1][j];

							puzzle[i - 1][j]->setX(tempX);
							puzzle[i - 1][j]->setY(tempY);
							puzzle[i - 1][j]->setID(tempID);
							puzzle[i - 1][j] = tempToken;
							return;
						}
						// We check [n] [m-1]
						else if (puzzle[i][j - 1]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i][j - 1]->getX());
							puzzle[i][j]->setY(puzzle[i][j - 1]->getY());
							puzzle[i][j]->setID(puzzle[i][j - 1]->getID());
							puzzle[i][j] = puzzle[i][j - 1];

							puzzle[i][j - 1]->setX(tempX);
							puzzle[i][j - 1]->setY(tempY);
							puzzle[i][j - 1]->setID(tempID);
							puzzle[i][j - 1] = tempToken;
							return;
						}
						// We check [n] [m+1]
						else if (puzzle[i][j + 1]->isBlank())
						{
							u16 tempX = puzzle[i][j]->getX();
							u16 tempY = puzzle[i][j]->getY();
							u16 tempID = puzzle[i][j]->getID();
							Token * tempToken = puzzle[i][j];

							puzzle[i][j]->setX(puzzle[i][j + 1]->getX());
							puzzle[i][j]->setY(puzzle[i][j + 1]->getY());
							puzzle[i][j]->setID(puzzle[i][j + 1]->getID());
							puzzle[i][j] = puzzle[i][j + 1];

							puzzle[i][j + 1]->setX(tempX);
							puzzle[i][j + 1]->setY(tempY);
							puzzle[i][j + 1]->setID(tempID);
							puzzle[i][j + 1] = tempToken;
							return;
						}
					}
				}
			}
		}
	}
}

// * Check Puzzle - We check if we ended right or not 
void GameScreen::CheckPuzzle()
{
	bool ret = true;

	u16 goodID = 0;

	for (int i = 0; i < PUZZLEDIFFICULTY; i++)
	{
		for (int j = 0; j < PUZZLEDIFFICULTY; j++)
		{
			if (puzzle[i][j] == original_puzzle[i][j])
			{
				goodID++;
			}
		}
	}

	if (goodID != PUZZLEDIFFICULTY*PUZZLEDIFFICULTY)
	{
		ret = false;
	}

	passedPuzzle(ret);
}

// * Passed Puzzle - We return to the speak with right answer or wrong answer
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
		m_time = m_maxTime;
		m_timePuzzle = m_maxTime;
	}
}

// * Reset Puzzle - We reset the puzzle 
void GameScreen::ResetPuzzle(bool ingame)
{
	m_time = m_maxTime;
	m_timePuzzle = m_time;

	if (ingame)
	{
		if (m_score >= PUZZLEPENALTY)
			m_score -= PUZZLEPENALTY;

		u16 x = 96;
		u16 y = 45;
		
		for (int i = 0; i < PUZZLEDIFFICULTY; i++)
		{
			for (int j = 0; j < PUZZLEDIFFICULTY; j++)
			{
				original_puzzle[i][j]->reset(x + j * 45, y + i * 45, (m_level * 135) + (j * 45), 508 + (i * 45), 45, 45);
			}
		}

		puzzle = original_puzzle;
		
		// Down - Right corner
		puzzle[PUZZLEDIFFICULTY - 1][PUZZLEDIFFICULTY - 1]->reset(x + (PUZZLEDIFFICULTY - 1) * 45, y + (PUZZLEDIFFICULTY - 1) * 45, 492, 155, 45, 45, true);

		for (int i = puzzle.size() - 1; i > 0; i--)
		{
			for (int j = puzzle[i].size() - 1; j > 0; j--) 
			{
				srand(static_cast<unsigned int>(time(0)));

				int m = rand () % (i + 1);
				int n = rand() % (j + 1);

				u16 tempX = puzzle[i][j]->getX();
				u16 tempY = puzzle[i][j]->getY();
				u16 tempID = puzzle[i][j]->getID();
				Token * tempToken = puzzle[i][j];

				puzzle[i][j]->setX(puzzle[m][n]->getX());
				puzzle[i][j]->setY(puzzle[m][n]->getY());
				puzzle[i][j]->setID(puzzle[m][n]->getID());
				puzzle[i][j] = puzzle[m][n];

				puzzle[m][n]->setX(tempX);
				puzzle[m][n]->setY(tempY);
				puzzle[m][n]->setID(tempID);
				puzzle[m][n] = tempToken;
			}
		}
	}
	else
	{
		srand(static_cast<unsigned int>(time(0)));

		m_level = rand() % NUMOFLEVELS;

		u16 x = 96;
		u16 y = 45;
		
		for (int i = 0; i < PUZZLEDIFFICULTY; i++) 
		{
			for (int j = 0; j < PUZZLEDIFFICULTY; j++) 
			{
				original_puzzle[i][j]->reset(x + j * 45, y + i * 45, (m_level * 135) + (j * 45), 508 + (i * 45), 45, 45);
			}
		}
		
		puzzle = original_puzzle;

		// Down - Right corner
		puzzle[PUZZLEDIFFICULTY-1][PUZZLEDIFFICULTY-1]->reset(x + (PUZZLEDIFFICULTY - 1) * 45, y + (PUZZLEDIFFICULTY - 1) * 45, 492, 155, 45, 45, true);

		for (int i = puzzle.size() - 1; i > 0; i--)
		{
			for (int j = puzzle[i].size() - 1; j > 0; j--)
			{
				srand(static_cast<unsigned int>(time(0)));

				int m = rand() % (i + 1);
				int n = rand() % (j + 1);

				u16 tempX = puzzle[i][j]->getX();
				u16 tempY = puzzle[i][j]->getY();
				u16 tempID = puzzle[i][j]->getID();
				Token * tempToken = puzzle[i][j];

				puzzle[i][j]->setX(puzzle[m][n]->getX());
				puzzle[i][j]->setY(puzzle[m][n]->getY());
				puzzle[i][j]->setID(puzzle[m][n]->getID());
				puzzle[i][j] = puzzle[m][n];

				puzzle[m][n]->setX(tempX);
				puzzle[m][n]->setY(tempY);
				puzzle[m][n]->setID(tempID);
				puzzle[m][n] = tempToken;
			}
		}
	}

}

// * Load Sentences from RomFS - 
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
		
		//m_sentences.push_back("Aaaaaand... that's it.");
		fclose(f);
	}
	else
	{
		m_sentences.push_back("Can't load the sentence file.");
	}
}

// * Load Bad Sentences from RomFS - 
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

			m_badSentences.push_back(mystring);
			sentence++;
		}

		//m_badSentences.push_back("Aaaaaand... that's it.");
		fclose(f);
	}
	else
	{
		m_badSentences.push_back("Can't load the sentence file.");
	}
}

// * End Game - 
void GameScreen::EndGame()
{

}

// * Reset Game - We reset the speaking scene
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

// * Check Score - 
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
