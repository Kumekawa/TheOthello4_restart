#pragma once
#include "BasePlayer.h"

//���ɑ��肪����萔���ŏ��ɂȂ�悤�ɑI��
class PlayerNextPointMin :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerNextPointMin(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF = true);
};
