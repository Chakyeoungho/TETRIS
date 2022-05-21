#include "pch.h"
#include "Constant.h"
#include <time.h>
#include "tipsware.h"

// sndPlaySound 함수를 사용하기 위해 필요한 헤더 파일과 라이브러리 추가!
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

typedef struct _GameData {
	POINT Tetrominoes[7][4];
	POINT drawTet[4];
	POINT move;
	BYTE playfield[24][10];
	BYTE currTetromino;
	void *tetromino_image[7];
	void *background_image;
} GameData, *pGameData;

void setImage();
void setTetromino(pGameData p_data);
bool check(pGameData p_data);
void move(pGameData p_data);
void drawTetris(pGameData p_data);
void spin(pGameData p_data);

TIMER FrameProc(NOT_USE_TIMER_DATA)
{
	pGameData ap_data = (pGameData)GetAppData();

	if (ap_data->drawTet[0].y + ap_data->move.y < TETROMINOSIZE * 19 &&
		ap_data->drawTet[1].y + ap_data->move.y < TETROMINOSIZE * 19 &&
		ap_data->drawTet[2].y + ap_data->move.y < TETROMINOSIZE * 19 &&
		ap_data->drawTet[3].y + ap_data->move.y < TETROMINOSIZE * 19)
		ap_data->move.y += TETROMINOSIZE;
	drawTetris(ap_data);
}

// 사용자가 메시지를 직접 처리할 때 사용하는 함수
int OnUserMsg(HWND ah_wnd, UINT a_message_id, WPARAM wParam, LPARAM lParam)
{
	pGameData p_data = (pGameData)GetAppData(); // 프로그램의 내부 데이터 주소를 가져온다.

	if (a_message_id == WM_KEYDOWN) {  // 키보드의 버튼이 눌러졌을 때
		bool move_flag = true;  // 방향 키가 눌러져야지만 화면을 갱신하게 하려고 1로 설정한다.

		// 어떤 키를 눌렀는지에 대한 정보가 wParam 변수에 들어있다. VK는 Virtual Key의 약자이고
		// 방향키는 VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT로 정의되어 있다.
		switch (wParam)
		{
		case VK_UP:
			spin(p_data);
			break;
		case VK_DOWN:
			if (p_data->drawTet[0].y + p_data->move.y < TETROMINOSIZE * 19 &&
				p_data->drawTet[1].y + p_data->move.y < TETROMINOSIZE * 19 &&
				p_data->drawTet[2].y + p_data->move.y < TETROMINOSIZE * 19 &&
				p_data->drawTet[3].y + p_data->move.y < TETROMINOSIZE * 19)
				p_data->move.y += TETROMINOSIZE;   // 아래쪽 버튼
			break;
		case VK_LEFT:
			if (p_data->drawTet[0].x + p_data->move.x >= TETROMINOSIZE &&
				p_data->drawTet[1].x + p_data->move.x >= TETROMINOSIZE &&
				p_data->drawTet[2].x + p_data->move.x >= TETROMINOSIZE &&
				p_data->drawTet[3].x + p_data->move.x >= TETROMINOSIZE)
				p_data->move.x -= TETROMINOSIZE;
			break;
		case VK_RIGHT:
			if (p_data->drawTet[0].x + p_data->move.x < TETROMINOSIZE * 9 &&
				p_data->drawTet[1].x + p_data->move.x < TETROMINOSIZE * 9 &&
				p_data->drawTet[2].x + p_data->move.x < TETROMINOSIZE * 9 &&
				p_data->drawTet[3].x + p_data->move.x < TETROMINOSIZE * 9)
				p_data->move.x += TETROMINOSIZE;
			break;
		default:
			move_flag = false;   // 방향 키가 아닌 다른 키를 눌렀다면 화면을 갱신할 필요가 없다.
			break;
		}

		if (move_flag) {  // 사각형이 움직인 경우!
			drawTetris(p_data);
		}
	}
	return 0;
}

ON_MESSAGE(NULL, NULL, NULL, NULL, NULL, OnUserMsg)

