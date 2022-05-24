#ifndef __CONSTANT_H__
#define __CONSTANT_H__

enum Tetromino {
	MITet,
	MJTet,
	MLTet,
	MOTet,
	MSTet,
	MTTet,
	MZTet,
	M_Tet
};

// 게임 상태
#define STARTPAGE 1000
#define PLAYGAME  1001
#define GAMEOVER  1002

// 표준 크기
#define FIELD_X_NUM (10)
#define FIELD_Y_NUM (28)
#define FREESPACE_NUM (FIELD_Y_NUM - (20))
#define TETROMINO_SIZE (30)
#define LINEINFO (1)

#endif // !__CONSTANT_H__
