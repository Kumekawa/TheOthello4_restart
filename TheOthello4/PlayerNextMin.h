#pragma once
#include "BasePlayer.h"

//次が最小のものを探す
class PlayerNextMin :public BasePlayer {
	vector<fieldstone> nextStones;
	bool SetPosition() override;
public:
	PlayerNextMin(Field* field, eFieldColor myColor, bool saveF = true);
};
