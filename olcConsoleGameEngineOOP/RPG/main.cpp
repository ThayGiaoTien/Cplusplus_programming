#include<iostream>
#include<fstream>
#include<string>
#include<utility>
#include<vector>

#include"olcConsoleGameEngineOOP.h""
#include"cMaps.h"
#include"cAssetsSingleton.h"



class RPG_Game : public olcConsoleGameEngineOOP
{
public:


private:
	cMaps* m_pCurrentMap;


		

protected:
	virtual bool OnUserCreate()
	{
		cAssetsSingleton::get().LoadSprite();
		m_pCurrentMap = new cMapHome();
		//m_pCurrentMap->pSprite = cAssetsSingleton::get().GetSprite("village");

		float fPlayerX = 10;
		float fPlayerY = 15;
		return true;
	}
	
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, BG_BLUE);

		// Draw map
		int nTileWidth = 16;
		int nTileHeight = 16;
		for (int x = 0; x < m_pCurrentMap->nWidth; x++)
		{
			for (int y = 0; y < m_pCurrentMap->nHeight; y++)
			{
				int idx = m_pCurrentMap->GetIndex(x, y);
				int sx = idx % 10;
				int sy = idx / 10;
				DrawPartialSprite(x*nTileWidth, y*nTileHeight, m_pCurrentMap->pSprite, sx*nTileWidth, sy*nTileHeight, nTileWidth, nTileHeight);
			}
		}


		return true;
	}
};


int main()
{
	RPG_Game game;
	game.ConstructConsole(256, 240, 4, 4);
	game.Start();

	return 0;
}