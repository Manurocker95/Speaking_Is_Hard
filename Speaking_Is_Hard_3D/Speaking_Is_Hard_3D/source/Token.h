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

#ifndef _TOKEN_H
#define _TOKEN_H

#include "sf2d.h"
#include "sfil.h"
#include "filepaths.h"
#include "Settings.h"

class Token
{
public:

	Token();
	Token(u16 x, u16 y, sf2d_texture & sprite, u16 startingOffsetX, u16 startingOffsetY, u16 sizePerFrame, u16 sizeYPerFrame, u16 id);
	~Token();

	void Draw(float offset, u32 opacity);
	bool Update();
	u16 getX();
	void setX(u16 value);
	u16 getY();
	void setY(u16 value);
	void move(int value, u16 dir);
	void moveToCoord(int x, u16 y);
	sf2d_texture* getSprite();
	void setSprite(sf2d_texture & sprite);
	void setTotallyNewSprite(sf2d_texture& sprite, u16 numFrames, u16 sizePerFrame, u16 sizeYPerFrame);
	void end();
	void setNumFrames(u16 value);
	u16 getNumFrames();
	void setFrameSize(u16 value);
	u16 getFrameSize();
	void reset(u16 x, u16 y, u16 startingOffsetX, u16 startingOffsetY, u16 sizePerFrame, u16 sizeYPerFrame);
	u16 getSizeYPerFrame();
	void moving(bool value, u16 dir);
	bool isMoving();
	u16 getID;

private:

	u16 m_x, m_originalX;
	u16 m_y, m_originalY;
	u16 m_numFrames;
	u16 m_sizePerFrame;
	u16 m_sizeYPerFrame;
	u16 m_currentFrame;
	u16 m_secondaryCounter;
	u16 m_offsetFrameX;
	u16 m_startingXOffset;
	u16 m_startingYOffset;
	u16 m_id;
	bool m_moving;
	bool m_multipleFrames;

	sf2d_texture & m_sprite;

};

#endif
