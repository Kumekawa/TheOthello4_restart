#pragma once
#include "BasePlayer.h"

//左上からローラー作戦
class PlayerRoler :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerRoler(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF = true);
};
