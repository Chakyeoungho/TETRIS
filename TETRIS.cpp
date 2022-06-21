#include "pch.h"
#include "Constant.h"
#include <time.h>
#include "tipsware.h"

// sndPlaySound 함수를 사용하기 위해 필요한 헤더 파일과 라이브러리 추가!
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

typedef struct _GameData {
	const POINT tetrominoesData[7][4];
	const POINT wallKickData[2][4][5];
	POINT drawTet[4];
	POINT moveTet;
	WORD gameState;
	BYTE playfield[FIELD_Y_NUM + BUFFERZONE][FIELD_X_NUM + LINE_INFO];
	BYTE currTetromino;
	BYTE currSpinState : 2;    // 4개 플래그를 사용하기 편함
	bool tetLock;
	void *tetromino_image[8];
} GameData, *pGameData;

void setImage();                                    // 이미지 파일 설정
void setTetromino(pGameData p_data);                // 테트리미노 설정
bool isNotFloor(pGameData p_data);                  // 테트리미노가 바닥에 닿았는지 확인
bool isNotWall(pGameData p_data, int direction);    // 테트리미노가 벽에 닿았는지 확인
void cascade(pGameData p_data);                     // 캐스캐이드, 줄이 꽉차면 삭제
void setData(pGameData p_data);                     // 테트리스 데이터 설정
void removeData(pGameData p_data);                  // 테트리스 데이터 제거
void drawTetris(pGameData p_data);                  // 테트리스 그리기
void spin(pGameData p_data, BYTE spinDirection);    // 회전, SRS(Super Rotation System) 확인

// Lock Delay 타이머
TIMER LockDelay(NOT_USE_TIMER_DATA)
{
	pGameData ap_data = (pGameData)GetAppData();

	ap_data->tetLock = TRUE;

	removeData(ap_data);
	if (!isNotFloor(ap_data)) {
		setData(ap_data);
		setTetromino(ap_data);
		cascade(ap_data);
		drawTetris(ap_data);
	}
	setData(ap_data);

	if (ap_data->playfield[FIELD_Y_NUM - 1][FIELD_X_NUM] < 10) {
		ap_data->gameState = GAMEOVER;
		drawTetris(ap_data);
	}

	KillTimer(T_LOCKDELAY);
}

// 프레임 타이머
TIMER FrameProc(NOT_USE_TIMER_DATA)
{
	pGameData ap_data = (pGameData)GetAppData();

	if (ap_data->gameState == PLAYGAME) {
		removeData(ap_data);
		if (isNotFloor(ap_data))
			ap_data->moveTet.y++;
		setData(ap_data);
		drawTetris(ap_data);
		
		if (!isNotFloor(ap_data) && ap_data->tetLock) {
			ap_data->tetLock = FALSE;
			//SetTimer(T_LOCKDELAY, 500, LockDelay);
		}
	}
}

// 사용자가 메시지를 직접 처리할 때 사용하는 함수
int OnUserMsg(HWND ah_wnd, UINT a_message_id, WPARAM wParam, LPARAM lParam)
{
	pGameData p_data = (pGameData)GetAppData(); // 프로그램의 내부 데이터 주소를 가져온다.

	if (p_data != NULL && p_data->gameState == PLAYGAME) {
		if (a_message_id == WM_KEYDOWN) {  // 키보드의 버튼이 눌러졌을 때
		// 어떤 키를 눌렀는지에 대한 정보가 wParam 변수에 들어있다. VK는 Virtual Key의 약자이고
		// 방향키는 VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT로 정의되어 있다.
			switch (wParam) {
			case VK_UP:   // 위쪽 버튼
				removeData(p_data);
				spin(p_data, CLOCKWISE);
				setData(p_data);
				drawTetris(p_data);
				break;
			case VK_CONTROL:
				removeData(p_data);
				spin(p_data, COUNTERCLOCKWISE);
				setData(p_data);
				drawTetris(p_data);
				break;
			case VK_DOWN:   // 아래쪽 버튼
				removeData(p_data);
				if (isNotFloor(p_data))
					p_data->moveTet.y++;
				setData(p_data);
				drawTetris(p_data);

				if (!isNotFloor(p_data) && p_data->tetLock) {
					p_data->tetLock = FALSE;
					//SetTimer(T_LOCKDELAY, 500, LockDelay);
				}
				break;
			case VK_LEFT:   // 왼쪽 버튼
				removeData(p_data);
				if (isNotWall(p_data, LEFT))
					p_data->moveTet.x--;
				setData(p_data);
				drawTetris(p_data);
				break;
			case VK_RIGHT:   // 오른쪽 버튼
				removeData(p_data);
				if (isNotWall(p_data, RIGHT))
					p_data->moveTet.x++;
				setData(p_data);
				drawTetris(p_data);
				break;
			case VK_SPACE:    // 스페이스바
				removeData(p_data);
				while (isNotFloor(p_data)) {
					p_data->moveTet.y++;
				}
				setData(p_data);
				cascade(p_data);
				setTetromino(p_data);
				drawTetris(p_data);

				if (p_data->playfield[FIELD_Y_NUM - 1][FIELD_X_NUM] < 10) {
					p_data->gameState = GAMEOVER;
					drawTetris(p_data);
				}

				SetTimer(T_FRAME, 1000, FrameProc);
				break;
			default:
				break;
			}
		}
	}

	return 0;
}

