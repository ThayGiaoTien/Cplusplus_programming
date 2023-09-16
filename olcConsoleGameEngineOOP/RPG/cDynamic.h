#pragma once
#include"olcConsoleGameEngineOOP.h"

class cDynamic
{
public:
	cDynamic(string name);
	~cDynamic();

public:
	float px, py, vx, vy;
	string sName;
	bool bSolidVsMap;
	bool bSolidVsDyn;
	bool bFriendly;

public:
	virtual void DrawSelf(olcConsoleGameEngineOOP* gfx, float ox, float oy); // ox, oy: screen offset
	virtual void Update(float fElapsedTime);

};

class cDynamicCreature : public cDynamic
{
public:
	cDynamicCreature(string name, olcSprite* sprite);
	

private:
	olcSprite* pSprite;
	int nHealth;
	int nMaxHealth;
	enum {
		SOUTH, WEST, NORTH, EAST
	};
	int m_nFacingDirection;
	enum {
		STANDING, WALKING, CELEBRATING, DEAD
	};
	int m_nGraphicState;
	float fTimer;
	int m_nGraphicCounter;


public:
	void DrawSelf(olcConsoleGameEngineOOP* gfx, float ox, float oy) override;
	void Update(float fElapsedTime) override;

};


