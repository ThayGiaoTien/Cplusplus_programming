#include<iostream>
#include<fstream>
#include<string>
#include<utility>
#include<vector>

#include"olcConsoleGameEngineOOP.h""
#include"cMaps.h"
#include"cAssetsSingleton.h"
#include"cDynamic.h"



class RPG_Game : public olcConsoleGameEngineOOP
{
public:


private:
	cMaps* m_pCurrentMap;
	olcSprite* m_pFont;
	cDynamicCreature* m_pPlayer;
	
	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;
	
		

protected:
	virtual bool OnUserCreate()
	{
		cAssetsSingleton::get().LoadSprite();
		m_pCurrentMap = new cMapVillage1();
		//m_pCurrentMap->pSprite = cAssetsSingleton::get().GetSprite("village");
		m_pFont = cAssetsSingleton::get().GetSprite("font");
		m_pPlayer = new cDynamicCreature("player", cAssetsSingleton::get().GetSprite("player"));
		float m_fPlayerX = 10.0f;
		float m_fPlayerY = 10.0f;
		float m_fPlayerVelX = 0.0f;
		float m_fPlayerVelY = 0.0f;
		m_pPlayer->px = m_fPlayerX;
		m_pPlayer->py = m_fPlayerY;
		m_pPlayer->vx = m_fPlayerVelX;
		m_pPlayer->vy = m_fPlayerVelY;



		return true;
	}
	
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, BG_BLUE);

		m_pPlayer->vx = 0.0f;
		m_pPlayer->vy = 0.0f;

		// User input===================================
		if (GetKey(VK_UP).bHeld)
		{
			m_pPlayer->vy = -4.0f;
		}

		if (GetKey(VK_DOWN).bHeld)
		{
			m_pPlayer->vy = 4.0f;
		}

		if (GetKey(VK_LEFT).bHeld)
		{
			m_pPlayer->vx = -4.0f;
		}

		if (GetKey(VK_RIGHT).bHeld)
		{
			m_pPlayer->vx = 4.0f;
		}


		// Physics udpate ===============================
		cDynamic* object = m_pPlayer;
		
		// Calculate posibility new position and update
		float fNewObjectPosX = object->px + object->vx * fElapsedTime;
		float fNewObjectPosY = object->py + object->vy * fElapsedTime;

		// Collision
		if (object->vx <= 0)
		{
			if (m_pCurrentMap->GetSolid(fNewObjectPosX + 0.0f, object->py + 0.0f) || m_pCurrentMap->GetSolid(fNewObjectPosX + 0.0f, object->py + 0.9f))
			{
				fNewObjectPosX = (int)fNewObjectPosX + 1;
				object->vx = 0;
			}
		}
		else
		{
			if (m_pCurrentMap->GetSolid(fNewObjectPosX + 1.0f, object->py + 0.0f) || m_pCurrentMap->GetSolid(fNewObjectPosX + 1.0f, object->py + 0.9f))
			{
				fNewObjectPosX = (int)fNewObjectPosX;
				object->vx = 0;

			}
		}

		if (object->vy <= 0)
		{
			if (m_pCurrentMap->GetSolid(fNewObjectPosX + 0.0f, fNewObjectPosY) || m_pCurrentMap->GetSolid(fNewObjectPosX + 0.9f, fNewObjectPosY))
			{
				fNewObjectPosY = (int)fNewObjectPosY + 1;
				object->vy = 0;
			}
		}
		else
		{
			if (m_pCurrentMap->GetSolid(fNewObjectPosX + 0.0f, fNewObjectPosY + 1.0f) || m_pCurrentMap->GetSolid(fNewObjectPosX + 0.9f, fNewObjectPosY + 1.0f))
			{
				fNewObjectPosY = (int)fNewObjectPosY;
				object->vy = 0;
			}
		}
		// Update
		object->px = fNewObjectPosX;
		object->py = fNewObjectPosY;

		// Call update object before camera positon
		object->Update(fElapsedTime);

		
		// Camera
		fCameraPosX = object->px;
		fCameraPosY = object->py;

		// Draw map=====================================
		// Draw Level
		int nTileWidth = 16;
		int nTileHeight = 16;
		int nVisibleTilesX = ScreenWidth() / nTileWidth;
		int nVisibleTilesY = ScreenHeight() / nTileHeight;

		// Calculate Top-Leftmost visible tile
		float fOffsetX = fCameraPosX - (float)nVisibleTilesX / 2.0f;
		float fOffsetY = fCameraPosY - (float)nVisibleTilesY / 2.0f;

		// Clamp camera to game boundaries
		if (fOffsetX < 0) fOffsetX = 0;
		if (fOffsetY < 0) fOffsetY = 0;
		if (fOffsetX > m_pCurrentMap->nWidth - nVisibleTilesX) fOffsetX = m_pCurrentMap->nWidth - nVisibleTilesX;
		if (fOffsetY > m_pCurrentMap->nHeight - nVisibleTilesY) fOffsetY = m_pCurrentMap->nHeight - nVisibleTilesY;

		// Get offsets for smooth movement
		float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
		float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;

		// Draw visible tile map
		for (int x = -1; x < nVisibleTilesX + 1; x++)
		{
			for (int y = -1; y < nVisibleTilesY + 1; y++)
			{
				int idx = m_pCurrentMap->GetIndex(x + fOffsetX, y + fOffsetY);
				int sx = idx % 10;
				int sy = idx / 10;
				
				DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, m_pCurrentMap->pSprite, sx * nTileWidth, sy * nTileHeight, nTileWidth, nTileHeight);
			}
		}
		for(int x=0; x<m_pCurrentMap->nWidth; x++)
			for (int y = 0; y < m_pCurrentMap->nHeight; y++)
			{
				if (m_pCurrentMap->GetSolid(x, y))
				{
					Fill(x * 16, y * 16, x * 16 + 8, y * 16 + 8, PIXEL_SOLID, FG_RED);
				}
			}
		DrawBigText("Hello mother fucker! ");

		// Draw player
		object->DrawSelf(this, 0, 0);

		return true;
	}

// UTILITIES===================================================
public:
	void DrawBigText(string text)
	{
		int i = 0;
		for (auto c : text)
		{
			int sx = (c - 32) % 16 * 8;
			int sy = (c - 32) / 16 * 8;
			DrawPartialSprite(20 + i * 8, 20, m_pFont, sx , sy , 8, 8);
			i++;
		}

	}
};


int main()
{
	RPG_Game game;
	game.ConstructConsole(256, 240, 4, 4);
	game.Start();

	return 0;
}