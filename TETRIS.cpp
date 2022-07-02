﻿#include "pch.h"
#include "Constant.h"
#include <time.h>
#include "WELL1024a.h"	// WELL Random number generator  http://www.iro.umontreal.ca/~panneton/WELLRNG.html
#include "tipsware.h"

// sndPlaySound 함수를 사용하기 위해 필요한 헤더 파일과 라이브러리 추가!
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// 게임 데이터
typedef struct _GameData {
	BYTE playfield[FIELD_Y_NUM + BUFFERZONE][FIELD_X_NUM + LINE_INFO];    // 플레이 필드
	BYTE currTetromino;           // 현재 테트로미노
	BYTE tetPocket[2][7];         // 테트로미노 포켓
	BYTE showTet[6];              // 미리보기
	BYTE tetHold;                 // 홀드된 테트로미노
	BYTE selectTet;               // 현재 테트로미노 선택
	POINT drawTet[4];             // 그릴 테트로미노
	POINT moveTet;                // 테트로미노 이동 좌표
	WORD gameState;               // 현재 게임 상태
	WORD tetLockTime;             // 테트로미노 잠금 시간
	BYTE currSpinState : 2;       // 현재 회전 상태 / 4개 플래그를 사용하기 편함
	BYTE chance;                  // 땅에 닿고 나서 움직이면 시간 초기화 되는 횟수
	bool tetLock;                 // 테트로미노 잠금 타이머가 실행 됐는지
	bool isHold;                  // 홀드를 한번 썼는지
	void *tetromino_image[15];    // 테트로미노 이미지
} GameData, *pGameData;

// 함수 선언
TIMER LockDelayProc(NOT_USE_TIMER_DATA);              // Lock Delay 타이머
TIMER FrameProc(NOT_USE_TIMER_DATA);                  // 프레임 타이머

void setImage();                                      // 이미지 파일 설정
void setTetromino(pGameData p_data);                  // 테트리미노 설정
bool isNotFloor(pGameData p_data);                    // 테트리미노가 바닥에 닿았는지 확인
bool isNotWall(pGameData p_data, WPARAM direction);   // 테트리미노가 벽에 닿았는지 확인
void cascade(pGameData p_data);                       // 캐스캐이드, 줄이 꽉차면 삭제
void setData(pGameData p_data);                       // 테트리스 데이터 설정
void removeData(pGameData p_data);                    // 테트리스 데이터 제거
void drawTetris(pGameData p_data);                    // 테트리스 그리기
void spin(pGameData p_data, WPARAM spinDirection);    // 회전, SRS(Super Rotation System) 확인
void moveDown(pGameData p_data);                      // 바닥에 닿았는지 검사 후 아래로 이동

// Lock Delay 타이머
TIMER LockDelayProc(NOT_USE_TIMER_DATA)
{
	pGameData ap_data = (pGameData)GetAppData();

	if (!isNotFloor(ap_data)) {
		ap_data->tetLockTime++;
		Rectangle(460, 450, 550, 466, RGB(255, 255, 255), RGB(255, 255, 255));
		TextOut(500, 450, "%d", ap_data->tetLockTime);
		ShowDisplay();

		if (ap_data->tetLockTime >= 50) {
			cascade(ap_data);
			setTetromino(ap_data);
			drawTetris(ap_data);
			ap_data->chance = 0;
			ap_data->isHold = false;

			SetTimer(T_FRAME, 1000, FrameProc);
			KillTimer(T_LOCKDELAY);
		}
	} else {
		SetTimer(T_FRAME, 1000, FrameProc);
		KillTimer(T_LOCKDELAY);
	}


	if (ap_data->playfield[FIELD_Y_NUM - 1][FIELD_X_NUM] < 10) {
		ap_data->gameState = GAMEOVER;
		drawTetris(ap_data);
	}
}