ON_MESSAGE(NULL, NULL, NULL, NULL, NULL, OnUserMsg)

int main()
{
	waveOutSetVolume(0, (DWORD)0x25002500);    // 사운드 볼륨 조정 오른쪽 왼쪽
	sndPlaySound(".\\Sound\\Tetris_theme.wav", SND_ASYNC | SND_LOOP);    // 음악 재생

	ChangeWorkSize(TETROMINO_SIZE * FIELD_X_NUM + 400, TETROMINO_SIZE * FIELD_Y_NUM);

	srand((unsigned int)time(NULL));

	SetTimer(T_FRAME, 1000, FrameProc);
					  // 테트로미노 모양 데이터
	GameData data = { { { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 } },      // I
						{ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },      // J
						{ { 2, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },      // L
						{ { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } },      // O
						{ { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } },      // S
						{ { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },      // T
						{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } } },    // Z
					  // Wall Kick 데이터
					  { { { { 0, 0 }, { -2, 0 }, { 1, 0 },   { -2, 1 },  { 1, -2 }  },		  //         I         0>>1
						  { { 0, 0 }, { -1, 0 }, { 2, 0 },   { -1, -2 }, { 2, 1 }   }, 	      //         I         1>>2
						  { { 0, 0 }, { 2, 0 },  { -1, 0 },  { 2, -1 },  { -1, 2 }  },		  //         I         2>>3
						  { { 0, 0 }, { 1, 0 },  { -2, 0 },  { 1, 2 },   { -2, -1 } } },      //         I         3>>0
						{ { { 0, 0 }, { -1, 0 }, { -1, -1 }, { 0, 2 },   { -1, 2 }  },        // J, L, O, S, T, Z  0>>1
						  { { 0, 0 }, { 1, 0 },  { 1, 1 },   { 0, -2 },  { 1, -2 }  },        // J, L, O, S, T, Z  1>>2
						  { { 0, 0 }, { 1, 0 },  { 1, -1 },  { 0, 2 },   { 1, 2 }   },        // J, L, O, S, T, Z  2>>3
						  { { 0, 0 }, { -1, 0 }, { -1, 1 },  { 0, -2 },  { -1, -2 } } } },    // J, L, O, S, T, Z  3>>0
					  { { 0, }, }, { 3, BUFFERZONE }, PLAYGAME, { { 0, }, }, 0, 0, TRUE, { 0, } };
	SetAppData(&data, sizeof(GameData));

	pGameData ap_data = (pGameData)GetAppData();
	setImage();
	memset(ap_data->playfield, M_Tet, sizeof(BYTE) * (FIELD_Y_NUM + BUFFERZONE) * (FIELD_X_NUM + LINE_INFO));
	for (int y = 0; y < FIELD_Y_NUM + BUFFERZONE; y++) {
		ap_data->playfield[y][FIELD_X_NUM] = 10;
	}
	setTetromino(ap_data);
	drawTetris(ap_data);

	ShowDisplay();
	return 0;
}

