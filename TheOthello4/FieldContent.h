#pragma once
#include "MacroWindowSize.h"
#include <string>
#include <sstream>
using namespace std;

constexpr int MFS_XSIZE = 8;
constexpr int MFS_YSIZE = 8;
constexpr int MFS_WIDTH = MWS_YMAX;
constexpr int MFS_HEIGHT = MWS_YMAX;
constexpr int MFS_UNIT = MFS_WIDTH / MFS_XSIZE;
constexpr int MFS_AMOUNT = MFS_XSIZE * MFS_YSIZE;

enum eFieldColor {
	eFC_Black,
	eFC_White,
	eFC_None
};
eFieldColor GetChangeFieldColor(eFieldColor turn);

void ChangeFieldColor(eFieldColor* turn);

struct fieldstone {
	//�ǂ��ɒu���Ă��̔ՖʂɂȂ�����
	int x, y;
	//���ۂ̔Ֆ�
	eFieldColor stone[MFS_XSIZE][MFS_YSIZE];
	//���ۂɂ��������邩
	int amount[3];
	//�ǂꂪ��ԑ�����
	eFieldColor GetMaxColor();

	void SetAmount();
	//�Ֆʂ𕶎���ŕ\���A�Ō��x,y��t�^����
	string GetString();

	bool GetEquals(fieldstone fieldStone);
};