// 프레임 타이머
TIMER FrameProc(NOT_USE_TIMER_DATA)
{
	pGameData ap_data = (pGameData)GetAppData();

	if (ap_data->gameState == PLAYGAME)
		moveDown(ap_data);
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
			case VK_UP: case VK_CONTROL:   // 회전
				if (p_data->currTetromino != MOTet) {
					spin(p_data, wParam);

					drawTetris(p_data);
				}

				if (!isNotFloor(p_data)) {
					SetTimer(T_LOCKDELAY, 10, LockDelayProc);
					KillTimer(T_FRAME);
					
					if (p_data->chance < 4) {
						p_data->chance++;
						p_data->tetLockTime = 0;
					}
				}
				break;
			case VK_DOWN:   // 아래쪽 버튼
				moveDown(p_data);
				break;
			case VK_LEFT: case VK_RIGHT:   // 왼쪽, 오른쪽 버튼
				if (isNotWall(p_data, wParam)) {
					removeData(p_data);
					p_data->moveTet.x += wParam - 0x26;
					setData(p_data);
					drawTetris(p_data);

					if (!isNotFloor(p_data) && p_data->chance < 4) {
						p_data->chance++;
						p_data->tetLockTime = 0;
					}
				}
				break;
			case VK_SPACE:    // 스페이스바
				while (isNotFloor(p_data)) {
					removeData(p_data);
					p_data->moveTet.y++;
					setData(p_data);
				}
				cascade(p_data);
				setTetromino(p_data);
				drawTetris(p_data);
				p_data->isHold = false;

				// TODO fix
				if (p_data->playfield[FIELD_Y_NUM - 1][FIELD_X_NUM] < 10) {
					p_data->gameState = GAMEOVER;
					drawTetris(p_data);
				}

				SetTimer(T_FRAME, 1000, FrameProc);
				break;
			// TODO fix
			case 'Z':    // 홀드
				if (p_data->tetHold == M_Tet) {
					p_data->isHold = true;
					removeData(p_data);
					p_data->tetHold = p_data->currTetromino;
					setTetromino(p_data);
					drawTetris(p_data);
				} else if (!p_data->isHold) {
					p_data->isHold = true;
					removeData(p_data);
					SWAPTET(p_data->tetHold, p_data->currTetromino);
					memcpy(p_data->drawTet, tetrominoesData[p_data->currTetromino], sizeof(POINT) * 4);
					p_data->moveTet = { 3, BUFFERZONE };
					p_data->currSpinState = 0;
					setData(p_data);
					drawTetris(p_data);
				}
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
	ChangeWorkSize(TETROMINO_SIZE * FIELD_X_NUM + 400, TETROMINO_SIZE * FIELD_Y_NUM);    // 윈도우 크기 조절

	waveOutSetVolume(0, (DWORD)0x25002500);    // 사운드 볼륨 조정 오른쪽 왼쪽
	sndPlaySound(".\\Sound\\Tetris_theme.wav", SND_ASYNC | SND_LOOP);    // 음악 재생

	// 난수 생성
	srand((unsigned)time(NULL));
	unsigned int init[32];
	for (int i = 0; i < 32; i++) {
		init[i] = rand() << 16 | rand();
		// WELL Random 을 초기화 하기 위해, C 표준 rand() 함수를 이용하여 init 값을 생성합니다
	}
	InitWELLRNG1024a(init); // WELL Random 초기화

	GameData data = { { { 0, }, }, M_Tet, { { 0, }, }, { 0, }, M_Tet, 0, { { 0, }, }, { 3, BUFFERZONE }, PLAYGAME, 0, 0, 0, true, false, { 0, } };
	SetAppData(&data, sizeof(GameData));

	pGameData ap_data = (pGameData)GetAppData();
	setImage();    // 이미지 설정
	memset(ap_data->playfield, M_Tet, sizeof(BYTE) * (FIELD_Y_NUM + BUFFERZONE) * (FIELD_X_NUM + LINE_INFO));    // 플레이 필드 M_Tet로 초기화
	// x좌표 끝줄은 줄이 꽉 찼는지 확인하는 용도로 10으로 초기화
	for (int y = 0; y < FIELD_Y_NUM + BUFFERZONE; y++) {
		ap_data->playfield[y][FIELD_X_NUM] = 10;
	}
	// 테트로미노 포켓 설정
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 7; j++) {
			ap_data->tetPocket[i][j] = (BYTE)((double)WELLRNG1024a() * 7);

			for (int k = 0; k < j; k++) {
				if (ap_data->tetPocket[i][j] == ap_data->tetPocket[i][k]) {
					j--;
					break;
				}
			}
		}
	}
	setTetromino(ap_data);    // 테트로미노 설정
	drawTetris(ap_data);      // 테트리스 그리기

	SetTimer(T_FRAME, 1000, FrameProc);

	ShowDisplay();
	return 0;
}