// 이미지 파일 설정
void setImage()
{
	pGameData ap_data = (pGameData)GetAppData();

	ap_data->tetromino_image[MITet] = LoadImageGP(".\\Tetromino\\MITet.png");    // I
	ap_data->tetromino_image[MJTet] = LoadImageGP(".\\Tetromino\\MJTet.png");    // J
	ap_data->tetromino_image[MLTet] = LoadImageGP(".\\Tetromino\\MLTet.png");    // L
	ap_data->tetromino_image[MOTet] = LoadImageGP(".\\Tetromino\\MOTet.png");    // O
	ap_data->tetromino_image[MSTet] = LoadImageGP(".\\Tetromino\\MSTet.png");    // S
	ap_data->tetromino_image[MTTet] = LoadImageGP(".\\Tetromino\\MTTet.png");    // T
	ap_data->tetromino_image[MZTet] = LoadImageGP(".\\Tetromino\\MZTet.png");    // Z
	ap_data->tetromino_image[M_Tet] = LoadImageGP(".\\Tetromino\\M_Tet.png");    // _
}

// 테트리미노 설정
void setTetromino(pGameData p_data)
{
	p_data->currTetromino = (BYTE)(rand() % 7);
	memcpy(p_data->drawTet, p_data->tetrominoesData[p_data->currTetromino], sizeof(POINT) * 4);
	p_data->moveTet = { 3, BUFFERZONE };
	p_data->currSpinState = 0;

	bool isOk = false;
	while (!isOk) {
		isOk = true;

		for (int i = 0; i < 4; i++) {
			if (p_data->playfield[p_data->drawTet[i].y + p_data->moveTet.y][p_data->drawTet[i].x + p_data->moveTet.x] != M_Tet) {
				p_data->moveTet.y--;
				isOk = false;
				break;
			}
		}
	}

	setData(p_data);
}

// 테트리미노가 바닥에 닿았는지 확인
bool isNotFloor(pGameData p_data)
{
	BYTE tempArr[4] = { 0, };

	for (int i = 0; i < 4; i++) {
		if (p_data->drawTet[i].y + p_data->moveTet.y >= FIELD_Y_NUM + BUFFERZONE - 1 ||
			p_data->playfield[p_data->drawTet[i].y + p_data->moveTet.y + 1][p_data->drawTet[i].x + p_data->moveTet.x] != M_Tet) {
			return false;
		}
	}

	return true;
}

// 테트리미노가 벽에 닿았는지 확인
bool isNotWall(pGameData p_data, int direction)
{
	BYTE tempArr[4] = { 0, };

	if (direction == LEFT) {
		memset(tempArr, UCHAR_MAX, sizeof(BYTE) * 4);
		for (int i = 0; i < 4; i++) {
			if (p_data->drawTet[i].x + p_data->moveTet.x < 1 || 
				p_data->playfield[p_data->drawTet[i].y + p_data->moveTet.y][p_data->drawTet[i].x + p_data->moveTet.x - 1] != M_Tet)
				return false;
		}
	} else if (direction == RIGHT) {
		for (int i = 0; i < 4; i++) {
			if (p_data->drawTet[i].x + p_data->moveTet.x >= FIELD_X_NUM - 1 || 
				p_data->playfield[p_data->drawTet[i].y + p_data->moveTet.y][p_data->drawTet[i].x + p_data->moveTet.x + 1] != M_Tet)
				return false;
		}
	}

	return true;
}

// 캐스캐이드, 줄이 꽉차면 삭제
void cascade(pGameData p_data)
{
	int i;

	for (int y = 4; y < FIELD_Y_NUM + BUFFERZONE; y++) {
		if (p_data->playfield[y][FIELD_X_NUM] == 0) {
			i = y;
			while (p_data->playfield[i][FIELD_X_NUM] != 10) {
				memcpy(p_data->playfield[i], p_data->playfield[i - 1], sizeof(BYTE) * (FIELD_X_NUM + LINE_INFO));
				i--;
			}
		}
	}
}

// 테트리스 데이터 설정
void setData(pGameData p_data)
{
	for (int i = 0; i < 4; i++) {
		p_data->playfield[p_data->drawTet[i].y + p_data->moveTet.y][p_data->drawTet[i].x + p_data->moveTet.x] = p_data->currTetromino;

		p_data->playfield[p_data->drawTet[i].y + p_data->moveTet.y][FIELD_X_NUM]--;
	}
}

// 테트리스 데이터 제거
void removeData(pGameData p_data)
{
	for (int i = 0; i < 4; i++) {
		p_data->playfield[p_data->drawTet[i].y + p_data->moveTet.y][p_data->drawTet[i].x + p_data->moveTet.x] = M_Tet;

		p_data->playfield[p_data->drawTet[i].y + p_data->moveTet.y][FIELD_X_NUM]++;
	}
}

