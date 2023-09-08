
#include<Windows.h>
#include <iostream>
#include<string>
#include<chrono>
#include<time.h>
#include<utility>
#include<vector>
#include<algorithm>


using namespace std;

int const nScreenWidth = 120, nScreenHeight = 40;
int const nMapWidth = 32, nMapHeight = 32;

int main()
{
	// Game State
	float fPlayerX = 2.0f;
	float fPlayerY = 2.0f;
	float fPlayerA= 0.0f;
	float fFOV = 3.14159f / 4.0f;

	float fDepth = 16.0f;
	
	wstring wsMap;
	wsMap.append(L"################");
	wsMap.append(L"#..............#");
	wsMap.append(L"#..............#");
	wsMap.append(L"#..............#");
	wsMap.append(L"#....##........#");
	wsMap.append(L"#.........##...#");
	wsMap.append(L"#..............#");
	wsMap.append(L"#..............#");
	wsMap.append(L"#..............#");
	wsMap.append(L"#..............#");
	wsMap.append(L"#..............#");
	wsMap.append(L"#..............#");
	wsMap.append(L"#.......####...#");
	wsMap.append(L"#..............#");
	wsMap.append(L"#..............#");
	wsMap.append(L"################");

	
		// Create Screen Buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L'$';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;


	chrono::system_clock::time_point tp1, tp2;
	chrono::duration<float> elapsedTime;
	tp1 = chrono::system_clock::now();
	while (1)
	{
		// Calculate cycle clock
		tp2 = chrono::system_clock::now();
		elapsedTime = tp2 - tp1;
		float fElapsedTime = elapsedTime.count();
		tp1 = tp2;

		// User input handle
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000 ) 
			fPlayerA -= 0.9f * fElapsedTime;
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) 
			fPlayerA += 0.9f * fElapsedTime;

		if (GetAsyncKeyState((unsigned short)'W') & 0x8000 )
		{
			fPlayerX += cosf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY += sinf(fPlayerA) * 5.0f * fElapsedTime;
			if (wsMap.c_str()[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#' )
			{
				fPlayerX -= cosf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY -= sinf(fPlayerA) * 5.0f * fElapsedTime;
			}		
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000 )
		{
			fPlayerX -= cosf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY -= sinf(fPlayerA) * 5.0f * fElapsedTime;
			if (wsMap.c_str()[(int)fPlayerY* nMapWidth + (int)fPlayerX] == '#')
			{
				fPlayerX += cosf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY += sinf(fPlayerA) * 5.0f * fElapsedTime;
			}
		}


		// RAY-CASTING
		for (int x = 0; x < nScreenWidth; x++)
		{// For each column, calculate the projected ray angle into world space
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

			float fStepSize = 0.1f;		  // Increment size for ray casting, decrease to increase										
			float fDistanceToWall = 0.0f; //                                      resolution

			bool bHitWall = false;		// Set when ray hits wall block
			bool bBoundary = false;		// Set when ray hits boundary between two wall blocks

			float fEyeX = cosf(fRayAngle); // Unit vector for ray in player space
			float fEyeY = sinf(fRayAngle);

			// Incrementally cast ray from player, along ray angle, testing for 
			// intersection with a block
			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += fStepSize;
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				// Test if ray is out of bounds
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
				{
					bHitWall = true;			// Just set distance to maximum depth
					fDistanceToWall = fDepth;
				}
				else
				{
					// Ray is in bound, so ray's positions relative to map
					if (wsMap.c_str()[nTestY * nMapWidth + nTestX] == '#')
					{
						bHitWall = true;
						vector<pair<float, float>> p;
						float fBoundary = 0.01f;

						// Find the boundaries
						// Cast 4 ray to the 4 point of boundaries
						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{
								float vx = (float)nTestX - fPlayerX + tx;
								float vy = (float)nTestY - fPlayerY + ty;
								float d = sqrtf(vx * vx + vy * vy);
								float dot = fEyeX * vx / d + fEyeY * vy / d;
								p.push_back(make_pair(d, dot));
							}
						// Once we have vector, we need to sort it using lambda
						sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right)
							{
								return left.first < right.first;
							}
						);

						// We will take the inverse cosin of 2 first sorted pair to get the angle between the ray-casting and ray from player to boundaries.
						float fRay1 = acosf(p.at(0).second);
						float fRay2 = acosf(p.at(1).second);
						if (fRay1 < fBoundary)
							bBoundary = true;
						if (fRay2 < fBoundary)
							bBoundary = true;
					}

				}
			}

			float fCeiling = float(nScreenHeight / 2.0f) - float(nScreenHeight) / fDistanceToWall;
			float fFloor = (float)nScreenHeight - fCeiling;
			wchar_t wcShade = L' ';

			// Calculate wcShade depend on distance to walls
			if (fDistanceToWall < fDepth / 4.0f) wcShade = 0x2588;
			else if (fDistanceToWall < fDepth / 3.0f) wcShade = 0x2593;
			else if (fDistanceToWall < fDepth / 2.0f) wcShade = 0x2592;
			else if (fDistanceToWall < fDepth / 1.0f) wcShade = 0x2591;
			else wcShade = L' ';
			if (bBoundary) wcShade = ' ';
			// Now, loop through each row of screen to display ceiling, walls and floor
			for (int y = 0; y < nScreenHeight; y++)
			{
				
				if (y <= fCeiling)
					screen[y * nScreenWidth + x] = ' ';
				else if (y < fFloor && y>= fCeiling)
				{	
					screen[y * nScreenWidth + x] = wcShade;
				}
				else
				{
					// Here is place to display walls based on distance to walls
					float b = 1.0f - (float)(y - nScreenHeight / 2) / (float)(nScreenHeight / 2);
					if (b < 0.25f) wcShade = L'#';
					else if (b < 0.5f) wcShade = L'x';
					else if (b < 0.75f) wcShade = L'.';
					else if (b < 0.9f) wcShade = L'-';
					else wcShade = L' ';
					screen[y * nScreenWidth + x] = wcShade;
				}

				
			}
		}

		// Display game state
		for(int x=0; x<nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				screen[(y + 1) * nScreenWidth + x] = wsMap.c_str()[y * nMapWidth + x];
			}
		screen[(int)(fPlayerY + 1) * nScreenWidth + (int)fPlayerX] = 'P';
		swprintf(&screen[30], 80, L"PlayerX: %f, PlayerY: %f, Angle: %f , FPS: %d", fPlayerX, fPlayerY, fPlayerA, int(1.0f / fElapsedTime));
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	CloseHandle(hConsole);
	
	
   
}