// 이미지 파일 설정
void setImage()
{
	pGameData ap_data = (pGameData)GetAppData();

	// Tetromino
	ap_data->tetromino_image[MITet] = LoadImageGP(".\\Tetromino\\MITet.png");    // I
	ap_data->tetromino_image[MJTet] = LoadImageGP(".\\Tetromino\\MJTet.png");    // J
	ap_data->tetromino_image[MLTet] = LoadImageGP(".\\Tetromino\\MLTet.png");    // L
	ap_data->tetromino_image[MOTet] = LoadImageGP(".\\Tetromino\\MOTet.png");    // O
	ap_data->tetromino_image[MSTet] = LoadImageGP(".\\Tetromino\\MSTet.png");    // S
	ap_data->tetromino_image[MTTet] = LoadImageGP(".\\Tetromino\\MTTet.png");    // T
	ap_data->tetromino_image[MZTet] = LoadImageGP(".\\Tetromino\\MZTet.png");    // Z
	ap_data->tetromino_image[M_Tet] = LoadImageGP(".\\Tetromino\\M_Tet.png");    // _

	// Ghost
	ap_data->tetromino_image[GITet] = LoadImageGP(".\\Tetromino\\GITet.png");    // I
	ap_data->tetromino_image[GJTet] = LoadImageGP(".\\Tetromino\\GJTet.png");    // J
	ap_data->tetromino_image[GLTet] = LoadImageGP(".\\Tetromino\\GLTet.png");    // L
	ap_data->tetromino_image[GOTet] = LoadImageGP(".\\Tetromino\\GOTet.png");    // O
	ap_data->tetromino_image[GSTet] = LoadImageGP(".\\Tetromino\\GSTet.png");    // S
	ap_data->tetromino_image[GTTet] = LoadImageGP(".\\Tetromino\\GTTet.png");    // T
	ap_data->tetromino_image[GZTet] = LoadImageGP(".\\Tetromino\\GZTet.png");    // Z
}

// 테트리미노 설정
void setTetromino(pGameData p_data)
{
	if (p_data->selectTet == 7) {
		p_data->selectTet = 0;

		// WELLRNG1024a() 기본 사용법
		// double x = (double)WELLRNG1024a(); // 0.0 <= x < 1.0  실수 랜덤 생성
		memcpy(p_data->tetPocket[0], p_data->tetPocket[1], sizeof(BYTE) * 7);

		for (int i = 0; i < 7; i++) {
			p_data->tetPocket[1][i] = (BYTE)((double)WELLRNG1024a() * 7);

			for (int j = 0; j < i; j++) {
				if (p_data->tetPocket[1][i] == p_data->tetPocket[1][j]) {
					i--;
					break;
				}
			}
		}
	}

	BYTE temp = p_data->selectTet + 1;
	for (int i = 0; i < 6; i++)
		p_data->showTet[i] = *(p_data->tetPocket[0] + temp++);
	p_data->currTetromino = p_data->tetPocket[0][p_data->selectTet++];
	memcpy(p_data->drawTet, tetrominoesData[p_data->currTetromino], sizeof(POINT) * 4);
	p_data->moveTet = { 3, BUFFERZONE };
	p_data->currSpinState = 0;

	bool isOk = true;
	while (isOk) {
		isOk = false;

		for (int i = 0; i < 4; i++) {
			if (p_data->playfield[p_data->drawTet[i].y + p_data->moveTet.y][p_data->drawTet[i].x + p_data->moveTet.x] != M_Tet) {
				p_data->moveTet.y--;
				isOk = true;
				break;
			}
		}
	}

	p_data->tetLock = true;
	setData(p_data);
	SetTimer(T_FRAME, 1000, FrameProc);
}

