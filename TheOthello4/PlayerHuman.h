#pragma once
#include "BasePlayer.h"

//�}�E�X����
class PlayerHuman :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerHuman(Field* field, eFieldColor myColor, bool saveF = true);
};
