#pragma once
#include "BasePlayer.h"

//DeepLearnigÇÃÇÊÇ§Ç»Ç‡ÇÃÇçÏÇ¡ÇƒÇ›ÇÈ
class PlayerMaxValue :public BasePlayer {
	double w[MFS_AMOUNT - 4][MFS_XSIZE][MFS_YSIZE];

	double GetValue(fieldstone fieldStone, int turn);


	bool SetPosition() override;
public:
	PlayerMaxValue(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, double weight[MFS_AMOUNT - 4][MFS_XSIZE][MFS_YSIZE], bool saveF = true);
};
