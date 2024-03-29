#pragma once
#include "BasePlayer.h"

//DeepLearnigのようなものを作ってみる
//機械学習っぽくなりました
class PlayerDeep :public BasePlayer {
	double w[MFS_AMOUNT - 4][MFS_XSIZE][MFS_YSIZE];

	FILE *fp;
	string fname;

	double GetValue(fieldstone fieldStone, int turn);

	void SetWeight();

	bool SetPosition() override;
public:
	PlayerDeep(Field* field, eFieldColor myColor, bool saveF = true);
};