int main()
{
	waveOutSetVolume(0, (DWORD)0x50005000);    // 사운드 볼륨 조정 오른쪽 왼쪽
	sndPlaySound(".\\Sound\\Tetris_theme.wav", SND_ASYNC | SND_LOOP);    // 음악 재생

	ChangeWorkSize(TETROMINOSIZE * 10, TETROMINOSIZE * 20);

	srand((unsigned int)time(NULL));

	SetTimer(1, 800, FrameProc);
	GameData data = { { { { 0, TETROMINOSIZE },     { TETROMINOSIZE, TETROMINOSIZE }, { TETROMINOSIZE * 2, TETROMINOSIZE }, { TETROMINOSIZE * 3, TETROMINOSIZE }             },     // I
						{ { 0, 0 },                 { 0, TETROMINOSIZE },             { TETROMINOSIZE, TETROMINOSIZE },     { TETROMINOSIZE * 2, TETROMINOSIZE } },     // J
						{ { TETROMINOSIZE * 2, 0 }, { 0, TETROMINOSIZE },             { TETROMINOSIZE, TETROMINOSIZE },     { TETROMINOSIZE * 2, TETROMINOSIZE } },     // L
						{ { TETROMINOSIZE, 0 },     { TETROMINOSIZE * 2, 0 },         { TETROMINOSIZE, TETROMINOSIZE },     { TETROMINOSIZE * 2, TETROMINOSIZE } },     // O
						{ { TETROMINOSIZE, 0 },     { TETROMINOSIZE * 2, 0 },         { 0, TETROMINOSIZE },                 { TETROMINOSIZE, TETROMINOSIZE }     },     // S
						{ { TETROMINOSIZE, 0 },     { 0, TETROMINOSIZE },             { TETROMINOSIZE, TETROMINOSIZE },     { TETROMINOSIZE * 2, TETROMINOSIZE } },     // T
						{ { 0, 0 },                 { TETROMINOSIZE, 0 },             { TETROMINOSIZE, TETROMINOSIZE },     { TETROMINOSIZE * 2, TETROMINOSIZE } } },   // Z
					  { { 0, }, }, { TETROMINOSIZE * 3, 0 }, { { 0, }, }, 0, { 0, }, 0 };
	SetAppData(&data, sizeof(GameData));

	pGameData ap_data = (pGameData)GetAppData();
	setImage();
	setTetromino(ap_data);
	drawTetris(ap_data);

	ShowDisplay();
	return 0;
}

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

	ap_data->background_image = LoadImageGP(".\\Background\\Tet.png");    // Background
}

void setTetromino(pGameData p_data)
{
	p_data->currTetromino = (BYTE)(rand() % 7);
	memcpy(p_data->drawTet, p_data->Tetrominoes[p_data->currTetromino], sizeof(POINT) * 4);
}

bool check(pGameData p_data)
{

	return true;
}

void move(pGameData p_data)
{

}

void drawTetris(pGameData p_data)
{
	Clear();

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			DrawImageGP(p_data->background_image, j * TETROMINOSIZE, i * TETROMINOSIZE, TETROMINOSIZE, TETROMINOSIZE);
		}
	}

	for (int i = 0; i < 4; i++) {
		DrawImageGP(p_data->tetromino_image[p_data->currTetromino], p_data->drawTet[i].x + p_data->move.x, p_data->drawTet[i].y + p_data->move.y, TETROMINOSIZE, TETROMINOSIZE);
	}

	ShowDisplay();
}

void spin(pGameData p_data)
{
	LONG temp;
	LONG tempX, tempY;

	switch (p_data->currTetromino) {
	case MOTet:
		break;
	case MITet:
		for (int i = 0; i < 4; i++) {
			tempX = p_data->drawTet[i].y + p_data->move.x;
			tempY = (TETROMINOSIZE * 3) - p_data->drawTet[i].x + p_data->move.y;

			if (tempX < 0 || tempX > TETROMINOSIZE * 9 || tempY > TETROMINOSIZE * 19)
				return;
		}

		for (int i = 0; i < 4; i++) {
			temp = p_data->drawTet[i].x;
			p_data->drawTet[i].x = p_data->drawTet[i].y;
			p_data->drawTet[i].y = (TETROMINOSIZE * 3) - temp;
		}
		break;
	default:
		for (int i = 0; i < 4; i++) {
			tempX = p_data->drawTet[i].y + p_data->move.x;
			tempY = (TETROMINOSIZE * 2) - p_data->drawTet[i].x + p_data->move.y;

			if (tempX < 0 || tempX > TETROMINOSIZE * 9 || tempY > TETROMINOSIZE * 19)
				return;
		}

		for (int i = 0; i < 4; i++) {
			temp = p_data->drawTet[i].x;
			p_data->drawTet[i].x = p_data->drawTet[i].y;
			p_data->drawTet[i].y = (TETROMINOSIZE * 2) - temp;
		}
		break;
	}
}