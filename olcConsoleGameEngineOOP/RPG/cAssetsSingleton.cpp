#include "cAssetsSingleton.h"
#include"olcConsoleGameEngineOOP.h"

cAssetsSingleton::cAssetsSingleton()
{
}

cAssetsSingleton::~cAssetsSingleton()
{
}

void cAssetsSingleton::LoadSprite()
{
	// 1. Create a lambda function which takes sName & fileName and then load its sprite add to the map
	auto load = [&](string name, wstring fileData)
		{
			olcSprite* s = new olcSprite(fileData);
			m_mapSprites[name] = s;
		};
	load("village", L"rpgdata/gfx/toml_spritesheetdark.spr");

	load("skelly", L"rpgdata/gfx/toml_Char001.png.spr");
	load("player", L"rpgdata/gfx/toml_CharacterSprites.spr");
	load("font", L"rpgdata/gfx/javidx9_nesfont8x8.spr");
	load("worldmap", L"rpgdata/gfx/worldmap1.png.spr");
	load("skymap", L"rpgdata/gfx/sky1.png.spr");
	load("title", L"rpgdata/gfx/title3.png.spr");
	load("balloon", L"rpgdata/gfx/balloon1.png.spr");
	load("sword", L"rpgdata/gfx/Sword.spr");
	load("hitech", L"rpgdata/gfx/toml_modernish.spr");

	load("purple", L"rpgdata/gfx/toml_purple.spr");

	load("health", L"rpgdata/gfx/item_health.spr");
	load("healthboost", L"rpgdata/gfx/item_healthboost.spr");

	load("Basic Sword", L"rpgdata/gfx/weapon_basic_sword.spr");
}

olcSprite* cAssetsSingleton::GetSprite(string name)
{
	return m_mapSprites[name];
}

void cAssetsSingleton::LoadMap()
{	
	//
}
