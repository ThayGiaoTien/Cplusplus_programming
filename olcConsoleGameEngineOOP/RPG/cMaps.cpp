#include "cMaps.h"
#include "cAssetsSingleton.h"
#include<fstream>


cMaps::cMaps()
{
	// Declare all properties
	nWidth = 0;
	nHeight = 0;
	m_indices = nullptr;
	m_solids = nullptr;
}

cMaps::~cMaps()
{
	delete(m_indices);
	delete(m_solids);
}

bool cMaps::GetSolid(int x, int y)
{
	if (m_solids[y * nWidth + x] == 1) return true;
	return false;
}

int cMaps::GetIndex(int x, int y)
{
	return m_indices[y*nWidth+x];
}

bool cMaps::Create(string name,olcSprite* sprite, wstring fileData)
{
	sName = name;
	pSprite = sprite;
	// Read map's data from the source file.
	ifstream data(fileData, ios::in | ios::binary);
	if (data.is_open())
	{
		data >> nWidth >> nHeight;
		m_indices = new int[nWidth * nHeight];
		m_solids = new bool[nWidth * nHeight];
		for (int i = 0; i < nWidth * nHeight; i++)
		{
			data >> m_indices[i];
			data >> m_solids[i];
		}
		return true;
	}
	return false;
}

cMapVillage1::cMapVillage1()
{
	Create("village", cAssetsSingleton::get().GetSprite("village"), L"./rpgdata/map/village1.lvl");
}	

cMapHome::cMapHome()
{
	Create("home", cAssetsSingleton::get().GetSprite("hitech"), L"./rpgdata/map/home.lvl");
}
