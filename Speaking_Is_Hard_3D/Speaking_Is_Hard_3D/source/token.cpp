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

#include "Token.h"

// * Constructor
Token::Token(u16 x, u16 y, sf2d_texture & sprite, u16 startingOffsetX, u16 startingOffsetY, u16 sizePerFrame, u16 sizeYPerFrame, u16 id) : m_sprite(sprite)
{
	m_x = x;
	m_originalX = x;
	m_y = y;
	m_originalY = y;
	m_sizePerFrame = sizePerFrame;
	m_startingXOffset = startingOffsetX;
	m_startingYOffset = startingOffsetY;
	m_moving = false;
	m_sizeYPerFrame = sizeYPerFrame;
	m_id = id;
	m_blank = false;
}

// * Destructor
Token::~Token()
{
	//delete m_sprite;
}

// * Draw token sprite
void Token::Draw(float offset, u32 opacity)
{
	sf2d_draw_texture_part_blend(&m_sprite, m_x - offset, m_y, m_startingXOffset, m_startingYOffset, m_sizePerFrame, m_sizeYPerFrame, opacity);
}

// * Update the token
bool Token::Update()
{

	return false;
}

// * if the token is moving
bool Token::isMoving()
{
	return (m_moving);
}

// * Move the token
void Token::moving(bool value, u16 dir)
{
	m_moving = value;
}

// * Move to a determinate coord
void Token::moveToCoord(int x, u16 y)
{
	m_x = x;
	m_y = y;
}

// * Get the sprite reference
sf2d_texture* Token::getSprite()
{
	return &m_sprite;
}

// * Set the sprite reference
void Token::setSprite(sf2d_texture & sprite)
{
	m_sprite = sprite;
}

// * Set a new sprite reference with different properties
void Token::setTotallyNewSprite(sf2d_texture & sprite, u16 numFrames, u16 sizePerFrame, u16 sizeYPerFrame)
{
	m_sprite = sprite;
	m_currentFrame = 0;
	m_numFrames = numFrames;
	m_sizePerFrame = sizePerFrame;
	m_sizeYPerFrame = sizeYPerFrame;
}

// * Set X position for transform position X
void Token::setX(u16 value)
{
	m_x = value;
}

// * Get X position
u16 Token::getX()
{
	return m_x;
}

// * Set Y position for transform position Y
void Token::setY(u16 value)
{
	m_y = value;
}

// * Get Y Position
u16 Token::getY()
{
	return m_y;
}

// * The token is blank
void Token::blanked(bool blank)
{
	m_blank = blank;
}

// * Get if the token is blank
bool Token::isBlank()
{
	return m_blank;
}

// * Delete this object
void Token::end()
{
	//sf2d_free_texture(&m_sprite);
	delete(this);
}

// * Set the number of frames
void Token::setNumFrames(u16 value)
{
	m_numFrames = value;
	m_currentFrame = 0;
}

// * Get ID of the token
u16 Token::getID()
{
	return m_id;
}

// * Get the number of frames
u16 Token::getNumFrames()
{
	return m_numFrames;
}

// * Set the id of the token
void Token::setID(u16 newid)
{
	m_id = newid;
}

// * Set the X frame Size
void Token::setFrameSize(u16 value)
{
	m_sizePerFrame = value;
}

// * Get the X frame Size
u16 Token::getFrameSize()
{
	return m_sizePerFrame;
}

// * Reset the token (Change the sprite and stuff)
void Token::reset(u16 x, u16 y, u16 startingOffsetX, u16 startingOffsetY, u16 sizePerFrame, u16 sizeYPerFrame, bool blank)
{
	m_x = x;
	m_originalX = x;
	m_y = y;
	m_originalY = y;
	m_sizePerFrame = sizePerFrame;
	m_startingXOffset = startingOffsetX;
	m_startingYOffset = startingOffsetY;
	m_moving = false;
	m_blank = blank;
	m_sizeYPerFrame = sizeYPerFrame;
}

// * Get the Y fframe size
u16 Token::getSizeYPerFrame()
{
	return m_sizeYPerFrame;
}

bool Token::isPressed(touchPosition touch)
{
	return (touch.px > m_x && touch.px < m_x + m_sizePerFrame && touch.py > m_y && touch.py < m_y + m_sizeYPerFrame);
}