// 테트리미노가 바닥에 닿았는지 확인
bool isNotFloor(pGameData p_data)
{
	removeData(p_data);

	for (int i = 0; i < 4; i++) {
		if (p_data->drawTet[i].y + p_data->moveTet.y >= FIELD_Y_NUM + BUFFERZONE - 1 ||
			p_data->playfield[p_data->drawTet[i].y + p_data->moveTet.y + 1][p_data->drawTet[i].x + p_data->moveTet.x] != M_Tet) {
			setData(p_data);
			return false;
		}
	}

	setData(p_data);

	return true;
}

// 테트리미노가 벽에 닿았는지 확인
bool isNotWall(pGameData p_data, WPARAM direction)
{
	removeData(p_data);

	if (direction == VK_LEFT) {
		for (int i = 0; i < 4; i++) {
			if (p_data->drawTet[i].x + p_data->moveTet.x < 1 ||
				p_data->playfield[p_data->drawTet[i].y + p_data->moveTet.y][p_data->drawTet[i].x + p_data->moveTet.x - 1] != M_Tet) {
				setData(p_data);
				return false;
			}
		}
	} else if (direction == VK_RIGHT) {
		for (int i = 0; i < 4; i++) {
			if (p_data->drawTet[i].x + p_data->moveTet.x >= FIELD_X_NUM - 1 ||
				p_data->playfield[p_data->drawTet[i].y + p_data->moveTet.y][p_data->drawTet[i].x + p_data->moveTet.x + 1] != M_Tet) {
				setData(p_data);
				return false;
			}
		}
	}

	setData(p_data);

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

	Rectangle(460, 450, 550, 466, RGB(255, 255, 255), RGB(255, 255, 255));
	TextOut(500, 450, "%d", p_data->tetLockTime);

	TextOut(500, 500, "%d", p_data->currSpinState);
	TextOut(490, 510, "%d", p_data->gameState);

	for (int y = BUFFERZONE; y < FIELD_Y_NUM + BUFFERZONE; y++) {
		for (int x = 0; x < FIELD_X_NUM; x++) {
			DrawImageGP(p_data->tetromino_image[p_data->playfield[y][x]], x * TETROMINO_SIZE, (y - BUFFERZONE) * TETROMINO_SIZE, TETROMINO_SIZE, TETROMINO_SIZE);
		}
	}

	// 고스트
	LONG ghostY = p_data->moveTet.y;
	bool isFloor = true;
	removeData(p_data);
	while (isFloor) {
		for (int i = 0; i < 4; i++) {
			if (p_data->drawTet[i].y + ghostY >= FIELD_Y_NUM + BUFFERZONE - 1 ||
				p_data->playfield[p_data->drawTet[i].y + ghostY + 1][p_data->drawTet[i].x + p_data->moveTet.x] != M_Tet) {
				isFloor = false;
				ghostY--;
				break;
			}
		}
		ghostY++;
	}
	setData(p_data);
	for (int i = 0; i < 4; i++) {
		DrawImageGP(p_data->tetromino_image[p_data->currTetromino + 8], (p_data->drawTet[i].x + p_data->moveTet.x) * TETROMINO_SIZE, (p_data->drawTet[i].y + ghostY - BUFFERZONE) * TETROMINO_SIZE, TETROMINO_SIZE, TETROMINO_SIZE);
	}

	// 현재 테트로미노 그리기
	for (int i = 0; i < 4; i++) {
		DrawImageGP(p_data->tetromino_image[p_data->currTetromino], (p_data->drawTet[i].x + p_data->moveTet.x) * TETROMINO_SIZE, (p_data->drawTet[i].y + p_data->moveTet.y - BUFFERZONE) * TETROMINO_SIZE, TETROMINO_SIZE, TETROMINO_SIZE);
	}

	// 테트로미노 미리 그리기
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			DrawImageGP(p_data->tetromino_image[p_data->showTet[i]], 350 + (tetrominoesData[p_data->showTet[i]][j].x * 10), 200 + (tetrominoesData[p_data->showTet[i]][j].y * 10) + (i * 30), 10, 10);
		}
	}

	// 홀드된 테트로미노 그리기
	for (int j = 0; j < 4; j++) {
		DrawImageGP(p_data->tetromino_image[p_data->tetHold], 350 + (tetrominoesData[p_data->tetHold][j].x * 10), 100 + (tetrominoesData[p_data->tetHold][j].y * 10), 10, 10);
	}

	ShowDisplay();
}

