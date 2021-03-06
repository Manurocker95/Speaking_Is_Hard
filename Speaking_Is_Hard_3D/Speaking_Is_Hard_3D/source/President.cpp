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

#include "President.h"

// * Constructor
President::President(int x, u16 y, sf2d_texture & sprite, bool multipleFrames, u16 numFrames, u16 sizePerFrame, u16 sizeYPerFrame) : m_sprite(sprite)
{
	m_x = x;
	m_originalX = x;
	m_y = y;
	m_originalY = y;
	m_multipleFrames = multipleFrames;
	m_numFrames = numFrames;
	m_sizePerFrame = sizePerFrame;
	m_currentFrame = 0;
	m_secondaryCounter = 0;
	m_state = WALKING_RIGHT;
	m_offsetFrameX = 0;
	m_startingYOffset = 0;
	m_timer = 0;
	m_timeToSpeak = 12;
	m_timeSpeaking = 300;
	m_stage = 0;
	m_moving = false;
	m_sizeYPerFrame = sizeYPerFrame;
	m_movement = PRESIDENTMOVEMENT;
}

// * Destructor
President::~President()
{
	//delete m_sprite;
}

// * Draw
void President::Draw(float offset, u32 opacity)
{
	if (m_state == NONE)
	{
		return;
	}

	if (m_multipleFrames)
	{
		sf2d_draw_texture_part_blend(&m_sprite, m_x - offset, m_y, m_currentFrame*m_sizePerFrame, m_startingYOffset, m_sizePerFrame, m_sizeYPerFrame, opacity);
	}
	else
	{
		sf2d_draw_texture(&m_sprite, m_x, m_y);
	}
}

// * Update
bool President::Update()
{
	if (m_state == NONE)
	{
		return true;
	}

	if (m_state == SPEAKING || m_state == MISTAKE)
	{
		m_timer++;
		if (m_timer > m_timeSpeaking)
		{
			m_timer = 0;
			wait();
			return false;
		}
	}

	m_secondaryCounter++;

	if (m_secondaryCounter > FRAMECOUNTERPRESIDENT)
	{
		m_currentFrame++;
		m_secondaryCounter = 0;

		if (m_state == WALKING_RIGHT)
		{
			if (m_x < 320)
			{
				m_x += PRESIDENTMOVEMENT;
			}
			else
			{
				moving(false, 2);
			}
		}
		else if (m_state == WALKING_LEFT)
		{
			if (m_x > -67)
			{
				m_x -= PRESIDENTMOVEMENT;
			}
			else
			{
				moving(false, 2);
			}
		}
		else if (m_state == STANDING)
		{
			m_timer++;

			if (m_timer > m_timeToSpeak)
			{
				m_timer = 0;
				m_timeToSpeak = 12;
				speak();
			}
		}
	}

	if (m_currentFrame >= m_numFrames)
	{
		m_currentFrame = 0;
	}

	return false;
}

// * Change to Speak
void President::speak()
{
	m_state = SPEAKING;
	m_startingYOffset = 127;
	m_currentFrame = 0;
	m_sizePerFrame = 43;
	m_sizeYPerFrame = 127;
	m_numFrames = 4;

}

// * Change to Mistake
void President::mistake()
{
	m_state = MISTAKE;
	m_startingYOffset = 643;
	m_currentFrame = 0;
	m_sizePerFrame = 43;
	m_sizeYPerFrame = 127;
	m_numFrames = 4;

}

// * Check if we are speaking
bool President::isSpeaking()
{
	return (m_state == SPEAKING);
}

// * Check if we are standing
bool President::isStanding()
{
	return (m_state == STANDING);
}

// * Check if we are waiting
bool President::isWaiting()
{
	return (m_state == WAITING);
}

// * Check if we are mistaking
bool President::isMistaking()
{
	return (m_state == MISTAKE);
}

// * Moving the president right or left
void President::moving(bool value, u16 dir)
{
	m_moving = value;

	if (dir == 0)
	{
		m_state = WALKING_RIGHT;
		m_startingYOffset = 0;
		m_currentFrame = 0;
		m_sizePerFrame = 67;
		m_sizeYPerFrame = 127;
		m_numFrames = 8;
	}
	else if (dir == 1)
	{
		m_state = WALKING_LEFT;
		m_startingYOffset = 256;
		m_currentFrame = 0;
		m_sizePerFrame = 67;
		m_sizeYPerFrame = 127;
		m_numFrames = 8;
	}
	else
	{
		m_state = STANDING;
		m_startingYOffset = 381;
		m_currentFrame = 0;
		m_sizePerFrame = 43;
		m_sizeYPerFrame = 127;
		m_numFrames = 6;
	}
}

