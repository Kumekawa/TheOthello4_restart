#pragma once
#include "BasePlayer.h"

//DeepLearnig�̂悤�Ȃ��̂�����Ă݂�
//�@�B�w�K���ۂ��Ȃ�܂���
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
