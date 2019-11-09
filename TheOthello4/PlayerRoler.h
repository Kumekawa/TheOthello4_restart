#pragma once
#include "BasePlayer.h"

//¶ã‚©‚çƒ[ƒ‰[ìí
class PlayerRoler :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerRoler(Field* field, eFieldColor myColor, bool saveF = true);
};
