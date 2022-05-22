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
	BYTE playfield[FIELD_Y_NUM][FIELD_X_NUM];
	BYTE currTetromino;
	void *tetromino_image[8];
} GameData, *pGameData;

void setImage();
void setTetromino(pGameData p_data);
bool check(pGameData p_data);
void setData(pGameData p_data);
void removeData(pGameData p_data);
void drawTetris(pGameData p_data);
void spin(pGameData p_data);

TIMER FrameProc(NOT_USE_TIMER_DATA)
{
	pGameData ap_data = (pGameData)GetAppData();

	if (ap_data->drawTet[0].y + ap_data->move.y < FIELD_Y_NUM - 1 &&
		ap_data->drawTet[1].y + ap_data->move.y < FIELD_Y_NUM - 1 &&
		ap_data->drawTet[2].y + ap_data->move.y < FIELD_Y_NUM - 1 &&
		ap_data->drawTet[3].y + ap_data->move.y < FIELD_Y_NUM - 1) {
		removeData(ap_data);
		ap_data->move.y++;
		setData(ap_data);
	}
	drawTetris(ap_data);
}

// 사용자가 메시지를 직접 처리할 때 사용하는 함수
int OnUserMsg(HWND ah_wnd, UINT a_message_id, WPARAM wParam, LPARAM lParam)
{
	pGameData p_data = (pGameData)GetAppData(); // 프로그램의 내부 데이터 주소를 가져온다.

	if (a_message_id == WM_KEYDOWN) {  // 키보드의 버튼이 눌러졌을 때
		// 어떤 키를 눌렀는지에 대한 정보가 wParam 변수에 들어있다. VK는 Virtual Key의 약자이고
		// 방향키는 VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT로 정의되어 있다.
		switch (wParam)
		{
		case VK_UP:
			removeData(p_data);
			spin(p_data);
			setData(p_data);
			drawTetris(p_data);
			break;
		case VK_DOWN:   // 아래쪽 버튼
			if (p_data->drawTet[0].y + p_data->move.y < FIELD_Y_NUM - 1 &&
				p_data->drawTet[1].y + p_data->move.y < FIELD_Y_NUM - 1 &&
				p_data->drawTet[2].y + p_data->move.y < FIELD_Y_NUM - 1 &&
				p_data->drawTet[3].y + p_data->move.y < FIELD_Y_NUM - 1) {
				removeData(p_data);
				p_data->move.y++;
				setData(p_data);
				drawTetris(p_data);
			}
			break;
		case VK_LEFT:   // 왼쪽 버튼
			if (p_data->drawTet[0].x + p_data->move.x >= 1 &&
				p_data->drawTet[1].x + p_data->move.x >= 1 &&
				p_data->drawTet[2].x + p_data->move.x >= 1 &&
				p_data->drawTet[3].x + p_data->move.x >= 1) {
				removeData(p_data);
				p_data->move.x--;
				setData(p_data);
				drawTetris(p_data);
			}
			break;
		case VK_RIGHT:   // 오른쪽 버튼
			if (p_data->drawTet[0].x + p_data->move.x < FIELD_X_NUM - 1 &&
				p_data->drawTet[1].x + p_data->move.x < FIELD_X_NUM - 1 &&
				p_data->drawTet[2].x + p_data->move.x < FIELD_X_NUM - 1 &&
				p_data->drawTet[3].x + p_data->move.x < FIELD_X_NUM - 1) {
				removeData(p_data);
				p_data->move.x++;
				setData(p_data);
				drawTetris(p_data);
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

ON_MESSAGE(NULL, NULL, NULL, NULL, NULL, OnUserMsg)

int main()
{
	waveOutSetVolume(0, (DWORD)0x25002500);    // 사운드 볼륨 조정 오른쪽 왼쪽
	sndPlaySound(".\\Sound\\Tetris_theme.wav", SND_ASYNC | SND_LOOP);    // 음악 재생

	ChangeWorkSize(TETROMINO_SIZE * FIELD_X_NUM + 400, TETROMINO_SIZE * (FIELD_Y_NUM - FREESPACE_NUM));

	srand((unsigned int)time(NULL));

	SetTimer(1, 800, FrameProc);
	GameData data = { { { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 } },     // I
						{ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },     // J
						{ { 2, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },     // L
						{ { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } },     // O
						{ { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } },     // S
						{ { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },     // T
						{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } } },   // Z
					  { { 0, }, }, { 3, 8 }, { { 0, }, }, 0, { 0, } };
	SetAppData(&data, sizeof(GameData));

	pGameData ap_data = (pGameData)GetAppData();
	setImage();
	setTetromino(ap_data);
	memset(ap_data->playfield, Tet, sizeof(BYTE) * FIELD_Y_NUM * FIELD_X_NUM);
	setData(ap_data);
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
	ap_data->tetromino_image[Tet]   = LoadImageGP(".\\Tetromino\\Tet.png");      // Background
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

void setData(pGameData p_data)
{
	for (int i = 0; i < 4; i++) {
		p_data->playfield[p_data->drawTet[i].y + p_data->move.y][p_data->drawTet[i].x + p_data->move.x] = p_data->currTetromino;
	}
}

void removeData(pGameData p_data)
{
	for (int i = 0; i < 4; i++) {
		p_data->playfield[p_data->drawTet[i].y + p_data->move.y][p_data->drawTet[i].x + p_data->move.x] = Tet;
	}
}

void drawTetris(pGameData p_data)
{
	Clear();

	for (int y = 8; y < FIELD_Y_NUM; y++) {
		for (int x = 0; x < FIELD_X_NUM; x++) {
			TextOut(x * FIELD_X_NUM + 450, (y - FREESPACE_NUM) * (FIELD_Y_NUM - FREESPACE_NUM) + 50, "%d", p_data->playfield[y][x]);
		}
	}

	for (int y = FREESPACE_NUM; y < FIELD_Y_NUM; y++) {
		for (int x = 0; x < FIELD_X_NUM; x++) {
			DrawImageGP(p_data->tetromino_image[p_data->playfield[y][x]], x * TETROMINO_SIZE, (y - FREESPACE_NUM) * TETROMINO_SIZE, TETROMINO_SIZE, TETROMINO_SIZE);
		}
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
			tempY = 3 - p_data->drawTet[i].x + p_data->move.y;

			if (tempX < 0 || tempX > (FIELD_X_NUM - 1) || tempY > (FIELD_Y_NUM - 1))
				return;
		}

		for (int i = 0; i < 4; i++) {
			temp = p_data->drawTet[i].x;
			p_data->drawTet[i].x = p_data->drawTet[i].y;
			p_data->drawTet[i].y = 3 - temp;
		}
		break;
	default:
		for (int i = 0; i < 4; i++) {
			tempX = p_data->drawTet[i].y + p_data->move.x;
			tempY = 2 - p_data->drawTet[i].x + p_data->move.y;

			if (tempX < 0 || tempX > (FIELD_X_NUM - 1) || tempY > (FIELD_Y_NUM - 1))
				return;
		}

		for (int i = 0; i < 4; i++) {
			temp = p_data->drawTet[i].x;
			p_data->drawTet[i].x = p_data->drawTet[i].y;
			p_data->drawTet[i].y = 2 - temp;
		}
		break;
	}
}