#pragma once
#include "BasePlayer.h"

//完全ランダム
class PlayerRandom :public BasePlayer {
	vector<fieldstone> nextStones;
	bool SetPosition() override;
public:
	PlayerRandom(Field* field, eFieldColor myColor, bool saveF = true);
};