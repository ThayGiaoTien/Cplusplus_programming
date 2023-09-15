#pragma once
#include"olcConsoleGameEngineOOP.h"
#include<string>
class cMaps
{
public:
	cMaps();
	~cMaps();
	bool GetSolid(int x, int y);
	bool Create(string name,olcSprite* sprite, wstring fileData);
	int GetIndex(int x, int y);
public:
	int nWidth, nHeight;
	string sName;
	olcSprite* pSprite;

private:
	int* m_indices;
	bool* m_solids;
};


class cMapVillage1 : public cMaps
{
public:
	cMapVillage1();
};


class cMapHome : public cMaps
{
public:
	cMapHome();
};