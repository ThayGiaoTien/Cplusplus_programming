#include "cDynamic.h"

cDynamic::cDynamic(string name)
{
	px = 0;
	py = 0;
	vx = 0;
	vy = 0;
	bSolidVsDyn = true;
	bSolidVsMap = true;
	bFriendly = true;
	sName = name;

}

cDynamic::~cDynamic()
{
}

void cDynamic::DrawSelf(olcConsoleGameEngineOOP* gfx, float ox, float oy)
{

}

void cDynamic::Update(float fElapsedTime)
{
}



cDynamicCreature::cDynamicCreature(string name, olcSprite* sprite) : cDynamic(name)
{
	pSprite = sprite;
	fTimer = 0.0f;
	m_nGraphicState = STANDING;
	m_nFacingDirection = SOUTH;
	nHealth = 10;
	nMaxHealth = 10;
	m_nGraphicCounter = 0;
}

void cDynamicCreature::DrawSelf(olcConsoleGameEngineOOP* gfx, float ox, float oy)
{
	// Draw based on updated states
	int nSheetOffsetX = 0;
	int nSheetOffsetY = 0;
	switch (m_nGraphicState)
	{
	case STANDING:
		nSheetOffsetX = m_nFacingDirection * 16;
		break;
	case WALKING:
		nSheetOffsetX = m_nFacingDirection * 16;
		nSheetOffsetY = m_nGraphicCounter * 16;
		break;
	case CELEBRATING:
		nSheetOffsetX = 4 * 16;
		break;
	case DEAD:
		nSheetOffsetX = 4 * 16;
		nSheetOffsetY = 1 * 16;
		break;
	}
	
	gfx->DrawPartialSprite((px - ox) * 16, (py - oy) * 16, pSprite, nSheetOffsetX, nSheetOffsetY, 16, 16);
}

void cDynamicCreature::Update(float fElapsedTime)
{
	fTimer += fElapsedTime;
	// Update after every 0.2 ms
	if (fTimer >= 0.2f)
	{
		fTimer -= 0.2f;
		m_nGraphicCounter++ ; // Determine left or right	
		m_nGraphicCounter %= 2; 
	}

	// Determine graphic state based on velocity
	if (vx != 0 || vy != 0) m_nGraphicState = WALKING;
	else m_nGraphicState = STANDING;

	if (nHealth == 0) m_nGraphicState = DEAD;

	// Determine facing direction based on velocity too.
	if (vx > 0) m_nFacingDirection = EAST;
	if (vx < 0) m_nFacingDirection = WEST;
	if (vy > 0) m_nFacingDirection = SOUTH;
	if (vy < 0) m_nFacingDirection = NORTH;


}
