#include<iostream>
#include<fstream>
#include<string>
#include<utility>

#include"olcConsoleGameEngineOOP.h"


class RPG_Game : public olcConsoleGameEngineOOP
{
public:


private:

protected:
	virtual bool OnUserCreate()
	{

		return true;
	}
	
	virtual bool OnUserUpdate(float fElapsedTime)
	{


		return true;
	}
};


int main()
{
	RPG_Game game;
	game.ConstructConsole(320, 240, 8, 8);
	game.Start();

	return 0;
}
