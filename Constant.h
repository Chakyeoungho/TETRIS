#ifndef __CONSTANT_H__
#define __CONSTANT_H__

// 테트리미노 종류
enum Tetromino {
	// Tetromino
	MITet,    // I
	MJTet,    // J
	MLTet,    // L
	MOTet,    // O
	MSTet,    // S
	MTTet,    // T
	MZTet,    // Z
	M_Tet,    // _

	// Ghost
	GITet,    // I
	GJTet,    // J
	GLTet,    // L
	GOTet,    // O
	GSTet,    // S
	GTTet,    // T
	GZTet     // Z
};

// 게임 상태
#define STARTPAGE 1000    // 시작 페이지
#define PLAYGAME  1001    // 플레이 중
#define GAMEOVER  1002    // 게임 오버

// 타이머 아이디
#define T_FRAME     2000    // 프레임
#define T_LOCKDELAY 2001    // 테트로미노 잠금

// 표준 크기
#define TETROMINO_SIZE (30)    // 테트리미노 크기(pixel)
#define FIELD_X_NUM    (10)    // 가로 크기
#define LINE_INFO      (1)     // 각 줄에 대한 정보
#define FIELD_Y_NUM    (20)    // 세로 크기
#define BUFFERZONE     (20)    // 버퍼존(위쪽 여유 공간)

// 테트로미노 모양 데이터
const POINT tetrominoesData[7][4] = { { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 } },      // I
									  { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },      // J
									  { { 2, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },      // L
									  { { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } },      // O
									  { { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } },      // S
									  { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },      // T
									  { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } } };    // z

// Wall Kick 데이터 (y좌표 반전)
const POINT wallKickData[2][4][5] = { { { { 0, 0 }, { -2, 0 }, {  1,  0 }, { -2,  1 }, {  1, -2 } },	   //         I         0>>1
										{ { 0, 0 }, { -1, 0 }, {  2,  0 }, { -1, -2 }, {  2,  1 } }, 	   //         I         1>>2
										{ { 0, 0 }, {  2, 0 }, { -1,  0 }, {  2, -1 }, { -1,  2 } },	   //         I         2>>3
										{ { 0, 0 }, {  1, 0 }, { -2,  0 }, {  1,  2 }, { -2, -1 } } },     //         I         3>>0
									  { { { 0, 0 }, { -1, 0 }, { -1, -1 }, {  0,  2 }, { -1,  2 } },       // J, L, O, S, T, Z  0>>1
										{ { 0, 0 }, {  1, 0 }, {  1,  1 }, {  0, -2 }, {  1, -2 } },       // J, L, O, S, T, Z  1>>2
										{ { 0, 0 }, {  1, 0 }, {  1, -1 }, {  0,  2 }, {  1,  2 } },       // J, L, O, S, T, Z  2>>3
										{ { 0, 0 }, { -1, 0 }, { -1,  1 }, {  0, -2 }, { -1, -2 } } } };   // J, L, O, S, T, Z  3>>0

#endif // !__CONSTANT_H__
