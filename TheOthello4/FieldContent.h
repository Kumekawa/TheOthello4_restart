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
	//どこに置いてこの盤面になったか
	int x, y;
	//実際の盤面
	eFieldColor stone[MFS_XSIZE][MFS_YSIZE];
	//実際にいくつずつあるか
	int amount[3];
	//どれが一番多いか
	eFieldColor GetMaxColor();

	void SetAmount();
	//盤面を文字列で表し、最後にx,yを付与する
	string GetString();

	bool GetEquals(fieldstone fieldStone);
};
