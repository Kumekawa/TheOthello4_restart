#pragma once
#include "BasePlayer.h"

//次が最大のものを探す
class PlayerNextMax :public BasePlayer {
	vector<fieldstone> nextStones;
	bool SetPosition() override;
public:
	PlayerNextMax(Field* field, eFieldColor myColor, bool saveF = true);
};
