#include "Token.h"

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
}


Token::~Token()
{
	//delete m_sprite;
}

void Token::Draw(float offset, u32 opacity)
{
	sf2d_draw_texture_part_blend(&m_sprite, m_x - offset, m_y, m_startingXOffset, m_startingYOffset, m_sizePerFrame, m_sizeYPerFrame, opacity);
}

bool Token::Update()
{

	return false;
}

bool Token::isMoving()
{
	return (m_moving);
}

void Token::moving(bool value, u16 dir)
{
	m_moving = value;
}

void Token::moveToCoord(int x, u16 y)
{
	m_x = x;
	m_y = y;
}

sf2d_texture* Token::getSprite()
{
	return &m_sprite;
}

void Token::setSprite(sf2d_texture & sprite)
{
	m_sprite = sprite;
}

void Token::setTotallyNewSprite(sf2d_texture & sprite, u16 numFrames, u16 sizePerFrame, u16 sizeYPerFrame)
{
	m_sprite = sprite;
	m_currentFrame = 0;
	m_numFrames = numFrames;
	m_sizePerFrame = sizePerFrame;
	m_sizeYPerFrame = sizeYPerFrame;
}

void Token::setX(u16 value)
{
	m_x = value;
}

u16 Token::getX()
{
	return m_x;
}

void Token::setY(u16 value)
{
	m_y = value;
}

u16 Token::getY()
{
	return m_y;
}

void Token::end()
{
	//sf2d_free_texture(&m_sprite);
	delete(this);
}

void Token::setNumFrames(u16 value)
{
	m_numFrames = value;
	m_currentFrame = 0;
}

u16 Token::getNumFrames()
{
	return m_numFrames;
}

void Token::setFrameSize(u16 value)
{
	m_sizePerFrame = value;
}

u16 Token::getFrameSize()
{
	return m_sizePerFrame;
}

void Token::reset(u16 x, u16 y, u16 startingOffsetX, u16 startingOffsetY, u16 sizePerFrame, u16 sizeYPerFrame)
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
}

u16 Token::getSizeYPerFrame()
{
	return m_sizeYPerFrame;
}