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

// 점수 체계
enum Action {
	Nothing,              // 0
	Single,               // 100 x Level
	Double,               // 300 x Level
	Triple,               // 500 x Level
	Tetris,               // 800 x Level
	MiniT_Spin,           // 100 x Level
	MiniT_SpinSingle,     // 200 x Level
	T_Spin,               // 400 x Level
	T_SpinSingle,         // 800 x Level
	T_SpinDouble,         // 1200 x Level
	T_SpinTriple,         // 1600 x Level
	Back_to_BackBonus,    // 0.5 x Action Total
	SoftDrop,             // 1 x n
	HardDrop              // 2 x m
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
#define LINE_INFO       (1)    // 각 줄에 대한 정보
#define FIELD_Y_NUM    (20)    // 세로 크기
#define BUFFERZONE     (20)    // 버퍼존(위쪽 여유 공간)
#define CHANCE         (10)    // 땅에 닿고 움직일 수 있는 횟수

// 테트로미노 교체
#define SWAPTET(tet1, tet2) do { \
	BYTE tempTet = tet1;    \
	tet1 = tet2;            \
	tet2 = tempTet;         \
} while(0)

// 레벨에 따른 시간(Time By Level)
#define TBYS(gameLevel) (UINT)(pow(0.8 - ((double)(gameLevel - 1) * 0.007), gameLevel - 1) * 1000)

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
