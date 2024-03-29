#pragma once
#include "BasePlayer.h"

//DeepLearnigのようなものを作ってみる
class PlayerMaxValue :public BasePlayer {
	double w[MFS_AMOUNT - 4][MFS_XSIZE][MFS_YSIZE];

	double GetValue(fieldstone fieldStone, int turn);


	bool SetPosition() override;
public:
	PlayerMaxValue(Field* field, eFieldColor myColor, double weight[MFS_AMOUNT - 4][MFS_XSIZE][MFS_YSIZE], bool saveF = true);
};
