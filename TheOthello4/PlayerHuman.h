#pragma once
#include "BasePlayer.h"

//ƒ}ƒEƒX“ü—Í
class PlayerHuman :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerHuman(Field* field, eFieldColor myColor, bool saveF = true);
};
