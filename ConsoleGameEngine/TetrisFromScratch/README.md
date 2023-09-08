Let's Make Tetris
<details>
<summary>Introduce to the game</summary>
  
### Tetromino:
  - Các khối xếp hình được lưu dưới dạng `c wstring`( L"..#..##...#....." ) với kích thước 4x4
  - Có tất cả 7 khối, ta cần một mảng kích thước 7.
  - Việc sử dụng mảng cho phép ta thuận tiện trong việc truy cập và thay đổi giá trị sử dụng các thuật toán.
  VD: Để quay khối hình các góc: 
  ```c
  0) i= y*w+x 
  90) i = 12 +y- x*4
  180) i =15- y*4 -x
  270) i =3+ y + x*4
  ```
### Playing field:
  - Thay vì in ra từng dòng sử dụng cout hoặc printf cho hiệu suất rất thấp, ta cần tạo một mảng để lưu các kí tự ở từng điểm ảnh(buffer) rồi sau đó in một lần ra màn hình luôn.
  - Tạo playing field kích thước 16x16 để lưu trữ các trạng thái của game `c unsigned char* pField = nullptr;`
  - Sử dụng thư viện "Windows.h" cho phép sử dụng terminal như một màn hình hiển thị, ta sẽ tạo mảng kích thước 30x80 để lưu các giá trị điểm ảnh của cả màn hình.
  ```c
wchar_t* screen= new wchar_t(nScreenWidth * nScreenHeight);
for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
	screen[i] = L' ';
HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
SetConsoleActiveScreenBuffer(hConsole);
DWORD dwBytesWritten = 0;
  ```
  - Để hiển thị buffer ra màn hình console, chúng ta cần gắn nó với bộ xử lí và thiết lập nó như một màn hình hoạt động( fuck it, i can't translate it well) (grab it a handle to the console buffer and set it as a active screen.
```c
    // Display Buffer
WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
```
- Để đóng màn hình hiển thị lại và kết thúc chương trình ta chỉ việc gọi lệnh closeHandle
```c
	CloseHandle(hConsole);
```
</details>


<details>
<summary>Game states</summary>
- Game states là các thuộc tính khởi tạo cho game khi bắt đầu chơi, nó chỉ khai báo một lần trước khi vào vòng lặp game và có thể bị thay đổi sau mỗi vòng lặp của game. VD: 
  ```c
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
  ```
</details>

<details>
<summary> Game Loop</summary>

  ### Game loop
- Vòng lặp game để giúp cho trò chơi tiếp tục mãi đến khi ng chơi muốn kết thúc. Sau mỗi vòng lập, chúng ta cần cập nhật những thứ sau:
  - Game timing
  - User input
  - Game logic
  - Render output...
  #### 1. Game timing
  - Sau mỗi vòng lặp cần 1 thời gian gọi là clock cycle, thời gian này khá ngắn. Nhưng sử dụng nó sẽ giúp cho game được mượt mà hơn.
  - Ở game nay, ta sẽ cho delay thêm 50ms sau mỗi vòng lặp:
    ```c
    // Game Timing
    this_thread::sleep_for(100ms);
    nTimeCount++;
    bool bForceDown = (nTimeCount % 10 == 0);
    ```

  #### 2. User input
  - Ta sẽ tạo một mảng boolean bKey[4] để check xem liệu có phím nào trong 4 phím được bấm hay không(RIGHT/LEFT/DOWN/Z) bằng việc sử dụng toán tử OR(&) với địa chỉ 0x8000
    ```c
    for (int k = 0; k < 4; k++)
	bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0; // R L D Z
    ```
  #### 3. Game logic
  - Trước hết, ta cần tạo 2 function utility là bDosePieceFit để kiểm tra liệu khối hình có thế di chuyển tới vị trí mới hay không và nRotate để xoay khối hinh.
  - Nếu các điểu kiện thỏa mãn, khối hình được phép di chuyển.
  - Rotate hold: Bởi vì thời gian của mỗi vòng lặp rất ngắn, vì vậy khi nhấn rotate có 1 lần nhưng máy tính có thể hiểu là nhiểu lần. Ta sử dụng thêm 1 flag là bRotateHold
    ```c
    if (bKey[3])
    {
    	nCurrentRotation += (bRotateHold && bDoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
    	bRotateHold = false;   // Prevent duplication of rotate before of clock cycle is ~ 50ms. 
    }
    else {
    	bRotateHold = true;
    }
    ```
  - Force the piece down: Khi thời gian nhất định trôi qua, khối hình sẽ tự động di chuyển không cần nhận tín hiệu từ người chơi.
  - Nếu nó thóa mãn điều kiện thì nó sẽ di chuyển xuống dưới, nếu không thì chúng ta sẽ:
    - Khóa nó lại ở vị trí hiện tại.
    - Kiểm tra xem có ghi điểm không(line check):
      Kiểm tra 4 dòng của mà khối hình đang nằm trong, xem có hàng nào đã được lấp đầy không. Nếu có thì thêm giá trị của hàng đó(tung độ) vào vector vecLines. 
    - Chọn khối mới
    - Hoặc là kết thúc game.

  - Ghi điểm: Khi vector vecLines không trống tức là có hàng đã được lấp đầy, ta cần xóa nó đi và cập nhật từ hàng đó trở lên trên xuống dưới một hàng và xóa hàng trên cùng đi.
    ```c
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
    ```
  #### 4. Game render
  - Để render game state ra màn hình, ta cần copy các giá trị điểm ảnh vào screenBuffer rồi gắn nó với handler
  - Vẽ sân chơi: Ta chỉ việc copy từng giá trị điểm ảnh của playing field vào screen field
  - Vẽ khối hình: Cũng thế.
  - Hiển thị điểm số: In đè lên console buffer
    ```c
    swprintf(&screen[2 * nScreenWidth + 26], 16, L"Score: %d", nScore);
    ```
</details>





   
  
  


