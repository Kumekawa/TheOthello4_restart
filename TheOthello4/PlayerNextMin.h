#pragma once
#include "BasePlayer.h"

//�����ŏ��̂��̂�T��
class PlayerNextMin :public BasePlayer {
	vector<fieldstone> nextStones;
	bool SetPosition() override;
public:
	PlayerNextMin(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF = true);
};
