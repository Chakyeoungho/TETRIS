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
#define TETROMINO_SIZE (30)    // 테트리미노 크기(pixel)
#define FIELD_X_NUM    (10)    // 가로 크기
#define FIELD_Y_NUM    (20)    // 세로 크기
#define LINE_INFO      (1)     // 각 줄에 대한 정보
#define BUFFERZONE     (20)    // 버퍼존(위쪽 여유 공간)

// 키보드 방향
#define LEFT  0x00    // 왼쪽
#define RIGHT 0x01    // 오른쪽

// 회전 방향
#define CLOCKWISE        100    // 시계 방향 
#define COUNTERCLOCKWISE 101    // 반시계 방향

// 타이머 아이디
#define T_FRAME     2000    // 프레임
#define T_LOCKDELAY 2001    // 테트로미노 잠금

#endif // !__CONSTANT_H__
