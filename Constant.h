#ifndef __CONSTANT_H__
#define __CONSTANT_H__

// 테트리미노 종류
enum Tetromino {
	MITet,    // I
	MJTet,    // J
	MLTet,    // L
	MOTet,    // O
	MSTet,    // S
	MTTet,    // T
	MZTet,    // Z
	M_Tet     // _
};

// 게임 상태
#define STARTPAGE 1000    // 시작 페이지
#define PLAYGAME  1001    // 플레이 중
#define GAMEOVER  1002    // 게임 오버

// 표준 크기
#define TETROMINO_SIZE (30)    // 테트리미노 크기
#define FIELD_X_NUM (10)    // 가로 크기
#define LINEINFO (1)    // 각 줄에 대한 정보
#define FIELD_Y_NUM (24)    // 세로 크기
#define DEADLINE (4)        // 이 이상 넘어오면 게임 오버
#define FREESPACE_NUM (FIELD_Y_NUM + DEADLINE - (20))    // 위에 안보이는 곳

// 키보드 방향
#define LEFT  0    // 왼쪽
#define RIGHT 1    // 오른쪽

#endif // !__CONSTANT_H__