// 테트리스 그리기
void drawTetris(pGameData p_data)
{
	Clear();

	for (int y = BUFFERZONE; y < FIELD_Y_NUM + BUFFERZONE; y++) {
		for (int x = 0; x < FIELD_X_NUM; x++) {
			TextOut(x * 12 + 450, (y - BUFFERZONE) * 12 + 50, "%d", p_data->playfield[y][x]);
		}
		TextOut(130 + 470, (y - BUFFERZONE) * 12 + 50, "%d", p_data->playfield[y][FIELD_X_NUM]);
	}
	
	TextOut(500, 500, "%d", p_data->currSpinState);
	TextOut(490, 510, "%d", p_data->gameState);

	for (int y = BUFFERZONE; y < FIELD_Y_NUM + BUFFERZONE; y++) {
		for (int x = 0; x < FIELD_X_NUM; x++) {
			DrawImageGP(p_data->tetromino_image[p_data->playfield[y][x]], x * TETROMINO_SIZE, (y - BUFFERZONE) * TETROMINO_SIZE, TETROMINO_SIZE, TETROMINO_SIZE);
		}
	}

	ShowDisplay();
}

// 회전, SRS(Super Rotation System) 확인
void spin(pGameData p_data, BYTE spinDirection)
{
	bool srs;
	POINT tempSpin[4];

	if (spinDirection == CLOCKWISE) {
		if (p_data->currTetromino == MITet) {
			for (int i = 0; i < 4; i++) {
				tempSpin[i].x = 3 - p_data->drawTet[i].y;
				tempSpin[i].y = p_data->drawTet[i].x;
			}

			for (int test = 0; test < 5; test++) {
				srs = TRUE;

				for (int i = 0; i < 4; i++)
					if (tempSpin[i].y + p_data->moveTet.y + p_data->wallKickData[0][p_data->currSpinState][test].y >= FIELD_Y_NUM + BUFFERZONE || 
						tempSpin[i].x + p_data->moveTet.x + p_data->wallKickData[0][p_data->currSpinState][test].x >= FIELD_X_NUM || 
						tempSpin[i].x + p_data->moveTet.x + p_data->wallKickData[0][p_data->currSpinState][test].x < 0 || 
						p_data->playfield[tempSpin[i].y + p_data->moveTet.y + p_data->wallKickData[0][p_data->currSpinState][test].y][tempSpin[i].x + p_data->moveTet.x + p_data->wallKickData[0][p_data->currSpinState][test].x] != M_Tet) {
						srs = FALSE;
						break;
					}

				if (srs) {
					memcpy(p_data->drawTet, tempSpin, sizeof(POINT) * 4);
					p_data->moveTet.x += p_data->wallKickData[0][p_data->currSpinState][test].x;
					p_data->moveTet.y += p_data->wallKickData[0][p_data->currSpinState][test].y;
					p_data->currSpinState++;
					break;
				}
			}
		} else if (p_data->currTetromino != MOTet) {
			for (int i = 0; i < 4; i++) {
				tempSpin[i].x = 2 - p_data->drawTet[i].y;
				tempSpin[i].y = p_data->drawTet[i].x;
			}

			for (int test = 0; test < 5; test++) {
				srs = TRUE;

				for (int i = 0; i < 4; i++)
					if (tempSpin[i].y + p_data->moveTet.y + p_data->wallKickData[1][p_data->currSpinState][test].y >= FIELD_Y_NUM + BUFFERZONE ||
						tempSpin[i].x + p_data->moveTet.x + p_data->wallKickData[1][p_data->currSpinState][test].x >= FIELD_X_NUM ||
						tempSpin[i].x + p_data->moveTet.x + p_data->wallKickData[1][p_data->currSpinState][test].x < 0 ||
						p_data->playfield[tempSpin[i].y + p_data->moveTet.y + p_data->wallKickData[1][p_data->currSpinState][test].y][tempSpin[i].x + p_data->moveTet.x + p_data->wallKickData[1][p_data->currSpinState][test].x] != M_Tet) {
						srs = FALSE;
						break;
					}

				if (srs) {
					memcpy(p_data->drawTet, tempSpin, sizeof(POINT) * 4);
					p_data->moveTet.x += p_data->wallKickData[1][p_data->currSpinState][test].x;
					p_data->moveTet.y += p_data->wallKickData[1][p_data->currSpinState][test].y;
					p_data->currSpinState++;
					break;
				}
			}
		}
	}
}