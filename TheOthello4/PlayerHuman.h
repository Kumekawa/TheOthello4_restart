#pragma once
#include "BasePlayer.h"

//マウス入力
class PlayerHuman :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerHuman(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF = true);
};
