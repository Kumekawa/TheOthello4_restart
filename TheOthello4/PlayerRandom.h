#pragma once
#include "BasePlayer.h"

//���S�����_��
class PlayerRandom :public BasePlayer {
	vector<fieldstone> nextStones;
	bool SetPosition() override;
public:
	PlayerRandom(Field* field, eFieldColor myColor, bool saveF = true);
};