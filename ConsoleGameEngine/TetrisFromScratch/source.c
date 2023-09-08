#include <iostream>
#include <thread>
#include <vector>
using namespace std;

#include <stdio.h>
#include <Windows.h>

int nScreenWidth = 80;			// Console Screen Size X (columns)
int nScreenHeight = 30;			// Console Screen Size Y (rows)
wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr;

int nRotate(int nCurrentRotation, int px, int py)
{
	int pi = 0;   
	
	switch (nCurrentRotation % 4)
	{
	case 0:
		pi = py * 4 + px;
		break;
	case 1:
		pi = 12 + py - 4 * px;
		break;
	case 2:
		pi = 15 - py * 4 - px;
		break;
	case 3:
		pi = 3 - py + px * 4;
		break;
	} 
	return pi;
}

bool bDoesPieceFit(int nCurrentPiece, int nCurrentRotation, int nPosX, int nPosY)
{
	// Get index into piece & index into field then check collision
	for(int px= 0; px<4; px++)
		for (int py = 0; py < 4; py++)
		{
			int pi =nRotate(nCurrentRotation, px, py);
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);
			// Check in bound
			if (px + nPosX < nFieldWidth  && px + nPosX>=0 && py + nPosY < nFieldHeight)
				if (tetromino[nCurrentPiece][pi] == L'#' && pField[fi] != 0)// pField[ fi] != L' '  ^^^^^^^^^^^^^^^^^^^^^66666
					return false;
		}

	return true;
}
int main()
{
	// GAME STATE====================
	tetromino[0].append(L"..#..##...#.....");
	tetromino[1].append(L".....##..##.....");
	tetromino[2].append(L".#...#...##.....");
	tetromino[3].append(L"..#..#...#...#..");
	tetromino[4].append(L"..#..##..#......");
	tetromino[5].append(L".#...##...#.....");
	tetromino[6].append(L"..#...#..##.....");


	// I forgoted to declare pFieldSize ^^^^^^^^^^^^^^^^^^^^^^^^^^^
	pField = new unsigned char[nFieldWidth * nFieldHeight];

	for(int x= 0; x<nFieldWidth; x++)
		for (int y = 0; y < nFieldHeight; y++)
		{
				pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight -1 ) ? 9 : 0;
		}

	wchar_t* screen= new wchar_t(nScreenWidth * nScreenHeight);
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
		screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;


	bool bKey[4];
	
	int nCurrentPiece = rand()%7;
	int nCurrentRotation = rand()%4;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;

	bool bRotateHold = true;

	int nTimeCount = 0;
	int nSpeedCount = 0;

	int nSpeed = 20;
	int nScore = 0;

	vector<int> vLines;
	bool bGameOver = false;

	// GAME LOOP=====================
	while (!bGameOver)
	{
		// Game Timing
		this_thread::sleep_for(100ms);
		nTimeCount++;
		bool bForceDown = (nTimeCount % 10 == 0);
		
		// User Input
		
		for (int k = 0; k < 4; k++)
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0; // R L D Z
		
		nCurrentY += (bKey[2] && bDoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;
		nCurrentX -= (bKey[1] && bDoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
		nCurrentX += (bKey[0] && bDoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;

		if (bKey[3])
		{
			nCurrentRotation += (bRotateHold && bDoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
			bRotateHold = false;   // Prevent duplication of rotate before of clock cycle is ~ 50ms. 
		}
		else {
			bRotateHold = true;
		}

		// Force the piece down for every second
		if (bForceDown)
		{
			if (bDoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
			{
				nCurrentY++;
			}
			else
			{
				// Lock the current piece in the field
				for(int x= 0; x<4; x++)
					for (int y = 0; y < 4; y++)
					{
						if (tetromino[nCurrentPiece][nRotate(nCurrentRotation, x, y)] == L'#')
						{
							pField[(nCurrentY + y) * nFieldWidth + (nCurrentX + x)] = nCurrentPiece+1;
						}
					}

				// Check have any lines
				for (int y = 0; y < 4; y++)
				{
					if (nCurrentY + y < nFieldHeight - 1)
					{
						bool bLine = true;
						for (int x = 1; x < nFieldWidth - 1; x++)
							bLine &= (pField[(nCurrentY + y) * nFieldWidth + x] != 0);
						
						if (bLine)
						{
							for (int x = 1; x < nFieldWidth - 1; x++)
							{
								
								pField[(nCurrentY + y) * nFieldWidth + x] = 8;

							}
							vLines.push_back(nCurrentY + y);  // You just need to pushback once. ^^^^^^^^^^^^^^^^^^^^^
						}
							

					}
				}

				nScore += 50;
				if (!vLines.empty()) nScore += 100<<vLines.size();
				
				// Choose next piece
				nCurrentPiece = rand()%7;
				nCurrentRotation = rand()%4;
				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;

				// Game over
				bGameOver = !bDoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
			}
		}

		// Score
		if (!vLines.empty())
		{
			// Refresh Frame
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
			// Delay for 400ms
			this_thread::sleep_for(400ms);
			// Delete the scored row
			for (auto& v : vLines)
			{
				for (int px = 1; px < nFieldWidth-1; px++)
				{
					for (int py=v; py > 0; py--)
					{
						pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px]; //Move down 1 line
					}
					pField[px] = 0; // Remove first row above.
				}
			}
			vLines.clear();
		}



		// Draw Field
		for(int x= 0; x<nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
			{
				screen[(y + 2) * nScreenWidth + (x +2)] = L" ABCDEFG=&"[pField[y * nFieldWidth + x]];
				
				
			}
		// Draw Piece into buffer
		for (int px = 0; px < 4; px++)
			for (int py = 0; py < 4; py++)
			{
				if (tetromino[nCurrentPiece][nRotate(nCurrentRotation, px, py)] == L'#')
					screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;
			}
	
		swprintf(&screen[2 * nScreenWidth + 26], 16, L"Score: %d", nScore);
		// Display Buffer
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}	

	CloseHandle(hConsole);
	cout << "Game over!! Score" << nScore << endl;
	system("pause");

	return 0;
}