// 회전, SRS(Super Rotation System) 확인
void spin(pGameData p_data, WPARAM spinDirection)
{
	removeData(p_data);

	bool srs, isITet = p_data->currTetromino;
	POINT tempSpin[4] = { 0, };

	if (spinDirection == VK_UP) {    // 시계방향 회전
		for (int i = 0; i < 4; i++) {
			tempSpin[i].x = (3 - isITet) - p_data->drawTet[i].y;
			tempSpin[i].y = p_data->drawTet[i].x;
		}

		for (int test = 0; test < 5; test++) {
			srs = true;

			for (int i = 0; i < 4; i++)
				if (tempSpin[i].y + p_data->moveTet.y + wallKickData[isITet][p_data->currSpinState][test].y >= FIELD_Y_NUM + BUFFERZONE ||
					tempSpin[i].x + p_data->moveTet.x + wallKickData[isITet][p_data->currSpinState][test].x >= FIELD_X_NUM ||
					tempSpin[i].x + p_data->moveTet.x + wallKickData[isITet][p_data->currSpinState][test].x < 0 ||
					p_data->playfield[tempSpin[i].y + p_data->moveTet.y + wallKickData[isITet][p_data->currSpinState][test].y][tempSpin[i].x + p_data->moveTet.x + wallKickData[isITet][p_data->currSpinState][test].x] != M_Tet) {
					srs = false;
					break;
				}

			if (srs) {
				memcpy(p_data->drawTet, tempSpin, sizeof(POINT) * 4);
				p_data->moveTet.x += wallKickData[isITet][p_data->currSpinState][test].x;
				p_data->moveTet.y += wallKickData[isITet][p_data->currSpinState][test].y;
				p_data->currSpinState++;
				break;
			}
		}
	} else if (spinDirection == VK_CONTROL) {    // 반시계방향 회전
		for (int i = 0; i < 4; i++) {
			tempSpin[i].x = p_data->drawTet[i].y;
			tempSpin[i].y = (3 - isITet) - p_data->drawTet[i].x;
		}

		for (int test = 0; test < 5; test++) {
			p_data->currSpinState--;
			srs = true;

			for (int i = 0; i < 4; i++)
				if (tempSpin[i].y + p_data->moveTet.y - wallKickData[isITet][p_data->currSpinState][test].y >= FIELD_Y_NUM + BUFFERZONE ||
					tempSpin[i].x + p_data->moveTet.x - wallKickData[isITet][p_data->currSpinState][test].x >= FIELD_X_NUM ||
					tempSpin[i].x + p_data->moveTet.x - wallKickData[isITet][p_data->currSpinState][test].x < 0 ||
					p_data->playfield[tempSpin[i].y + p_data->moveTet.y - wallKickData[isITet][p_data->currSpinState][test].y][tempSpin[i].x + p_data->moveTet.x - wallKickData[isITet][p_data->currSpinState][test].x] != M_Tet) {
					p_data->currSpinState++;
					srs = false;
					break;
				}

			if (srs) {
				memcpy(p_data->drawTet, tempSpin, sizeof(POINT) * 4);
				p_data->moveTet.x -= wallKickData[isITet][p_data->currSpinState][test].x;
				p_data->moveTet.y -= wallKickData[isITet][p_data->currSpinState][test].y;
				break;
			}
		}
	}

	setData(p_data);
}

// 바닥에 닿았는지 검사 후 아래로 이동
void moveDown(pGameData p_data) {
	if (isNotFloor(p_data)) {
		removeData(p_data);
		p_data->moveTet.y++;
		setData(p_data);
		drawTetris(p_data);

		p_data->tetLockTime = 0;
		p_data->tetLock = true;
		KillTimer(T_LOCKDELAY);
	}

	if (!isNotFloor(p_data) && p_data->tetLock) {
		p_data->tetLock = false;
		SetTimer(T_LOCKDELAY, 10, LockDelayProc);
		KillTimer(T_FRAME);
	}
}