// * Wait for transition after speaking
void President::wait()
{
	m_state = WAITING;
	m_startingYOffset = 381;
	m_currentFrame = 0;
	m_sizePerFrame = 43;
	m_sizeYPerFrame = 127;
	m_numFrames = 6;
}

// * Move to certain coordinate
void President::moveToCoord(int x, u16 y)
{
	m_x = x;
	m_y = y;
}

// * Move (same as moving but adapted)
void President::move(int value, u16 dir)
{
	if (m_state == NONE)
	{
		return;
	}

	m_moving = true;

	m_offsetFrameX = dir;

	if (dir == 0)
	{
		switch (m_stage)
		{
		case 0:
			m_startingYOffset = 0;
			break;
		case 2:
			m_startingYOffset = 52;
			break;
		case 4:
			m_startingYOffset = 124;
			break;
		default:
			break;
		}

	}
	else
	{
		switch (m_stage)
		{
		case 0:
			m_startingYOffset = 26;
			break;
		case 2:
			m_startingYOffset = 88;
			break;
		case 4:
			m_startingYOffset = 154;
			break;
		default:
			break;
		}
	}

	m_x += value*m_movement;

	if (m_x > TOP_WIDTH)
	{
		m_x = 0;
	}

	if (m_x < 0 - m_sizePerFrame)
	{
		m_x = TOP_WIDTH;
	}
}

// * Get the sprite reference 
sf2d_texture* President::getSprite()
{
	return &m_sprite;
}

// * Set the sprite reference
void President::setSprite(sf2d_texture & sprite)
{
	m_sprite = sprite;
}

// * Set new sprite with animations set by frames
void President::setTotallyNewSprite(sf2d_texture & sprite, u16 numFrames, u16 sizePerFrame, u16 sizeYPerFrame)
{
	m_sprite = sprite;
	m_currentFrame = 0;
	m_numFrames = numFrames;
	m_sizePerFrame = sizePerFrame;
	m_sizeYPerFrame = sizeYPerFrame;
	m_stage = 0;
}

// * Set the transform position X to a X position
void President::setX(int value)
{
	m_x = value;
}

// * Get the transform position X
int President::getX()
{
	return m_x;
}

// * Set the transform position Y to a Y position
void President::setY(u16 value)
{
	m_y = value;
}

// * Get the transform position Y
u16 President::getY()
{
	return m_y;
}

// * End - We delete this object
void President::end()
{
	//sf2d_free_texture(&m_sprite);
	delete(this);
}

// * Get the state 
President::MState President::getState()
{
	return m_state;
}

// * Set the state
void President::setState(President::MState value)
{
	m_state = value;
}

// * Set the number of frames for the current animation
void President::setNumFrames(u16 value)
{
	m_numFrames = value;
	m_currentFrame = 0;
}

// * Get the number of frames in the current animation
u16 President::getNumFrames()
{
	return m_numFrames;
}

// * Frame Size in X axis
void President::setFrameSize(u16 value)
{
	m_sizePerFrame = value;
}

// * Get Frame Size in X axis
u16 President::getFrameSize()
{
	return m_sizePerFrame;
}

// * Reset the variables for reseting the game
void President::reset()
{
	m_timer = 0;
	m_timeToSpeak = 12;
	m_x = m_originalX;
	m_y = m_originalY;
	m_currentFrame = 0;
	m_secondaryCounter = 0;
	m_state = WALKING_RIGHT;
	m_offsetFrameX = 0;
	m_moving = false;
	m_sizePerFrame = 20;
	m_sizeYPerFrame = 26;
	m_timeSpeaking = 300;
	m_startingYOffset = 0;
	m_stage = 0;
	m_movement = PRESIDENTMOVEMENT;
}

// * Get Frame Size in Y axis
u16 President::getSizeYPerFrame()
{
	return m_sizeYPerFrame;
}