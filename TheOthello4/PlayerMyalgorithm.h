#pragma once
#include "BasePlayer.h"

class PlayerMyAlgorithm :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerMyAlgorithm(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF = true);
};
