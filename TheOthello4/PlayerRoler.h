#pragma once
#include "BasePlayer.h"

//���ォ�烍�[���[���
class PlayerRoler :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerRoler(Field* field, eFieldColor myColor, bool saveF = true);
};