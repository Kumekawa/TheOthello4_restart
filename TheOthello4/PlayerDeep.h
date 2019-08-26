#pragma once
#include "BasePlayer.h"

//DeepLearnig‚Ì‚æ‚¤‚È‚à‚Ì‚ğì‚Á‚Ä‚İ‚é
//‹@ŠBŠwK‚Á‚Û‚­‚È‚è‚Ü‚µ‚½
class PlayerDeep :public BasePlayer {
	double w[MFS_AMOUNT - 4][MFS_XSIZE][MFS_YSIZE];

	FILE *fp;
	string fname;

	double GetValue(fieldstone fieldStone, int turn);

	void SetWeight();

	bool SetPosition() override;
public:
	PlayerDeep(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF = true);
};
