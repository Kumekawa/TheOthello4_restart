#pragma once
#include "BasePlayer.h"

//�����ő�̂��̂�T��
class PlayerNextMax :public BasePlayer {
	vector<fieldstone> nextStones;
	bool SetPosition() override;
public:
	PlayerNextMax(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF = true);
};
